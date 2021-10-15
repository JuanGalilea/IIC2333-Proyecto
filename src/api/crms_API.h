#pragma once

extern char diskPath[256];
void cr_mount(char* memory_path);
void cr_ls_processes();
int cr_exists(int process_id, char* file_name);
void cr_ls_files(int process_id);
void cr_start_process(int process_id, char *process_name);