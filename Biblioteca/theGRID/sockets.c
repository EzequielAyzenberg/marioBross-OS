#include<arpa/inet.h>
#include<errno.h>
#include<netdb.h>
#include<netinet/in.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<unistd.h>

typedef struct __attribute__((packed)){
	char type;
	char name[8];
	char symbol;
}handshake;

typedef struct {
	char msg;
	short cont;
	char data;
	char symbol;
}answer;

void terminar(short estado, short sockfd){
	switch (estado){
	case 0:
		puts("Ha elegido terminar el programa, adios.");
		exit(0);
	case 1:
		puts("Se ha terminado el programa por un error.");
		close(sockfd);
		exit(1);
	case 2:
		perror("Hubo un error al llamar un socket.");
		exit(1);
	case 3:
		perror("Hubo un error al conectar.");
		exit(1);
	case 4:
		perror("Hubo un error al bindear.");
		exit(1);
	case 5:
		perror("Hubo un error al escuchar.");
		exit(1);
	}
}

void flush_in(void){
short ch;
while( (ch = fgetc(stdin)) != EOF && ch != '\n' ){}
}

short connectGRID(short port,char *ipdest){
	short sockfd,estado;
	struct sockaddr_in socket_addr;
	sockfd=socket(AF_INET,SOCK_STREAM,0);
		if (sockfd==-1)	terminar(2,(short)sockfd);
	socket_addr.sin_family=AF_INET;
	socket_addr.sin_port=htons(port);
	socket_addr.sin_addr.s_addr=inet_addr(ipdest);
	memset(&(socket_addr.sin_zero),'\0',8);
	//Los mensajes probablemente se reemplacen con salidas al log.
	puts("Estableciendo cenexion con el servidor..");
	estado=connect(sockfd,(struct sockaddr *)&socket_addr,sizeof(struct sockaddr));
	if (estado==-1)terminar(3,(short)sockfd);
	puts("Conexion realizada con exito!!");
	return sockfd;
}

short listenGRID(short port,char *ipdest){
	short sockfd,estado;
	struct sockaddr_in socket_addr;
	sockfd=socket(AF_INET,SOCK_STREAM,0);
	if (sockfd==-1) terminar(2,(short)sockfd);
	socket_addr.sin_family =AF_INET;
	socket_addr.sin_port=htons(port);
	socket_addr.sin_addr.s_addr=inet_addr(ipdest);
	memset(&(socket_addr.sin_zero),'\0',8);
	//Los mensajes probablemente se reemplacen con salidas al log.
	estado=bind(sockfd, (struct sockaddr *)&socket_addr, sizeof(struct sockaddr));
	if (estado==-1)terminar(4,(short)sockfd);
	puts("Puerto asiganado correctamente.");
	estado=listen(sockfd,10);
	if (estado==-1)terminar(5,(short)sockfd);
	puts("Escuchando...");
	return sockfd;
}

short acceptGRID(short sockescucha){
	struct sockaddr_in their_addr;
	unsigned int size=sizeof(struct sockaddr_in);
	//Los mensajes probablemente se reemplacen con salidas al log.
	short socknuevo=(short) accept(sockescucha,(struct sockaddr *)&their_addr,&size);
	if (socknuevo==-1)terminar(3,(short)socknuevo);
	puts("Conexion establecida!!");
	return socknuevo;
}

void sendHandshake(char tipo,char* nombre,char simbolo,short sockfd){
	handshake *temp=(handshake*)malloc(sizeof(handshake));
	temp->type=tipo;
	temp->symbol=simbolo;
	strcpy(temp->name,nombre);
	short estado=0;
	while (estado<sizeof(handshake)){
		//Los mensajes probablemente se reemplacen con salidas al log.
		puts("Intentando enviar mensaje..");
		estado=(short)send(sockfd,&temp,sizeof(handshake),0);
		sleep(0.1);
	}
	puts("Mensaje enviado con exito!!");
}

void sendAnswer(char mensaje,short contador, char datos, char simbolo, short sockfd){
	answer *temp=(answer*)malloc(sizeof(answer));
	temp->msg=mensaje;
	temp->cont=contador;
	temp->data=datos;
	temp->symbol=simbolo;
	short estado=0;
	while (estado<sizeof(answer)){
		//Los mensajes probablemente se reemplacen con salidas al log.
		puts("Intentando enviar mensaje..");
		estado=(short)send(sockfd,&temp,sizeof(answer),0);
		sleep(0.1);
	}
	puts("Mensaje enviado con exito!!");
}

char recvHandshake(handshake *temp,short sockfd){
	short estado=0;
	while(estado<sizeof(handshake)){
		//Los mensajes probablemente se reemplacen con salidas al log.
		puts("Recibiendo mensaje..");
		estado=(short)recv(sockfd,temp,sizeof(handshake),0);
	}
	puts("Mensaje recibido satisfactoriamente!!");
	return temp->type;
}

char recvAnswer(answer *temp,short sockfd){
	short estado=0;
	while(estado<sizeof(answer)){
		//Los mensajes probablemente se reemplacen con salidas al log.
		puts("Recibiendo mensaje..");
		estado=(short)recv(sockfd,temp,sizeof(answer),0);
	}
	puts("Mensaje recibido satisfactoriamente!!");
	return temp->msg;
}
