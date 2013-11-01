#include"sockets.h"

void terminar(int estado,int sockfd){

	//Funciones internas para terminar el programa por error.

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

int connectGRID(int port,char *ipdest){

	//Le pasas el puerto y la IP(en string) y el chabon te devuelve el socket ya conectado.

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
	puts("\n--AUX--Estableciendo cenexion con el servidor..");
	estado=connect(sockfd,(struct sockaddr *)&socket_addr,sizeof(struct sockaddr));
	if (estado==-1)terminar(3,sockfd);
	puts("--AUX--Conexion realizada con exito!!\n");
	return sockfd;
}

int listenGRID(int port){

	//Le pasas el puerto y el chabon te devuelve el socket escuchando.

	int sock,estado;
	struct sockaddr_in socket_addr;
	sock=socket(AF_INET,SOCK_STREAM,0);
	if (sock==-1) terminar(2,sock);
	socket_addr.sin_family =AF_INET;
	socket_addr.sin_port=htons(port);
	socket_addr.sin_addr.s_addr=htonl(INADDR_ANY);
	memset(&(socket_addr.sin_zero),'\0',8);
	//Los mensajes probablemente se reemplacen con salidas al log.
	int yes=1;
	estado=setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &yes,sizeof(int));
	if (estado==-1)terminar(4,sock);
	estado=bind(sock, (struct sockaddr *)&socket_addr, sizeof(struct sockaddr));
	if (estado==-1)terminar(4,sock);
	puts("--AUX--Puerto asignado correctamente.");
	estado=listen(sock,10);
	if (estado==-1)terminar(5,sock);
	puts("--AUX--Escuchando...");
	return sock;
}

int selectGRID(int maxfd,fd_set *dirfdRead){

	//Le pasas el maxfd y la lista copia de fd_set y te devuelve lo que el select, sin errores.

	int estado=0,aux=0;
	//struct timeval time;
	while (estado<=0){
		if(aux==5)terminar(1,0);
		estado=select (maxfd+1,dirfdRead,NULL,NULL,NULL);
		aux++;
	}
return estado;
}

int acceptGRID(int sockescucha){

	//Simple...Le pasas el Listener y te devuelve un nuevo socket conectado.

	struct sockaddr_in their_addr;
	unsigned int size=sizeof(struct sockaddr_in);
	//Los mensajes probablemente se reemplacen con salidas al log.
	int nuevo=accept(sockescucha,(struct sockaddr *)&their_addr,&size);
	if (nuevo==-1)terminar(3,nuevo);
	char direccion[15];
	int dir,addrlen;
	addrlen=sizeof(struct sockaddr);
	dir=getpeername(nuevo,(struct sockaddr *)&their_addr,(unsigned int *)&addrlen);
	if (dir==-1)terminar(3,nuevo);
	strcpy(direccion,inet_ntoa(their_addr.sin_addr));
	printf("--AUX--Conexion establecida con IP Nº: %s en el socket Nº: %d\n\n",direccion,nuevo);
	return nuevo;
}

void sendHandshake(short tipo,char* nombre,char simbolo,short sockfd){

	//Le pasas los datos del Handshake más el socket destino en formato short (castearlo).
	//Nombre de un maximo de 12 Caracteres.

	handshake temp;
	temp.type=tipo;
	temp.symbol=simbolo;
	strcpy(temp.name,nombre);
	short estado=0;
	while (estado<sizeof(handshake)){
		//Los mensajes probablemente se reemplacen con salidas al log.
		printf("\n--AUX--Intentando enviar mensaje...");
		estado=send(sockfd,(void*)&temp,sizeof(handshake),0);
		sleep(0.1);
	}
	printf("Mensaje enviado con exito!!\n\n");
}

void sendAnswer(short mensaje,short contador, char datos, char simbolo, short sockfd){

	//Lo mismo que el anterior pero con Answer.

	answer temp;
	temp.msg=mensaje;
	temp.cont=contador;
	temp.data=datos;
	temp.symbol=simbolo;
	int estado=0;
	while (estado<sizeof(answer)){
		//Los mensajes probablemente se reemplacen con salidas al log.
		printf("\n--AUX--Intentando enviar mensaje...");
		estado=send(sockfd,(void*)&temp,sizeof(answer),0);
		sleep(0.1);
	}
	printf("Mensaje enviado con exito!!\n\n");
}

int recvHandshake(handshake *temp,int sockfd){

	/*Le pasas una estructura Handshake vacia (por referencia) más el socket, y te da
	el estructura->Type como resultado o 0 si se desconecto.*/

	int aux=1,estado=-1;
	while(estado<0){		//Itera hasta que se reciba algo potable (incluida desconexion) o 5 veces, que se termina todo.
		//Los mensajes probablemente se reemplacen con salidas al log.
		puts("--AUX--Recibiendo mensaje..");
		estado=recv(sockfd,(handshake*)temp,sizeof(handshake),0);
		aux++;
		if(aux==5)terminar(1,sockfd);
	}
	printf("--AUX-------El estado es: %d-------\n",estado);
	if(estado==0)return -1; 	//Si el Cliente se desconecta antes de enviar el primer handshake, devuelve -1.
	puts("--AUX--Mensaje recibido satisfactoriamente!!\n");
	return (int)temp->type;
}

int recvAnswer(answer *temp,int sockfd){

	/*Lo mismo que el anterior pero con una estructura Answer.
	Te devuelve el estructura->Msg como resultado o 0 si se desconecto.*/

	int aux=0,estado=-1;
	while(estado<0){		//Itera hasta que se reciba algo potable (incluida desconexion) o 5 veces, que se termina todo.
		//Los mensajes probablemente se reemplacen con salidas al log.
		puts("--AUX--Recibiendo mensaje..");
		estado=recv(sockfd,(answer*)temp,sizeof(*temp),0);
		aux++;
		if(aux==5)terminar(1,sockfd);
	}printf("--AUX-------El estado es: %d-------\n",estado);
	if(estado==0)return 0;	//Si se detecta una desconexion entonces devuelve 0 como habitualmente hace el recv.
	puts("--AUX--Mensaje recibido satisfactoriamente!!\n");
	return (int)temp->msg;
}
