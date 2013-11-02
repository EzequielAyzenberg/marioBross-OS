

#include"Planificador.h"


#define RD_INICIAL 0

void _each_Personaje(void*);

void modificarAlgoritmo(answer,global);

void inicializar(nodoNivel*,global*);

void borrarNodo(nodoNivel*);
void crearStruct(nodoNivel*,player*,int);
int leerNovedad(nodoNivel*,global*,player*);

void reordenar(t_list*ready,int);
void cargarAlFinal(player*,t_list*,int);

void aLaMierdaConTodo(global);
void muertePersonaje(int, global);
void matarPersonaje(char,global);
void interrupcion(int,short,answer*,global);
int selectear(answer*,short,fd_set*,int,int,global);

void darInstancia(player*,stack*,global*);
void asignarRecursos(global*);

void devolverRecursos(global*);


void *planificador (void *parametro){
	puts("\nHola mundo!!--Yo planifico.");
	nodoNivel*raiz=(nodoNivel*)parametro;
	t_list *stack,*ready,*sleeps;

	stack=list_create();
	ready=list_create();
	sleeps=list_create();
	player exec;
	fd_set master;
	int maxfd;
	FD_ZERO(&master);
	FD_SET(raiz->nid,&master);
	maxfd=raiz->nid;
	global general;
		general.algo=(struct algo*)malloc(sizeof(struct algo));
		general.cabecera=raiz;
		general.ready=ready;
		general.sleeps=sleeps;
		general.algo->RemainDist=0;
		general.algo->algo=0;
		general.exe=&exec;
		general.recur=stack;
		general.original=&master;
		general.maxfd=&maxfd;
	printf("Nuestro Nivel Se llama: %s\n",raiz->name);
	puts("\nEnviando Saludos al nivel..");
	inicializar(raiz,&general);
	player*temp;
	int estado=1;
	short respuesta;
	player aux;
	while (1){ 		//Solo por ahora lee conexiones!! (estado!=0)
		estado=leerNovedad(raiz,&general,&aux);
		if(estado!=0){
			puts("Avisandole al nivel..");
			sendAnswer(7,0,' ',aux.sym,(short)raiz->nid);
			respuesta=selectear(NULL,1,&master,maxfd,raiz->nid,general);
			switch (respuesta){
				case 1:puts("--El nivel ha dado el ok.--");
				puts("Cargando jugador a la base de datos..");
				temp=malloc(sizeof(player));
				*temp=aux;
				cargarAlFinal(temp,ready,general.algo->algo);
				FD_SET(temp->pid,&master);
				if(temp->pid>maxfd)maxfd=temp->pid;
				raiz->cantJugadores++;
				sendAnswer(1,0,' ',' ',temp->pid);
				puts("\nLa lista hasta ahora a quedado asi:");
				list_iterate(ready,_each_Personaje);
				puts("");
				break;
				case -1:puts("--ERROR: El nivel comenta que hubo un error.--");
				sendAnswer(-1,0,' ',' ',aux.pid);
				break;
			}
		}
		asignarRecursos(&general);
		devolverRecursos(&general);
	}
puts("El hilo termina ahora!!");

	return 0;
}

void _each_Personaje(void*jug){
	player* jugador;
	jugador=(player*)jug;
	static int contador;
	printf("El jugador Nº %d ",contador);
	printf("es: %c\n",jugador->sym);
	contador++;
}

void modificarAlgoritmo(answer temp,global general){
	general.algo->algo=temp.cont;
	general.algo->RemainDist=temp.data -'0';
	if(general.algo->algo==0)puts("Se ha elegido usar el Algoritmo SRDF.");
	else printf("Se ha elegido usar el Algoritmo Round Robins Q==%d\n",general.algo->algo);
	printf("El Remaining Distance ahora es de: %d\n\n",general.algo->RemainDist);
}

