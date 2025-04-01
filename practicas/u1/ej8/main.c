#include <stdio.h>
#include <signal.h>
#include <unistd.h>
int denom = 0;
void handler(int s) { printf("ouch!\n"); denom = 1; }
int main() {
int r;
signal(SIGFPE, handler);
r = 1 / denom;
printf("r = %d\n", r);
return 0;
}
// Sucede que siempre se escribe ouch debido a que cuando el handler cambia el denom por 1 el regitro que 
//ejecuta r =1/denom sigue siendo sobre 0 por que la variable denom ya estar cargada en un registro