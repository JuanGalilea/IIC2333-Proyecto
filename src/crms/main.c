#include <stdio.h>
#include <string.h> 
#include <unistd.h> // exce
#include <sys/types.h> // pid_t
#include <stdlib.h>
#include <math.h> // floor()
#include <string.h>
#include "../api/functions.h"


// ********************************** PROGRAMA PRINCIPAL *******************************************

int main(int argc, char **argv)
{
  printf("Operative system project init\n");
  char *filename;
  if( argc >= 2){
    filename = argv[1];
  }else{
    filename = "memfilled.bin";
  }
  printf("Memory name: %s\n",filename);
  cr_mount(filename);
}
