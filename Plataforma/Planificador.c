

#include"Planificador.h"


#define RD_INICIAL 0

void inicializar(nodoNivel*,global*);

void borrarNodo(nodoNivel*);
void crearStruct(nodoNivel*,player*,int);
int leerNovedad(nodoNivel*,global*,player*);

void reordenar(t_list*ready,int);
void cargarAlFinal(player*,t_list*,int);

void interrupcion(int,short,answer*,global);
int selectear(answer*,short,fd_set*,int,int,global);

void *planificador (void *parametro){
	puts("\nHola mundo!!--Yo planifico.");
	nodoNivel*raiz=(nodoNivel*)parametro;
	t_list *stack,*ready,*sleeps;

	stack=list_create();
	ready=list_create();
	sleeps=list_create();
	puts("--PLANIFICADOR-- 1");
	player exec;
	fd_set master, copy;
	int maxfd;
	FD_ZERO(&master);
	FD_ZERO(&copy);
	FD_SET(raiz->nid,&master);
	maxfd=raiz->nid;
	puts("--PLANIFICADOR-- 2");
	global general;
	general.cabecera=raiz;
	general.ready=ready;
	general.sleeps=sleeps;
	general.algo=0;
	general.RemainDist=0;
	general.exe=&exec;
	general.recur=stack;
	printf("Nuestro Nivel Se llama: %s\n",raiz->name);
	puts("\nEnviando Saludos al nivel..");
	inicializar(raiz,&general);
	player*temp;
	int estado=1;
	short respuesta;
	while (1){ 		//Solo por ahora lee conexiones!! (estado!=0)
		player aux;
		estado=leerNovedad(raiz,&general,&aux);
		if(estado!=0){
			puts("Cargando jugador a la base de datos..");
			temp=malloc(sizeof(player));
			*temp=aux;
			cargarAlFinal(temp,ready,general.algo);
			FD_SET(temp->pid,&master);
			if(temp->pid>maxfd)maxfd=temp->pid;
			raiz->cantJugadores++;
			puts("Avisandole al nivel..");
			sendAnswer(7,0,' ',temp->sym,(short)raiz->nid);
			copy=master;
			puts("Selecteando..");
			respuesta=selectear(NULL,1,&copy,maxfd,raiz->nid,general);
			switch (respuesta){
			case 1:puts("--El nivel se ha enterado.--");
				break;
			case -1:puts("--ERROR: El nivel comenta que hubo un error.--");
				break;
			}
		}
		sleep(2);
	}
puts("El hilo termina ahora!!");

	return 0;
}

void inicializar(nodoNivel*raiz,global*general){
	puts("\nPidiendo algoritmo.");
	sendAnswer(6,0,' ',' ',(short)raiz->nid);
	sleep(1);
	answer tempo;
	recvAnswer(&tempo,(short)raiz->nid);
	general->algo=tempo.cont;
	general->RemainDist=tempo.data-'0';
	if(general->algo==0)puts("Se ha elegido usar el Algoritmo SRDF.");
	else printf("Se ha elegido usar el Algoritmo Round Robins Q==%d\n",general->algo);
	printf("El Remaining Distance ahora es de: %d\n\n",general->RemainDist);
}


void borrarNodo(nodoNivel*raiz){
	nuevo*aux;
	aux=raiz->tandaRaiz;
	raiz->tandaRaiz=raiz->tandaRaiz->sgte;
	free(aux);
}
void crearStruct(nodoNivel*raiz,player*temp,int RD){
	temp->pid=raiz->tandaRaiz->pid;
	temp->sym=raiz->tandaRaiz->sym;
	temp->data.dist=RD;
	temp->data.recsol=' ';
	temp->stack=list_create();
}
int leerNovedad(nodoNivel*raiz,global*general,player*temp){
	if (raiz->tandaRaiz->pid==0)return 0;
	else{
		crearStruct(raiz,temp,general->RemainDist);
		puts("Se ha conectado un jugador!!");
		borrarNodo(raiz);
	}
	return 1;
}

bool comparator(void*anterior, void*actual){
	player *ant,*act;
	ant=(player*)anterior;
	act=(player*)actual;
	if(ant->data.dist>act->data.dist)return true;
	else return false;
}
void reordenar(t_list*ready,int RR){
	if (RR!=0)puts("--Planificación Round Robins => Sin Reordenar--");
	else{
		puts("--Planificación SRDF => Reordenando..--");
		list_sort(ready,comparator);
	}
}
void cargarAlFinal(player*temp,t_list*ready,int RR){
	list_add(ready, (void*)temp);
	reordenar(ready,RR);
}

void interrupcion(int i,short respuesta,answer* aux,global tabla){
	if(i==tabla.cabecera->nid){
		/*switch(respuesta){
		case 0:aLaMierdaConTodo(tabla);
		break;
		case 6:modificarAlgoritmo(aux->cont,tabla);
		break;
		case 8:muertePersonaje(aux->cont,tabla);
		break;
		}*/
	}
}
int selectear(answer*tempo,short esperado,fd_set*readfds,int fdmax,int sock,global tabla){
	answer aux;
	short respuesta;
	do{
		selectGRID(fdmax,readfds);

		int i=0;
		while ((!FD_ISSET(i,readfds))&&(i<=fdmax))	i++;
		if (i>fdmax){
			puts("--ERROR:No se encontro candidato para selectear!!--");
			exit(1);
		}else{
			printf("Se escuchara al socket numero %d\n",i);
			respuesta=recvAnswer(&aux,i);
			if (i==sock){
				if((respuesta==esperado)||(respuesta==-1))return respuesta;
				else interrupcion(i,respuesta,&aux,tabla);
			}else{
				interrupcion(i,respuesta,&aux,tabla);
			}
			if (tempo!=NULL)*tempo=aux;
		}
	}while(1);
	return -1;
}


