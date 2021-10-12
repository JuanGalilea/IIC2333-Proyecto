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

