
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>        
#define BUFFER_MAX_SIZE 256
void shell(){
    char commandEntero[BUFFER_MAX_SIZE];  // comando 
    char *argv[BUFFER_MAX_SIZE];
    char *token;
    printf("$ ");
    fgets(commandEntero, BUFFER_MAX_SIZE, stdin);
    commandEntero[strlen(commandEntero) - 1] = '\0';
    int canComandos = 0;

    char * comandos[BUFFER_MAX_SIZE];
    token = strtok(commandEntero, " ");
    while (token != NULL) {
        comandos[canComandos] = token;
        canComandos++;
        token = strtok(NULL, " ");
    }
    int fd[2];
    int fdEntrada =0;

    for ( size_t i = 0; i < canComandos; i++) {
        pipe(fd);
        if (!fork()) {
            if (i < canComandos - 1) {
                dup2(fd[1], 1);
            }
            close(fd[0]);
            close(fd[1]);
            execlp(comandos[i], comandos[i], NULL);
            perror("exec");
            exit(1);
        }        
    }
    return;
}



int main() {
 
    while(1){
        if (!fork()) { // child 
            shell(); 
        }
        wait(NULL); // parent

    }

    return 0;
}