void inicializar(nodoNivel*raiz,global*general){
	int estado=6;
	answer tempo;
	do{
	puts("\nPidiendo algoritmo.");
	sendAnswer(6,0,' ',' ',(short)raiz->nid);
	sleep(1);
	estado=recvAnswer(&tempo,(short)raiz->nid);
	if(estado!=6) puts("El nivel flasheo cualquiera!");
	}while(estado!=6);
	modificarAlgoritmo(tempo,*general);
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
		crearStruct(raiz,temp,general->algo->RemainDist);
		puts("Se ha conectado un jugador!!");
		borrarNodo(raiz);
	}
	return 1;
}

bool comparator(void*anterior, void*actual){
	player *ant,*act;
	ant=(player*)anterior;
	act=(player*)actual;
	if(ant->sym<act->sym)return true;
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

void aLaMierdaConTodo(global tabla){
	puts("Se cayo el nivel, procesando..");
	usleep(500000);
	close(tabla.cabecera->nid);
	player*temp;
	stack*tempstack;

	puts("Eliminando jugadores activos.");
	while(!list_is_empty(tabla.ready)){
		sleep(1);
		temp=(player*)list_remove(tabla.ready,0);
		while (!list_is_empty(temp->stack)){
			puts("Recurso eliminado.");
			tempstack=(stack*)list_remove(temp->stack,0);
			free(tempstack);
		}
		sendAnswer(0,0,' ',' ',temp->pid);
		close(temp->pid);
		free(temp->stack);
		free(temp);
	}
	free(tabla.ready);

	sleep(2);
	puts("Eliminando jugadores dormidos.");
	while(!list_is_empty(tabla.sleeps)){
		temp=(player*)list_remove(tabla.sleeps,0);
		while (list_is_empty(temp->stack)){
			puts("Recurso eliminado.");
			tempstack=(stack*)list_remove(temp->stack,0);
			free(tempstack);
		}
		sendAnswer(0,0,' ',' ',temp->pid);
		close(temp->pid);
		free(temp->stack);
		free(temp);
	}
	free(tabla.sleeps);

	sleep(2);
	puts("Eliminando recursos.");
	while (!list_is_empty(tabla.recur)){
		tempstack=(stack*)list_remove(tabla.recur,0);
		free(tempstack);
	}
	free(tabla.recur);
	free(tabla.algo);

	sleep(2);
	puts("Nos Vamos todos al carajo!");
	exit (EXIT_FAILURE);
}
void muertePersonaje(int i,global tabla){
	puts("Personaje Desconectado, Procesando..");
	bool _is_PID(player*jugador) {
		    if(jugador->pid==i)return true;
		    return false;
			}
	usleep(500000);
	puts("Localizando cadaver.");
	player*aux = list_remove_by_condition(tabla.ready,(void*) _is_PID);
	if(aux == NULL) {
		sleep(2);
		puts("Buscando entre los dormidos.");
		aux=list_remove_by_condition(tabla.ready,(void*) _is_PID);
		if(aux==NULL){
			puts("No se ha podido ubicar el fiambre.");
			return;
		}
	}
	sleep(2);
	puts("Fiambre localizado");
	FD_CLR(aux->pid,tabla.original);
	close(aux->pid);
	puts("Vaciando la lista de recursos..");
	while (!list_is_empty(aux->stack)){
		stack*tempstack;
		tempstack=(stack*)list_remove(aux->stack,0);
		list_add(tabla.recur, (void*)tempstack);
		puts("Borrando recurso.");
		usleep(500000);
	}
	sleep(2);
	puts("Lista vaciada.");
	sendAnswer(8,0,0,aux->sym,tabla.cabecera->nid);
	free(aux);
	puts("Personaje Completamente eliminado!!\n");
}
void matarPersonaje(char simbolo,global tabla){
	puts("Murio un Personaje.");
	bool _is_Personaje(player*jugador) {
		    if(jugador->sym==simbolo)return true;
		    return false;
			}
	player*aux = list_find(tabla.ready, (void*) _is_Personaje);
	if(aux == NULL){
		aux=list_find(tabla.sleeps, (void*) _is_Personaje);
		if(aux==NULL){
			puts("Parece que ya habia muerto antes, solicitud ignorada.");
			return;
		}
	}
	puts("Avisandole sobre la situacion al pobre..");
	sendAnswer(8,0,' ',' ',aux->pid);
}
void interrupcion(int i,short respuesta,answer* aux,global tabla){
	puts("\nManejando la interrupcion.");
	sleep(1);
	if(i==tabla.cabecera->nid){
		puts("La interrupcion no se puede enmascarar, atendiendo..");
		switch(respuesta){
		case 0:aLaMierdaConTodo(tabla);
		break;
		case 6:modificarAlgoritmo(*aux,tabla);
		break;
		case 8:matarPersonaje(aux->symbol,tabla);
		break;
		}
	}else{
		if(respuesta!=0)puts("Pescado Enmascarado");
		else muertePersonaje(i,tabla);

	}
}
int selectear(answer*tempo,short esperado,fd_set*originalfds,int fdmax,int sock,global tabla){
	answer aux;
	short respuesta;
	fd_set readfds;
	do{
		readfds=*originalfds;
		puts("Selecteando..");
		selectGRID(fdmax,&readfds);
		int i=0;
		while ((!FD_ISSET(i,&readfds))&&(i<=fdmax))	i++;
		if (i>fdmax){
			puts("--ERROR:No se encontro candidato para selectear!!--");
			exit(1);
		}else{
			printf("Se escuchara al socket numero %d\n",i);
			respuesta=recvAnswer(&aux,i);
			if (i==sock){
				if((respuesta==esperado)||(respuesta==-1)){
					if (tempo!=NULL)*tempo=aux;
					return respuesta;
				}
				else interrupcion(i,respuesta,&aux,tabla);
			}else{
				interrupcion(i,respuesta,&aux,tabla);
			}
		}
	}while(1);
	return -1;
}


void darInstancia(player*jugador,stack*instancia,global*tabla){
	list_add(jugador->stack,(void*)instancia);
	jugador->data.recsol=' ';
	jugador->data.dist=tabla->algo->RemainDist;
	bool _is_PID(player*personaje) {
		if(personaje->pid==jugador->pid)return true;
		return false;
	}
	list_remove_by_condition(tabla->sleeps,(void*) _is_PID);
	list_add(tabla->ready,(void*)jugador);
}
void asignarRecursos(global*tabla){
	short respuesta;
	answer temp;
	void intentarAsignar(void*paquete){
		player*jugador;
		jugador=(player*)paquete;
		bool hayInstancia(void*dato){
			stack*instancia;
			instancia=(stack*)dato;
			return instancia->recurso==jugador->data.recsol ? true : false;
		}
		stack*recurso=list_remove_by_condition(tabla->recur, hayInstancia);
		if (recurso!=NULL){
			darInstancia(jugador,recurso,tabla);
		}else{
			sendAnswer(2,1,jugador->data.recsol,jugador->sym,jugador->pid);
			respuesta=selectear(&temp,1,tabla->original,*(tabla->maxfd),jugador->pid,*tabla);
			if (respuesta==-1)return;
			stack*recnuevo=(stack*)malloc(sizeof(stack));
			recnuevo->recurso=jugador->data.recsol;
			darInstancia(jugador,recnuevo,tabla);
		}
		puts("Instancia concedida.");
	}
	list_iterate(tabla->sleeps,intentarAsignar);
}

void devolverRecursos(global*tabla){
	if(!list_is_empty(tabla->recur)){
		stack*tempstack;
		sendAnswer(5,0,' ',' ',tabla->cabecera->nid);
		if(selectear(NULL,1,tabla->original,*(tabla->maxfd),tabla->cabecera->nid,*tabla)==-1){
			puts("El nivel no quiere recibir nada por ahora.");
			return;
		}else{
			while (!list_is_empty(tabla->recur)){
				tempstack=(stack*)list_remove(tabla->recur,0);
				sendAnswer(2,0,tempstack->recurso,' ',tabla->cabecera->nid);
				if(selectear(NULL,1,tabla->original,*(tabla->maxfd),tabla->cabecera->nid,*tabla)==-1){
					puts("El nivel no quiso recibir nada por ahora.");
					return;
				}
				free(tempstack);
				usleep(100000);
			}
			sendAnswer(5,0,' ',' ',tabla->cabecera->nid);
		}
	}
}









