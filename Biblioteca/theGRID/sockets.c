#include"sockets.h"

void terminar(int estado,int sockfd){
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
int ch;
while( (ch = fgetc(stdin)) != EOF && ch != '\n' && ch!='\0' ){}
}

int connectGRID(int port,char *ipdest){
	int estado;
	int sockfd;
	struct sockaddr_in socket_addr;
	sockfd=socket(AF_INET,SOCK_STREAM,0);
		if (sockfd==-1)	terminar(2,sockfd);
	socket_addr.sin_family=AF_INET;
	socket_addr.sin_port=htons(port);
	socket_addr.sin_addr.s_addr=inet_addr(ipdest);
	memset(&(socket_addr.sin_zero),'\0',8);
	//Los mensajes probablemente se reemplacen con salidas al log.
	puts("\nEstableciendo cenexion con el servidor..");
	estado=connect(sockfd,(struct sockaddr *)&socket_addr,sizeof(struct sockaddr));
	if (estado==-1)terminar(3,sockfd);
	puts("Conexion realizada con exito!!");
	return sockfd;
}

int listenGRID(int port){
	int sock,estado;
	struct sockaddr_in socket_addr;
	sock=socket(AF_INET,SOCK_STREAM,0);
	if (sock==-1) terminar(2,sock);
	socket_addr.sin_family =AF_INET;
	socket_addr.sin_port=htons(port);
	socket_addr.sin_addr.s_addr=htonl(INADDR_ANY);
	memset(&(socket_addr.sin_zero),'\0',8);
	//Los mensajes probablemente se reemplacen con salidas al log.
	estado=bind(sock, (struct sockaddr *)&socket_addr, sizeof(struct sockaddr));
	if (estado==-1)terminar(4,sock);
	puts("Puerto asiganado correctamente.");
	estado=listen(sock,10);
	if (estado==-1)terminar(5,sock);
	puts("Escuchando...");
	return sock;
}

int selectGRID(int maxfd,fd_set *dirfdRead){
	int estado=0;
	struct timeval time;
	time.tv_sec=10;
	time.tv_usec=0;
	while (estado<=0)	estado=select (maxfd,dirfdRead,NULL,NULL, &time);
return estado;
}

int acceptGRID(int sockescucha){
	struct sockaddr_in their_addr;
	unsigned int size=sizeof(struct sockaddr_in);
	//Los mensajes probablemente se reemplacen con salidas al log.
	int nuevo=accept(sockescucha,(struct sockaddr *)&their_addr,&size);
	if (nuevo==-1)terminar(3,nuevo);
	char direccion[15];
	strcpy(direccion,inet_ntoa(their_addr.sin_addr));
	printf("Conexion establecida con IP Nº: %s\n",direccion);
	return nuevo;
}

void sendHandshake(char tipo,char* nombre,char simbolo,short sockfd){
	handshake temp;
	temp.type=tipo;
	temp.symbol=simbolo;
	strcpy(temp.name,nombre);
	short estado=0;
	while (estado<sizeof(handshake)){
		//Los mensajes probablemente se reemplacen con salidas al log.
		puts("Intentando enviar mensaje..");
		estado=(short)send(sockfd,(void*)&temp,sizeof(handshake),0);
		sleep(0.1);
	}
	puts("Mensaje enviado con exito!!");
}

void sendAnswer(char mensaje,short contador, char datos, char simbolo, short sockfd){
	answer temp;
	temp.msg=mensaje;
	temp.cont=contador;
	temp.data=datos;
	temp.symbol=simbolo;
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
		estado=(short)recv(sockfd,(void*)temp,sizeof(handshake),0);
	}
	(handshake*)temp;
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
