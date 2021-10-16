#include "crms_API.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

int len_string(char* string){
    int i=0;
    while(1){
        if (string[i] == '\0'){
            break;
        }
        i++;
    }
    return i;
}

void cr_mount(char* memory_path){
    strcpy(diskPath,memory_path);
    printf("Mount memory in %s \n",diskPath);
}

void cr_ls_processes(){
    printf("List of running process in %s:\n",diskPath);
    FILE *fileDisk;

    fileDisk = fopen(diskPath,"rb"); 

    for(int i = 0; i < 16; i++){
        // Se establece la posición del puntero del archivo en la página i de PCB
        fseek(fileDisk, i*256, SEEK_SET);
        //Se guardan en un buffer los 256 bytes 
        unsigned char buffer[256];
        fread(buffer,sizeof(buffer),1,fileDisk);
        if(buffer[0] != 0x01){
            continue;
        }
        
        unsigned int pid = buffer[1];
        printf("PID: %i   ", pid);
        char name[12];
        for(int j = 0; j<12; j++){
            sprintf(&name[j],"%c",(char) buffer[2+j]);
        }
        printf("Process name: %s \n",name);
    }
    
    fclose(fileDisk);
    
}



void cr_ls_files(int process_id){
    printf("List of files in process with pid %i.\n",process_id);
    FILE *fileDisk;

    fileDisk = fopen(diskPath,"rb"); 

    for(int i = 0; i < 16; i++){
        fseek(fileDisk, i*256, SEEK_SET);
        unsigned char buffer[256];
        fread(buffer,sizeof(buffer),1,fileDisk);
                
        unsigned int pid = buffer[1];
        if (pid == process_id){
            for(int j=0;j<10;j++){
                if(buffer[14+j*21] != 0x01){
                    continue;
                }
                char filename[12];
                for(int k = 0; k<12; k++){
                    sprintf(&filename[k],"%c",(char) buffer[14+j*21+k+1]);
                }
                printf("Filename: %s\n",filename);
            }
            fclose(fileDisk);
            return;
        }

        
    }
    printf("Pid not found\n");
    fclose(fileDisk);
}

int cr_exists(int process_id, char* file_name){
    printf("Checking if file %s exist in process %i.\n",file_name, process_id);
    FILE *fileDisk;

    fileDisk = fopen(diskPath,"rb"); 

    for(int i = 0; i < 16; i++){
        fseek(fileDisk, i*256, SEEK_SET);
        unsigned char buffer[256];
        fread(buffer,sizeof(buffer),1,fileDisk);
                
        unsigned int pid = buffer[1];
        if (pid == process_id){
            for(int j=0;j<10;j++){
                if(buffer[14+j*21] != 0x01){
                    continue;
                }
                char filename[12];
                for(int k = 0; k<12; k++){
                    sprintf(&filename[k],"%c",(char) buffer[14+j*21+k+1]);
                }

                if(strcmp(filename, file_name) == 0){
                    return 1;
                }
            }
            return 0;
        }

        
    }
    
    fclose(fileDisk);
    return 0;
}

// TODO: utilizar cr_find_file para crear esta func???
int cr_exists1(int process_id, char* file_name){
    printf("Checking if file %s exist in process %i.\n",file_name, process_id);

    FILE *fileDisk;

    fileDisk = fopen(diskPath,"rb"); 

    for(int i = 0; i < 16; i++){
        fseek(fileDisk, i*256, SEEK_SET);
        unsigned char buffer[256];
        fread(buffer,sizeof(buffer),1,fileDisk);
                
        unsigned int pid = buffer[1];
        if (pid == process_id){
            fclose(fileDisk);
            return i;
        }   // not found
    }
    fclose(fileDisk);
    return -1;
}

