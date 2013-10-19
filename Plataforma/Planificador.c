#include<stdio.h>
#include<theGRID/sockets.h>

typedef struct tnuevo{
	int pid;
	char sym;
	struct tnuevo* sgte;
}nuevo;

typedef struct tinfo{
	int nid;
	char name[17];
	int currplay;
	nuevo* new;
	struct tinfo* sgte;
}info;

struct data{
	int dist;
	char recsol;
};

struct stack{
	char recasig;
	struct stack* sgte;
};

typedef struct tplayer{
	int pid;
	char sym;
	struct data data;
	struct stack* stack;
	struct tplayer* sgte;
}player;

void inicializar(info*);

void borrarTanda(info*);
void crearStruct(nuevo*,player*);
int leerNovedad(info*,nuevo*,player*);

void reordenar(player**,player**);
void cargarAlFinal(player*,player**,player**);

int cantleidos=0,RR=0;

int main(){

//Harcodeo
	info*raiz;
	nuevo*actual;
	player*readyfte,*readyfin,*sleepfte,*sleepfin;
	readyfte=NULL;
	readyfin=NULL;
	sleepfte=NULL;
	sleepfin=NULL;
	raiz=NULL;
	actual=NULL;

	inicializar(raiz);

	player*temp;
	int estado=1;
	while (estado!=0){
		temp=(player*)malloc(sizeof(player));
		estado=leerNovedad(raiz,actual,temp);
		cargarAlFinal(temp,&readyfte,&readyfin);

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
		info*aux;
		aux=raiz;
		raiz=raiz->sgte;
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



