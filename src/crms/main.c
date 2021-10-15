#include <stdio.h>
#include <string.h> 
#include <unistd.h> // exce
#include <sys/types.h> // pid_t
#include <stdlib.h>
#include <math.h> // floor()
#include <string.h>
#include "../api/crms_API.h"



char diskPath[256]; //char with the path of disk
int main(int argc, char **argv)
{
  printf("Operative system project init\n");
  
  if( argc == 1){
    return 1;
  }
  cr_mount(argv[1]);
  printf("\n");
  cr_ls_processes();
  printf("\n");
  cr_ls_files(27);
  char* filename = "greatcat.mp4";
  int ex = cr_exists1(27,filename);
  cr_finish_process(27);

  // CrmsFile* archivo;
  // archivo = cr_open(200, "greatcat.mp4", "w");

  if (ex == 1){
    printf("File exists in process.\n");
  }else{
    printf("File not exists in process.\n");
  }
  for(int i =0; i<21; i++){
    int process_id = i;
    char *process_name = "abcdefghijkl";
    cr_start_process(process_id, process_name);
  }

  cr_ls_processes();
  printf("\n");
}