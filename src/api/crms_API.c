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
        for(int i = 0; i<12; i++){
            sprintf(&name[i],"%c",(char) buffer[2+i]);
        }
        printf("Process name: %s \n",name);
    }
    
    fclose(fileDisk);
    
}