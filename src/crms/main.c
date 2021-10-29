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
  printf("\n");
  printf("\n");
  printf("--------------------------Funciones Generales--------------------------\n");
  printf("\n");
  if( argc == 1){
    return 1;
  }
  cr_mount(argv[1]);
  printf("\n");
  printf("\n");
  cr_ls_processes();
  printf("\n");
  printf("\n");
  printf("Ingresa el PID del proceso, para ver sus archivos: ");
  int i;
  scanf("%i", &i);
  cr_ls_files(i);
  
  char filename[50];
  printf("Ingresa el nombre del archivo, para ver si existe: ");
  scanf("%s", filename);
  int ex = cr_exists1(i,filename);
  if (ex == 1){
    printf("File exists in process.\n");
  }else{
    printf("File not exists in process.\n");
  }
  printf("\n");
  printf("\n");
  cr_ls_files(i);
  printf("\n");
  printf("\n");

  printf("--------------------------Start Process--------------------------\n");
  printf("\n");
  printf("\n");
  char nombre[50];
  printf("Ingresa el nombre del proceso a iniciar: ");
  scanf("%s", nombre);
  cr_start_process(40, nombre);
  //for(int i =0; i<21; i++){
  //  int process_id = i;
  //  char *process_name = "abcdefghijkl";
  //  cr_start_process(process_id, process_name);
  //}
  printf("\n");
  printf("\n");
  cr_ls_processes();
  printf("\n");
  printf("\n");


  printf("--------------------------Finished Process--------------------------\n");
  printf("\n");
  printf("\n");
  cr_ls_processes();
  printf("Ingresa el PID del proceso que deseas finalizar: ");
  int e;
  scanf("%i", &e);
  cr_finish_process(e);
  printf("\n");
  printf("\n");
  cr_ls_processes();
  printf("\n");
  printf("\n");

  printf("--------------------------Open and close archive--------------------------\n");
  printf("\n");
  printf("\n");
  printf("Ingresa el PID del proceso que deseas mostrar: ");
  int o;
  scanf("%i", &o);
  printf("\n");
  printf("\n");
  cr_ls_files(o);
  printf("\n");
  printf("\n");

  char archivo[50];
  printf("Ingresa el nombre del archivo: ");
  scanf("%s", archivo);

  CrmsFile* archivo1;
  archivo1 = cr_open(o, archivo, 'r');
  printf("\n");
  printf("\n");
  printf("nombre del archivo: %s, pid: %i, size: %i", archivo1->filename, archivo1->process_id, archivo1->size);
  printf("\n");
  printf("\n");
  cr_close(archivo1);
  printf("\n");
  printf("\n");
  printf("--------------------------Delete file--------------------------\n");
  printf("\n");
  printf("\n");
  cr_ls_files(o);
  printf("\n");
  printf("\n");
  char archivo3[50];
  printf("Ingresa el nombre del archivo: ");
  scanf("%s", archivo3);
  CrmsFile* archivo2;
  archivo2 = cr_open(o, archivo3, 'r');
  printf("\n");
  printf("\n");
  printf("nombre del archivo: %s, pid: %i, size: %i", archivo1->filename, archivo1->process_id, archivo1->size);
  printf("\n");
  printf("\n");
  printf("Enter para eliminar archivo: ");
  char archivo4[50];
  scanf("%s", archivo4);
  cr_delete_file(archivo2);
  cr_ls_files(o);
  
}