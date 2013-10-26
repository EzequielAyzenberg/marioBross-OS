
#include"Planificador.h"

void inicializar(info*);

void borrarTanda(info*);
void crearStruct(nuevo*,player*);
int leerNovedad(info*,nuevo*,player*);

void reordenar(player**,player**);
void cargarAlFinal(player*,player**,player**);

void interrupcion(int,short,answer*,global);
int selectear(answer*,short,fd_set*,int,int,global);

int cantleidos=0,RR=0;

void *planificador (void *parametro){
	info*raiz=(info*)parametro;

//Harcodeo
	nuevo*actual;
	actual=raiz->new;
	stack *recleft;
	recleft=NULL;
	player exec;
	player*readyfte,*readyfin,*sleepfte,*sleepfin;
	readyfte=NULL;
	readyfin=NULL;
	sleepfte=NULL;
	sleepfin=NULL;
	fd_set master, copy;
	int maxfd,rest=0;
	FD_ZERO(&master);
	FD_ZERO(&copy);
	FD_SET(raiz->nid,&master);
	maxfd=raiz->nid;
	global general;
	general.cabecera=raiz;
	general.rfte=&readyfte;
	general.rfin=&readyfin;
	general.sfte=&sleepfte;
	general.sfin=&sleepfin;
	general.algo=&RR;
	general.exe=&exec;
	general.recur=&recleft;
	general.resto=&rest;

	inicializar(raiz);

	player*temp;
	int estado=1;
	short respuesta;
	while (estado!=0){
		temp=(player*)malloc(sizeof(player));
		estado=leerNovedad(raiz,actual,temp);
		if(estado!=0){
			cargarAlFinal(temp,&readyfte,&readyfin);
			FD_SET(temp->pid,&master);
			if(temp->pid>maxfd)maxfd=temp->pid;
			raiz->currplay++;
			sendAnswer(7,0,' ',temp->sym,(short)raiz->nid);
			copy=master;
			respuesta=selectear(NULL,1,&copy,maxfd,raiz->nid,general);
			switch (respuesta){
			case 1:puts("--El nivel se ha enterado.--");
				break;
			case -1:puts("--ERROR: El nivel comenta que hubo un error.--");
				break;
			}
		}
	}





	return 0;
}

void inicializar(info*raiz){
	sendAnswer(6,0,' ',' ',(short)raiz->nid);
	sleep(1);
	answer tempo;
	recvAnswer(&tempo,(short)raiz->nid);
	RR=tempo.cont;
}

void borrarTanda(info*raiz){
	int cont;
	for(cont=1;cont<=5;cont++){
		nuevo*aux;
		aux=raiz->new;
		raiz->new=raiz->new->sgte;
		free(aux);
	}
}
void crearStruct(nuevo*actual,player*temp){
	temp->pid=actual->pid;
	temp->sym=actual->sym;
	temp->data.dist=0;
	temp->data.recsol=' ';
	temp->stack=NULL;
	temp->sgte=NULL;
}
int leerNovedad(info*raiz,nuevo*actual,player*temp){
	if (cantleidos==6){
		borrarTanda(raiz);
		cantleidos=0;
	}
	if (actual->pid==0)return 0;
	else{
		crearStruct(actual,temp);
	}
	return 1;
}

void reordenar(player**lfte,player**lfin){
	if (RR!=0)puts("--Planificación Round Robins => Sin Reordenar--");
	else{
		puts("--Planificación SRDF => Reordenando..--");
		//Tratar de aprender a usar el algoritmo de reordenacion de la catedra.
	}
}
void cargarAlFinal(player*temp,player**lfte,player**lfin){
	if (*lfin!=NULL)(*lfin)->sgte=temp;
	*lfin=temp;
	if (*lfte==NULL)*lfte=*lfin;
	reordenar(lfte,lfin);
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
	answer*aux;
	short respuesta;
	do{
		selectGRID(fdmax,readfds);

		int i=0;
		while ((!FD_ISSET(i,readfds))&&(i<=fdmax))	i++;
		if (i>fdmax){
			puts("--ERROR:No se encontro candidato para selectear!!--");
			exit(1);
		}else{
			if (tempo!=NULL)aux=tempo;
			respuesta=recvAnswer(aux,sock);
			if (i==sock){
				if((respuesta==esperado)||(respuesta==-1))return respuesta;
				else interrupcion(i,respuesta,aux,tabla);
			}else{
				interrupcion(i,respuesta,aux,tabla);
			}
		}
	}while(1);
	return -1;
}