void cr_start_process(int process_id, char *process_name){
    FILE *fileDisk;

    fileDisk = fopen(diskPath,"rb+"); 

    int start = -1;
    int size_string = len_string(process_name);
    if (size_string > 12){
        printf("Invalid size of process name.\n");
        fclose(fileDisk);
        return;
    }
    uint8_t pid ;
    if (0 <= process_id && process_id <256){
        pid = process_id;
    }else{
        printf("Invalid range of process id.\n");
        fclose(fileDisk);
        return;
    }
        
    for(int i = 0; i < 16; i++){
        fseek(fileDisk, i*256, SEEK_SET);
        unsigned char buffer[256];
        fread(buffer,sizeof(buffer),1,fileDisk);
        if (buffer[0] == 0x00 && start == -1){
            start = i;
        }        
        unsigned int pid = buffer[1];
        if (pid == process_id && buffer[0]== 0x01){
            printf("Process with id %i exists.\n",process_id);
            fclose(fileDisk);
            return;
        }
    }
    if(start != -1){
        int init = start*256;
        uint8_t valid = 1;
        fseek(fileDisk,init,SEEK_SET);
        fwrite(&valid,1,1,fileDisk);
        fseek(fileDisk,init+1,SEEK_SET);
        fwrite(&pid,sizeof(uint8_t),1,fileDisk);
        for(int k = 0; k<size_string; k++){
            fseek(fileDisk,init+2+k,SEEK_SET);
            fwrite(&process_name[k],sizeof(char),1,fileDisk);
        }
        
        printf("Process with id %i has been created.\n",process_id);
    }else{
        printf("All memory is in used, remove a process to start this process.\n");
        fclose(fileDisk);
        return;
    }
    

    
    fclose(fileDisk);
}


int find_process_entry (int process_id) {
    printf("Looking for the entry of process %i.\n", process_id);
    FILE *fileDisk;

    fileDisk = fopen(diskPath,"rb"); 

    for(int i = 0; i < 16; i++){
        fseek(fileDisk, i*256, SEEK_SET);
        unsigned char buffer[256];
        fread(buffer,sizeof(buffer),1,fileDisk);
                
        unsigned int pid = buffer[1];
        if (pid == process_id && buffer[0] == 0x01){
            fclose(fileDisk);
            return i;
        }   // not found
    }
    fclose(fileDisk);
    return -1;
}

void cr_finish_process(int process_id){
    
    printf("List of files in process with pid %i.\n",process_id);
    FILE *fileDisk;
    fileDisk = fopen(diskPath,"rb+"); 

    int entry = find_process_entry(process_id);
    //entry = entrada en la tabla de pcbs
    if (entry == -1){
        printf("Process doesn't exists :(\n");
        fclose(fileDisk);
        return;    // proceso no tiene entrada
    }

    fseek(fileDisk, entry*256, SEEK_SET);
    unsigned char buffer[256];
    fread(buffer,sizeof(buffer),1,fileDisk);

    printf("Entrada (0 a 16): %i\n", entry);
    for(int j=0;j<10;j++){
        if(buffer[14+j*21] != 0x01){
            continue;
        }
        printf("Entrada archivo (0 a 10): %i\n", j);

        char filename[12];
        for(int k = 0; k<12; k++){
            sprintf(&filename[k],"%c",(char) buffer[14+j*21+k+1]);
        }
        printf("Filename: %s\n",filename);
        
        int vpn = 0;
        int aux = 16;
        int pos_bit = 3;
        int pos_byte = 17;
        for(int i = 0; i<5; i++){
            if (pos_bit == -1) {
                pos_bit = 7;
                pos_byte++;
                aux = 1;
            }
            
            int bit = (buffer[14+j*21+pos_byte]>> pos_bit) & 1;
            bit *= aux; // bit = bit * aux; bit = 0
            vpn += bit;

            aux /= 2;
            pos_bit--;
        }

        //dir = direcc fisica relativa
        // revisar offset 
        int offset = 0;
        aux = 4194304;
        for(int i = 0; i<23; i++){
            if (pos_bit == -1) {
                pos_bit = 7;
                pos_byte++;
            }
            
            int bit = (buffer[14+j*21+pos_byte]>> pos_bit) & 1;
            bit *= aux;
            offset += bit;

            aux /= 2;
            pos_bit--;
        }
        printf("VPN: %i de archivo %s ira a Entrada %i\n", vpn, filename, vpn+1);
        
        //Tabla de paginas 
        // int pos_tabla = vpn;
        // int pfn = (int) buffer[223 + pos_tabla] - 128;
        // int bit_validez = (buffer[222+pos_tabla]) >> 7 && 0x01;
        // long dir = 4096 + 16 + pfn*8388608 + offset;



        
    }
    
    //Cambia nombre a 0
    int init = entry*256;
    for(int k = 0; k<256; k++){
        uint8_t byte = 0;
        fseek(fileDisk,init+k,SEEK_SET);
        fwrite(&byte,1,1,fileDisk);
    }
    
    fclose(fileDisk);
    return;    
}


