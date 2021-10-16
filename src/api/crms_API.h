#pragma once

typedef struct CrmsFile {
    unsigned int size;
    char* filename;
    unsigned int raw_data;
    char mode;
    unsigned int pointer;
    int process_id;
} CrmsFile;

typedef struct VirtualAddress {
    int vpn;
    int offset;
    int size;
} VirtualAddress;

extern char diskPath[256];
void cr_mount(char* memory_path);
void cr_ls_processes();
int cr_exists1(int process_id, char* file_name);
void cr_ls_files(int process_id);
void cr_finish_process(int process_id);
void cr_start_process(int process_id, char *process_name);
CrmsFile* cr_open (int process_id, char* file_name, char mode);
void cr_close(CrmsFile* file_desc);
int cr_write_file(CrmsFile* file_desc, void* buffer, int n_bytes);
