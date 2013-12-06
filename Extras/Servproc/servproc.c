/*ESTE SERVIDOR PROBARÁ RECIBIR INFORMACION EN LA ESTRUCTURA OFICIAL,
 IMPLEMENTANDO LA BIBLIOTECA DE SOCKETS*/

#include<stdio.h>
#include<theGRID/sockets.h>
#include<pthread.h>


#define MYPORT 2600
#define MYIP "127.0.0.1"
#define MAX 64

void recibirNuevo(int);
void respuesta(int);
void atenderDante(int);
void moverDante(int);

int main(int argc, char *argv[]){
					//Carga la IP de destino y el puerto puestos a la hora de ejecutar el programa.
	int puerto,i;
	switch (argc){
	case 2:
		puerto=atoi(argv[1]);
		printf("Se usara el siguiente puerto:\t%d\n", puerto);
		break;
	default:
		puerto=MYPORT;
		puts("Se usara el puerto estandar.");
		break;
	}
	char caracter;
	printf("\n¿Desea Encender el Servidor? s/n: ");
	scanf("%c",&caracter);
	if (caracter=='n') terminar(0,0);
	if (caracter!='s') terminar(1,0);
					//Aca arranca el servidor..
	fd_set master, readfds;			//Se crean 2 porque el select sobreescribe la que le envias.
	int listener,fdmax,newfd;
	FD_ZERO(&master);
	FD_ZERO(&readfds);				//Se setean a 0 ambas estructuras.
	puts("Antes del Listen");
	listener=listenGRID(puerto,MYIP);		//Se deja el Listener escucha
	puts("Despues del Listen");
	FD_SET(listener,&master);			//Se agrega al Listener a la lista maestra.
	fdmax = listener;
	while(1){
		readfds=master;					//Siempre antes del Select se copia la lista maestra a la que se le pasa al Select.
		puts("\nEntrando en el select..");
		selectGRID(fdmax,&readfds);
		puts("Procesando informacion..");
		sleep(1);
		for(i = 0; i <= fdmax; i++) {
			if (FD_ISSET(i, &readfds)) {								//Pregunta por todos, aun cuando ya lo haya encontrado.
				if (i == listener) {					//Si el Listener escucho algo, se gestiona una nueva conexion.
					newfd=acceptGRID(listener);
					FD_SET(newfd, &master);								//Se agrega a cada nueva conexion a la lista maestra.
					if (newfd > fdmax) fdmax = newfd;					//Se modifica el maximo descriptor de ser necesario.
					recibirNuevo(newfd);
					printf("¿Desea responderle algo o Conectar Dante? s/n/d: ");
					flush_in();
					scanf("%c",&caracter);
					if (caracter!='s'&&caracter!='d') printf("Sin respuesta será..\n");
					else if(caracter=='s')respuesta(newfd);
					else sendAnswer(6,0,' ',' ',newfd);

				}else{									//Si no fue el Listener, se recibio mensaje de un cliente...
					answer temp;
					int tipo,flag;
					tipo=recvAnswer(&temp,i);
					if (tipo == 0){							//El cliente cerro la conexion.
						printf("El socket Nº: %d Ha cerrado conexión.\n", i);
						close(i);
						FD_CLR(i, &master);						//Se lo remueve de la lista maestra.
					}else{
						puts("\nLa estructura recibida fue:");			//Se recibe el mensaje.
						printf("==>Tipo: %d\n",temp.msg);
						printf("==>Contador: %d\n",temp.cont);
						printf("==>Dato: %c\n",temp.data);
						printf("==>Simbolo: %c\n\n",temp.symbol);
						flag=1;
						do{
						printf("Desea recibir(r),enviar(s),nuevo(n),conectarDante(c),movDante(m) o salir(e)? ");
						flush_in();
						scanf("%c",&caracter);

						switch (caracter){
						case 'r':
							tipo=recvAnswer(&temp,i);
							if (tipo == 0) {							//El cliente cerro la conexion.
								printf("El socket Nº: %d Ha cerrado conexión.\n", i);
								close(i);
								FD_CLR(i, &master);						//Se lo remueve de la lista maestra.
							} else {
								puts("\nLa estructura recibida fue:");			//Se recibe el mensaje.
								printf("==>Tipo: %d\n",temp.msg);
								printf("==>Contador: %d\n",temp.cont);
								printf("==>Dato: %c\n",temp.data);
								printf("==>Simbolo: %c\n\n",temp.symbol);
							}break;
						case 's':
							respuesta(i);
							break;
						case 'n':
							flag=0;
							break;
						case 'e':
							exit(0);
							break;
						case 'm':
							moverDante(i);
							break;
						case 'c':
							atenderDante(i);
							break;
						}
						}while(flag);
					}
				}
			}
		}
	}
	return 0;
}