VirtualAddress find_lowest_space_after (int entry, int vpn, int offset) {
    FILE *fileDisk;
    fileDisk = fopen(diskPath, "rb");

    fseek(fileDisk, entry*256, SEEK_SET);
    unsigned char buffer[256];
    fread(buffer, sizeof(buffer), 1, fileDisk);

    int lowest_vpn = -1;
    int lowest_offset = -1;
    int lowest_entry_size = 0;


    for(int j=0;j<10;j++){
        if(buffer[14+j*21] != 0x01){
            continue;
        }
        // calculo vpn 
        int aux_vpn = 0;
        int aux = 16;
        int pos_bit = 3;
        int pos_byte = 17;
        for(int i = 0; i<5; i++){
            if (pos_bit == -1) {
                pos_bit = 7;
                pos_byte++;
                aux = 1;
            }
            
            int bit = (buffer[14+j*21+pos_byte]>> pos_bit) & 1;
            bit *= aux; // bit = bit * aux; bit = 0
            aux_vpn += bit;

            aux /= 2;
            pos_bit--;
        }
        // necesitamos revisar offset? o pasamos al sgte
        if (aux_vpn >= vpn && (lowest_vpn >= aux_vpn || lowest_vpn == -1)) {
            // calculo offset 
            int aux_offset = 0;
            int aux = 4194304;
            for(int i = 0; i<23; i++){
                if (pos_bit == -1) {
                    pos_bit = 7;
                    pos_byte++;
                }
                
                int bit = (buffer[14+j*21+pos_byte]>> pos_bit) & 1;
                bit *= aux;
                aux_offset += bit;

                aux /= 2;
                pos_bit--;
            }
            // asignaciones
            if (aux_vpn > vpn || (aux_vpn == vpn && aux_offset > offset)) {
                if (lowest_vpn > aux_vpn || lowest_offset > aux_offset || lowest_offset == -1) {
                    lowest_offset = aux_offset;
                    lowest_vpn = aux_vpn;

                    int auxSize = 0;
                    for(int k = 0; k<4; k++){
                        auxSize *= 256;                                 // hacemos espacio para meter el proximo byte
                        auxSize += (int) buffer[14+j*21+13+k];
                    }
                    lowest_entry_size = auxSize;

                }
            }
        }
    }

    // acá en lowers tenemos al sgte espacio ocupado o -1 si no hay nada
    VirtualAddress output;
    output.vpn = lowest_vpn;
    output.offset = lowest_offset;
    output.size = lowest_entry_size;
    fclose(fileDisk);
    return output;
}


