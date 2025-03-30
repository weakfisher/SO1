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
//Sucede que el programa siempre imprime ouch ya que la variable denom se inicializa en 0 y en la función handler se le asigna el valor de 1,  por lo que  se produce una excepción de división por cero.
// luego como ya se esta evaluando no va a parar de imprimir ouch pues el handler no para la ejecucion pues luego el comportamiendo es indefinido.