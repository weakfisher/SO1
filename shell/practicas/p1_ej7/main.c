#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>

#include <fcntl.h>

#define BUFFER_MAX_SIZE 256

int mini_shell() {
    char comandoEntero[BUFFER_MAX_SIZE];
    char *argv[BUFFER_MAX_SIZE];
    char * token;


    printf("$ ");
    fgets(comandoEntero, BUFFER_MAX_SIZE, stdin);
    comandoEntero[strcspn(comandoEntero, "\n")] = '\0';
    if (strcmp(comandoEntero, "exit") == 0) {
        return 1;
    }
    // Ejercicio d
    char * comandos[BUFFER_MAX_SIZE];
    int cantComandos = 0;
    token = strtok(comandoEntero, "|");
    while (token) {
        comandos[cantComandos++] = token;
        token = strtok(NULL, "|");
    }
    int fd[2];
    int fdEntrada = 0;

    for(size_t d = 0; d < cantComandos; d++) {
        pipe(fd);
        if(!fork()) { // Child
            close(fd[0]);

            dup2(fdEntrada, 0); // recibe lo del descriptor
            if(d != cantComandos - 1) {
                dup2(fd[1], 1);
            }

            // Ejercicio c
            char * file;
            if(file = strstr(comandos[d], ">")) {
                file += 2; // "limpia" '> '
                close(1); // Cierra stdout
                open(file, O_CREAT | O_WRONLY | O_TRUNC, 0644);
            }  
            token = strtok(comandos[d], ">");
            
            // Ejercicio b
            int cantStrings = 0; // Cantidad de strings del comando
            token = strtok(comandos[d], " ");
            while (token) {
                argv[cantStrings] = token;
                cantStrings++;
                token = strtok(NULL, " ");
            }
            argv[cantStrings] = NULL;
            
            execvp(argv[0], argv);
        }
        else { // Parent
            close(fd[1]);
            wait(NULL);

            fdEntrada = fd[0];
        }
    }
    return 0 ;
}

int main() {
    while (1) {
        if (!fork()){ // Child
            int exit_code = mini_shell();
            exit(exit_code);
        }
        else {// Parent
            int status;
            wait(&status);
            if(WIFEXITED(status) && WEXITSTATUS(status) == 1) {
                break;
            }}
    }
    return 0;

}



