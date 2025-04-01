#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#ifndef MAP_ANONYMOUS
#define MAP_ANONYMOUS 0x20
#endif
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>

#include <sys/mman.h>
#include <asm-generic/fcntl.h>
#include <bits/mman-linux.h>
/*
 * Para probar, usar netcat. Ej:
 *
 *      $ nc localhost 4040
 *      NUEVO
 *      0
 *      NUEVO
 *      1
 *      CHAU
 */

void quit(char *s)
{
	perror(s);
	abort();
}

int U = 0;

int fd_readline(int fd, char *buf)
{
	int rc;
	int i = 0;

	/*
	 * Leemos de a un caracter (no muy eficiente...) hasta
	 * completar una línea.
	 */
	while ((rc = read(fd, buf + i, 1)) > 0) {
		if (buf[i] == '\n')
			break;
		i++;
	}

	if (rc < 0)
		return rc;

	buf[i] = 0;
	return i;
}

void handle_conn(int csock,int * smemm)
{
	char buf[200];
	int rc;
	while (1) {
		/* Atendemos pedidos, uno por linea */
		rc = fd_readline(csock, buf);
		if (rc < 0)
			quit("read... raro");

		if (rc == 0) {
			/* linea vacia, se cerró la conexión */
			close(csock);
			return;
		}

		if (!strcmp(buf, "NUEVO")) {
			char reply[20];
			sprintf(reply, "%d\n", *smemm);
			(*smemm)++;
			write(csock, reply, strlen(reply));
		} else if (!strcmp(buf, "CHAU")) {
			close(csock);
			return;
		}
		
	}
}

void wait_for_clients(int lsock,int * smemm)
{
	int csock;	

/* Esperamos una conexión, no nos interesa de donde viene */
	while (1)
	{
		/* code */
		
		
		csock = accept(lsock, NULL, NULL);
		if (csock < 0)
		quit("accept");
		pid_t pid=fork();
		
		if(pid==0){
			/* Atendemos al cliente */
			close(lsock);
			handle_conn(csock,smemm);
			
			exit(0);
		}
		else{
			/* Volvemos a esperar conexiones */
			close(csock);
		}
	}
}

/* Crea un socket de escucha en puerto 4040 TCP */
int mk_lsock()
{
	struct sockaddr_in sa;
	int lsock;
	int rc;
	int yes = 1;

	/* Crear socket */
	lsock = socket(AF_INET, SOCK_STREAM, 0);
	if (lsock < 0)
		quit("socket");

	/* Setear opción reuseaddr... normalmente no es necesario */
	if (setsockopt(lsock, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof yes) == 1)
		quit("setsockopt");

	sa.sin_family = AF_INET;
	sa.sin_port = htons(4040);
	sa.sin_addr.s_addr = htonl(INADDR_ANY);

	/* Bindear al puerto 4040 TCP, en todas las direcciones disponibles */
	rc = bind(lsock, (struct sockaddr *)&sa, sizeof sa);
	if (rc < 0)
		quit("bind");

	/* Setear en modo escucha */
	rc = listen(lsock, 10);
	if (rc < 0)
		quit("listen");

	return lsock;
}

int main()
{
	int lsock;
	int * smemm = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED |MAP_ANONYMOUS, -1, 0);
	lsock = mk_lsock();
	wait_for_clients(lsock,smemm);
	munmap(smemm,sizeof(int));
}

