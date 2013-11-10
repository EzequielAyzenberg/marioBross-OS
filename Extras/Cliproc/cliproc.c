/*ESTE CLIENTE PROBARÁ ENVIAR INFORMACION EN LA ESTRUCTURA OFICIAL,
 IMPLEMENTANDO LA BIBLIOTECA DE SOCKETS*/

#include<theGRID/sockets.h>
#include<theGRID/general.h>


#define PORT 2600
#define IPDEST "127.0.0.1"
#define MAX 8

void recibidor(int sock);

int main(int argc, char *argv[]){
						//Carga la IP de destino y el puerto puestos a la hora de ejecutar el programa.
	int puerto;
	char ipdestino[15];
	switch (argc){
	case 3:
		puerto=atoi(argv[1]);
		strcpy(ipdestino,argv[2]);
		printf("Se usara el siguiente puerto:\t%d\n", puerto);
		printf("Se usara la siguiente direccion: %s", argv[2]);
		break;
	case 2:
		puerto=atoi(argv[1]);
		printf("Se usara el siguiente puerto:\t%d\n", puerto);
		strcpy(ipdestino,IPDEST);
		puts("Se usara la IP de destino estandar.");
		break;
	default:
		puerto=PORT;
		strcpy(ipdestino,IPDEST);
		puts("Se usaran el puerto y la IP de destino estandar.");
		break;
	}
	short sockfd= connectGRID(puerto,ipdestino);				//Cre al nuevo socket y establece una nueva conexion.
		printf("\nWelcome to the GRID!!\n\n");
		puts("Primero el Handshake!!");
		printf("Ingrese los datos a continuacion:\n");
		int tipo;
		char symbol,nombre[13];				//Se ingresa toda la informacion del Answer, borrando el buffer entre cada ingreso.
		printf("Tipo: ");
		scanf("%d",&tipo);
		flush_in();
		printf("Nombre: ");
		scanf("%s",nombre);
		flush_in();
		printf("Simbolo: ");
		scanf("%c",&symbol);
		sendHandshake((short)tipo,nombre,symbol,(short)sockfd);	//Se envia la informacion completa unicamente.
		char caracter;
		printf("¿Desea esperar la respuesta? s/n: ");
		flush_in();
		scanf("%c",&caracter);
		if (caracter!='s') printf("Sin respuesta será..\n");
		else recibidor(sockfd);
		puts("Ahora si con los tipo Answer...");
		do{
			printf("Desea recibir(r), enviar(s) o salir(e)? ");
			flush_in();
			scanf("%c",&caracter);
			switch (caracter){
				case 'r':
					recibidor(sockfd);
					break;
				case 's':
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
					sendAnswer((short)msg,(short)cont,data,sym,(short)sockfd);	//Se envia la informacion completa unicamente.
					break;
				case 'e':
					exit(0);
					break;
			}
		}while(1);
	return 0;
}

void recibidor(int sock){
	answer temp;
	recvAnswer(&temp,sock);
	puts("\nLa estructura recibida fue:\n");			//Se recibe el mensaje.
	printf("==>Tipo: %d\n",temp.msg);
	printf("==>Contador: %d\n",temp.cont);
	printf("==>Dato: %c\n",temp.data);
	printf("==>Simbolo: %c\n\n",temp.symbol);
}

