#include "crms_API.h"
#include <stdio.h>
#include <string.h>
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
    int process = 0;
    for(int i = 0; i < 16; i++){
        // Se establece la posición del puntero del archivo en la página i de PCB
        fseek(fileDisk, i*256, SEEK_SET);
        //Se guardan en un buffer los 256 bytes 
        unsigned char buffer[256];
        fread(buffer,sizeof(buffer),1,fileDisk);
        if(buffer[0] != 0x01){
            continue;
        }
        process+=1;
        unsigned int pid = buffer[1];
        printf("PID: %i   ", pid);
        char name[12];
        for(int j = 0; j<12; j++){
            sprintf(&name[j],"%c",(char) buffer[2+j]);
        }
        printf("Process name: %s \n",name);
    }
    if (process == 0){
        printf("  No process running\n");
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

