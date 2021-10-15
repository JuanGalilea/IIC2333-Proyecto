#include "crms_API.h"
#include <stdio.h>
#include <string.h>

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
            return;
        }

        
    }
    printf("Pid not found\n");
    fclose(fileDisk);
}

void cr_finish_process(int process_id){
    
    printf("List of files in process with pid %i.\n",process_id);
    FILE *fileDisk;

    fileDisk = fopen(diskPath,"rb"); 

    int entry = find_process_entry(process_id);
    if (entry == -1){
        return NULL;    // proceso no tiene entrada
    }

    FILE *readDisk;
    readDisk = fopen(diskPath,"rb");
    fseek(readDisk, entry*256, SEEK_SET);
    unsigned char buffer[256];
    fread(buffer,sizeof(buffer),1,readDisk);

            
    //cambiar byte de validez
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
        int aux = 8;
        int pos_bit = 3;
        int pos_byte = 17;
        for(int i = 0; i<5; i++){
            if (pos_bit == 0) {
                pos_bit = 7;
                pos_byte++;
                aux = 128;
            }
            
            int bit = (buffer[14+j*21+pos_byte]>> pos_bit) & 1;
            bit *= aux;
            vpn += bit;

            aux /= 2;
            pos_bit--;
        }

        int offset = 0;
        for(int i = 0; i<23; i++){
            if (pos_bit == 0) {
                pos_bit = 7;
                pos_byte++;
                aux = 128;
            }
            
            int bit = (buffer[14+j*21+pos_byte]>> pos_bit) & 1;
            bit *= aux;
            offset += bit;

            aux /= 2;
            pos_bit--;
        }
    }
    return;

        
    
    printf("Pid not found\n");
    fclose(fileDisk);
}


int get_vpn(int process_id){

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
                int count = 0;
                for(int k = 0; k<12; k++){
                    if(file_name[k] == (char) buffer[14+j*21+k+1]){
                        count++;
                    }
                }
                if(count == 12){
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



// -1 si no se encuentra, posicion de la subentrada si si se encuentra. 
// TODO: posible fallo en comparacion debido a los null
// int cr_find_file(int entry, char* file_name){
//     printf("Checking if file %s exist in entry %i, and where.\n",file_name, entry);
//     FILE *fileDisk;

//     fileDisk = fopen(diskPath,"rb"); 

//     fseek(fileDisk, entry*256, SEEK_SET);
//     unsigned char buffer[256];
//     fread(buffer,sizeof(buffer),1,fileDisk);

//     for(int j=0;j<10;j++){
//         if(buffer[14+j*21] != 0x01){
//             continue;
//         }
//         char filename[12];
//         for(int k = 0; k<12; k++){
//             sprintf(&filename[k],"%c",(char) buffer[14+j*21+k+1]);
//         }

//         if(strcmp(filename, file_name) == 0){
//             fclose(fileDisk);
//             return j;
//         }
//     }
//     fclose(fileDisk);
//     return -1;
// }


int find_process_entry (int process_id) {
    printf("Looking for the entry of process %i.\n", process_id);
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


// CrmsFile* cr_open (int process_id, char* file_name, char mode) {
//     CrmsFile* output = malloc(sizeof(CrmsFile));

//     int entry = find_process_entry(process_id);
//     if (entry == -1){
//         return NULL;    // proceso no tiene entrada
//     }
//     int subentry = cr_find_file(entry, file_name);

//     output->mode = mode;
//     output->process_id = process_id;
//     output->filename = malloc(12*sizeof(char));

//     FILE *readDisk;
//     readDisk = fopen(diskPath,"rb");
//     fseek(readDisk, entry*256, SEEK_SET);
//     unsigned char readBuffer[256];
//     fread(readBuffer,sizeof(readBuffer),1,readDisk);

//     if (mode == 'r' && -1 != subentry){
//         // modo de lectura y el archivo si existe
//         // abrimos disco


//         // 1 de validez
//         // 12 de nombre
//         for(int k = 0; k<12; k++){
//             sprintf(&output->filename[k],"%c",(char) readBuffer[14+subentry*21+k+1]);
//         }
//         // 4 para el tamaño
//         int aux = 0;
//         for(int k = 0; k<4; k++){
//             aux *= 256;                                 // hacemos espacio para meter el proximo byte
//             aux += (int) readBuffer[14+subentry*21+13+k];
//         }
//         output->size = aux;

//         // 4 para dirección virtual
//         aux = 0;
//         for(int k = 0; k<4; k++){
//             aux *= 256;                                 // hacemos espacio para meter el proximo byte
//             aux += (int) readBuffer[14+subentry*21+17+k];
//         }
//         output->raw_data = aux;

//         fclose(readDisk);
//         return output;
//     }

//     else if (mode == 'w' &&  !subentry) {
//         // modo de lectura y el archivo si existe
//         int aux = -1;
//         for (int i = 0; i < 10; i++){
//             if (readBuffer[14+i*21] == 0x00){
//                 aux = i;
//                 break;
//             }
//         }
//         fclose(readDisk);

//         if (aux == -1){
//             return NULL;
//         }

//         FILE* writeDisk;
//         writeDisk = fopen(diskPath, "wb");
//         fseek(writeDisk, entry*256+14+21*aux, SEEK_SET);
        
        
//         char* to_write = malloc(21*sizeof(char));

//         // byte de validez
//         to_write[0] = 0x01;

//         // 12 bytes del nombre
//         for(int k = 0; k<12; k++){
//             sprintf(&to_write[1+k],"%c",(char) file_name[k]);   // TODO: filename de 12? no necesariamente
//             sprintf(&output->filename[k],"%c",(char) file_name[k]);
//         }

//         // 4 bytes del tamaño (no hemos escrito todavía)
//         output->size = 0;
//         to_write[13] = 0x00;
//         to_write[14] = 0x00;
//         to_write[15] = 0x00;
//         to_write[16] = 0x00;

//         // 4 bytes de la direccion virtual TODO: cambiar al valor correcto
//         output->raw_data = 0;
//         to_write[17] = 0x00;
//         to_write[18] = 0x00;
//         to_write[19] = 0x00;
//         to_write[20] = 0x00;

//         fwrite(to_write, sizeof(char), 21, writeDisk);
//         fclose(fwrite);
//         return output;
//     }

//     else {
//         free(output->filename);
//         free(output);
//         // ERROROROOROROROOR
//     }

// }