void recibirNuevo(int sock){
	handshake temp;
	recvHandshake(&temp,sock);
	puts("\nLa estructura recibida fue:\n");			//Se recibe el mensaje.
	printf("==>Tipo: %d\n",temp.type);
	printf("==>Nombre: %s\n",temp.name);
	printf("==>Simbolo: %c\n\n",temp.symbol);
}
void respuesta(int sock){
printf("Ingrese los datos a continuacion:\n");
		int cont,msg;
		char data,sym;				//Se ingresa toda la informacion del Answer, borrando el buffer entre cada ingreso.
		flush_in();
		printf("Mensaje: ");
		scanf("%d",&msg);
		flush_in();
		printf("Contador: ");
		scanf("%d",&cont);
		flush_in();
		printf("Dato: ");
		scanf("%c",&data);
		flush_in();
		printf("Simbolo: ");
		scanf("%c",&sym);
		sendAnswer((short)msg,(short)cont,data,sym,(short)sock);	//Se envia la informacion completa unicamente.
}

void atenderDante(int sock){
	answer temp;
	sendAnswer(7,0,' ','A',sock);
	recvAnswer(&temp,sock);
	sendAnswer(7,0,' ','B',sock);
	recvAnswer(&temp,sock);
	sendAnswer(7,0,' ','C',sock);
	recvAnswer(&temp,sock);
	sendAnswer(7,0,' ','D',sock);
	recvAnswer(&temp,sock);
	sendAnswer(7,0,' ','E',sock);
	recvAnswer(&temp,sock);
}

void moverDante(int sock){
	int i,a=0,b=1,c=1, bandera=0;
	answer temp;
	//sendAnswer(3,101,' ','A',sock);
	sendAnswer(3,101,' ','B',sock);
	recvAnswer(&temp,sock);
	sendAnswer(3,101,' ','C',sock);
	recvAnswer(&temp,sock);
	sendAnswer(3,101,' ','D',sock);
	recvAnswer(&temp,sock);
	sendAnswer(3,101,' ','E',sock);
	recvAnswer(&temp,sock);
	for(i=0;i<40;i++){

		if(bandera==0){
			sendAnswer(3,((b+1)*100)+b,' ','B',sock);
			recvAnswer(&temp,sock);
			sendAnswer(3,((c*3-2)*100)+c,' ','C',sock);
			recvAnswer(&temp,sock);
			sendAnswer(3,((c*3-1)*100)+c,' ','C',sock);
			recvAnswer(&temp,sock);
			sendAnswer(3,((c*3)*100)+c,' ','C',sock);
			recvAnswer(&temp,sock);
			bandera=1;
		}else if(bandera==1){
			sendAnswer(3,(b*100)+b,' ','B',sock);
			recvAnswer(&temp,sock);
			usleep(250000);
			sendAnswer(3,((a*2-1)*100)+a,' ','A',sock);
			recvAnswer(&temp,sock);
			usleep(50000);
			sendAnswer(3,(c*100)+c,' ','C',sock);
			recvAnswer(&temp,sock);
			sendAnswer(3,((a*2)*100)+a,' ','A',sock);
			recvAnswer(&temp,sock);
			bandera=0;
			a++;
			b++;
			c++;
		}
	usleep(250000);
	}
}
