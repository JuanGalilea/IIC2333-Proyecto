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
  cr_ls_processes();
}