// muere si no quedan frames para continuar o se termina espacio contiguo
int cr_write_file(CrmsFile* file_desc, void* buffer, int n_bytes){
    int entry = find_process_entry(file_desc->process_id);

    // Es nuevo? hay que pillar donde escribir y dejarlo en raw_data
    if (file_desc->raw_data >> 24 == 240) {
        return 0;
    }
    
    // empieza la escritura con un raw_data valido
    int vpn = file_desc->raw_data >> 23;
    int offset = (file_desc->raw_data << 9) >> 9;
    VirtualAddress next = find_lowest_space_after(entry, vpn, offset);

    int amount_written = 0;

    // checkeamos espacio
    int append_position = offset + file_desc->size;
    int pages_crossed = 0;
    while (append_position >= (int)(0x01 << 23)) {
        append_position -= (int) (0x01 << 23);
        pages_crossed++;
    }
    if (vpn + pages_crossed >= 32) {
        return 0;
    }

    // si hay siguiente archivo en las pags
    if (next.vpn != -1 || next.offset != -1) {
        // si hay espacio (por lo menos 1 byte) antes del sgte
        if (vpn + pages_crossed < next.vpn || append_position < next.offset) {
            // escribimos
            //Tabla de paginas 
            // int pos_tabla = vpn;
            // int pfn = (int) (buffer[223 + pos_tabla]) - 128;
            // long dir = 4096 + 16 + pfn*8388608 + offset;
            
            int end_write_flag = 0;

            while (1){
                FILE *readWriteDisk;

                readWriteDisk = fopen(diskPath, "rb+");
                // nos paramos en la tabla de paginas del proceso
                fseek(readWriteDisk, 256*entry+224+vpn+pages_crossed, SEEK_SET);
                unsigned char page_table_entry;
                fread(&page_table_entry, sizeof(page_table_entry), 1, readWriteDisk);

                int pfn;
                // checkeamos que sea una pag valida
                if (page_table_entry >= 0x01 << 7) {
                    pfn = (int) ((page_table_entry << 1) >> 1);
                }
                // pedimos un nuevo frame
                else {
                    // abrimos el frame bitmap para buscar un nuevo frame disponible
                    fseek(readWriteDisk, 4096, SEEK_SET);
                    unsigned char frame_bitmap[16];
                    fread(frame_bitmap, sizeof(frame_bitmap), 1, readWriteDisk);
                    int continue_flag = 1;
                    int i;
                    int j;
                    for (i = 0; continue_flag && i<16; i++) {
                        for (j = 0; continue_flag && j < 8; j++) {
                            // está libre este espacio
                            if (!(0x80 & (frame_bitmap[i] << j))) {
                                fseek(readWriteDisk, 4096+i, SEEK_SET);
                                char auxx = frame_bitmap[i] + (0x80 >> j);
                                fwrite(&auxx, sizeof(char), 1, readWriteDisk);
                                continue_flag = 0;
                            }
                        }
                    }
                    if (!continue_flag) {
                        uint8_t aux_pfn = 1*(j)+8*i;
                        // pfn = 1*(j)+8*i;
                        uint8_t auxx = 128 + aux_pfn;
                        fseek(readWriteDisk, 256*entry+224+vpn+pages_crossed, SEEK_SET);
                        fwrite(&auxx, sizeof(uint8_t), 1, readWriteDisk);
                    }
                    // no pillamos frame
                    else {
                        fclose(readWriteDisk);
                        return amount_written;
                    }
                }
                // escribimos
                int bytes_to_write;
                if (vpn + pages_crossed == next.vpn) {
                    bytes_to_write = next.offset - append_position;
                    if (amount_written + bytes_to_write > n_bytes) {
                        bytes_to_write = n_bytes - amount_written;
                    }
                    end_write_flag = 1;
                }
                else {
                    bytes_to_write = (0x01 << 23) - append_position;
                    if (amount_written + bytes_to_write > n_bytes) {
                        bytes_to_write = n_bytes - amount_written;
                        end_write_flag = 1;
                    }
                }
                // ahora escribimos bytes_to_write bytes
                fseek(readWriteDisk, (pfn << 23) + append_position, SEEK_SET);
                fwrite(buffer, sizeof(char), bytes_to_write, readWriteDisk);

                // recuento de cuanto he escrito
                amount_written += bytes_to_write;

                // condiciones de termino
                if (end_write_flag) {
                    fclose(readWriteDisk);
                    return amount_written;
                }

                // pasamos a la sgte página
                pages_crossed++;
                append_position = 0;
            }
            return 0;


        }
        return 0;
    }

    else {
    // si (terminada la pagina) queda otro frame
        return 0;
    }
}



