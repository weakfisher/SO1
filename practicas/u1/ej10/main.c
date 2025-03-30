// //uego del fork, el padre env´ıa una se˜nal SIGUSR1 al hijo y entra en un loop infinito. El hijo espera en
// un loop infinito. Ambos (padre e hijo) cuando reciven una se˜nal SIGUSR1 responden lo mismo. (el padre
//     al hijo y el hijo al padre). ¿Qu´e pasa si cambiamos los loops infinitos por pause()?. Usar las funciones
//     signal() y sigaction().
#define _POSIX_C_SOURCE 199309L
#include <stdio.h> 
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
void handler(int s){
    printf("ouch!\n");
}
int main(){

__pid_t pid =fork();
if(pid==0){
    struct sigaction sa;
    sa.sa_handler = handler;

    sigaction(SIGUSR1, &sa, NULL);
    while(1){
        pause();
    }
}else{
    while(1){
        kill(pid, SIGUSR1);
        pause();
    }
}
return 0;
}