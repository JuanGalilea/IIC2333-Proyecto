#pragma once


typedef struct CrmsFile {
    unsigned int size;
    char* filename;
    unsigned int raw_data;
    char mode;
    unsigned int pointer;
    int process_id;
} CrmsFile;

extern char diskPath[256];
void cr_mount(char* memory_path);
void cr_ls_processes();
int cr_exists1(int process_id, char* file_name);
void cr_ls_files(int process_id);
CrmsFile* cr_open (int process_id, char* file_name, char mode);