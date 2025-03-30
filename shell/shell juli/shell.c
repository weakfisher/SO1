#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

/*
Hacer una Shell, 
la misma debe poder leer comandos (con o sin argumentos) 
ingresados por teclado y ejecutarlo. 
Una vez finalizado el programa que se ejecutó, 
debe volver al prompt y permitir el ingreso de otro programa. 
Se debe también proporcionar el comportamiento de poder salir de la shell.
*/

#define BUFFER_MAX_SIZE 256
#define MAX_ARGS 80

int main(){
  char buffer[BUFFER_MAX_SIZE];
  char *argv[MAX_ARGS];              
  char *token;    
   
  printf("Iniciando la shell...\n");

  while(1){
    printf("$");
    fgets(buffer, BUFFER_MAX_SIZE, stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
    int i = 0;
    token = strtok(buffer, " "); 
    while (token != NULL) {
      argv[i++] = token;      
      token = strtok(NULL, " ");
    }
    argv[i] = NULL;

    if (strcmp(argv[0], "exit") == 0) {
      return 0;
    } 
    
    pid_t pid = fork();
    if(strlen(buffer) == 0){
      continue;
    }
    if (pid == 0) {
        execvp(argv[0], argv); 
        perror("No se puede ejecutar el comando.");
        exit(1);  
    } else if (pid > 0) {
        wait(NULL);
    } else {
        perror("Error.\n");
        exit(1);
    }
  }
  printf("Cerrando la shell...\n");
  return 0;
}