//-1 si no se encuentra, posicion de la subentrada si si se encuentra. 
//TODO: posible fallo en comparacion debido a los null
int cr_find_file(int entry, char* file_name){
    printf("Checking if file %s exist in entry %i, and where.\n",file_name, entry);
    FILE *fileDisk;

    fileDisk = fopen(diskPath,"rb"); 

    fseek(fileDisk, entry*256, SEEK_SET);
    unsigned char buffer[256];
    fread(buffer,sizeof(buffer),1,fileDisk);

    for(int j=0;j<10;j++){
        if(buffer[14+j*21] != 0x01){
            continue;
        }
        char filename[12];
        for(int k = 0; k<12; k++){
            sprintf(&filename[k],"%c",(char) buffer[14+j*21+k+1]);
        }

        if(strcmp(filename, file_name) == 0){
            fclose(fileDisk);
            return j;
        }
    }
    fclose(fileDisk);
    return -1;
}

CrmsFile* cr_open (int process_id, char* file_name, char mode) {
    CrmsFile* output = malloc(sizeof(CrmsFile));

    int entry = find_process_entry(process_id);
    if (entry == -1){
        return 0;    // proceso no tiene entrada
    }
    int subentry = cr_find_file(entry, file_name);

    output->mode = mode;
    output->process_id = process_id;
    output->filename = malloc(12*sizeof(char));

    FILE *readDisk;
    readDisk = fopen(diskPath,"rb");
    fseek(readDisk, entry*256, SEEK_SET);
    unsigned char readBuffer[256];
    fread(readBuffer,sizeof(readBuffer),1,readDisk);

    if (mode == 'r' && -1 != subentry){
        // modo de lectura y el archivo si existe
        // abrimos disco


        // 1 de validez
        // 12 de nombre
        for(int k = 0; k<12; k++){
            sprintf(&output->filename[k],"%c",(char) readBuffer[14+subentry*21+k+1]);
        }
        // 4 para el tamaño
        int aux = 0;
        for(int k = 0; k<4; k++){
            aux *= 256;                                 // hacemos espacio para meter el proximo byte
            aux += (int) readBuffer[14+subentry*21+13+k];
        }
        output->size = aux;

        // 4 para dirección virtual
        aux = 0;
        for(int k = 0; k<4; k++){
            aux *= 256;                                 // hacemos espacio para meter el proximo byte
            aux += (int) readBuffer[14+subentry*21+17+k];
        }
        output->raw_data = aux;

        fclose(readDisk);
        return output;
    }

    else if (mode == 'w' &&  !subentry) {
        // modo de lectura y el archivo si existe
        int aux = -1;
        for (int i = 0; i < 10; i++){
            if (readBuffer[14+i*21] == 0x00){
                aux = i;
                break;
            }
        }
        fclose(readDisk);

        if (aux == -1){
            return NULL;
        }

        FILE* writeDisk;
        writeDisk = fopen(diskPath, "wb");
        fseek(writeDisk, entry*256+14+21*aux, SEEK_SET);
        
        
        char* to_write = malloc(21*sizeof(char));

        // byte de validez
        to_write[0] = 0x01;

        // 12 bytes del nombre
        for(int k = 0; k<12; k++){
            sprintf(&to_write[1+k],"%c",(char) file_name[k]);   // TODO: filename de 12? no necesariamente
            sprintf(&output->filename[k],"%c",(char) file_name[k]);
        }

        // 4 bytes del tamaño (no hemos escrito todavía)
        output->size = 0;
        to_write[13] = 0x00;
        to_write[14] = 0x00;
        to_write[15] = 0x00;
        to_write[16] = 0x00;

        // 4 bytes de la direccion virtual TODO: cambiar al valor correcto
        output->raw_data = 240 << 24;
        // si los primeros 4 son 1 entonces no tiene memoria asignada todavía
        to_write[17] = 0xf0;    // 4 bits no significativos + 4 de VPN
        to_write[18] = 0x00;    // ultimo bit del VPN y inicio del offset
        to_write[19] = 0x00;    // offset
        to_write[20] = 0x00;    // offset

        fwrite(to_write, sizeof(char), 21, writeDisk);
        fclose(writeDisk);
        return output;
    }

    else {
        free(output->filename);
        free(output);
        return NULL;
        // ERROROROOROROROOR
    }

}



void cr_close(CrmsFile* file_desc){
    printf("Closing file %s\n.", file_desc->filename);
    free(file_desc->filename);
    free(file_desc);
}