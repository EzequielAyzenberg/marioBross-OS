

#include"Planificador.h"


#define RD_INICIAL 0

void _each_Personaje(void*);

void modificarAlgoritmo(answer,global);
void modificarRetardo(answer,global);

void inicializar(nodoNivel*,global*);

void borrarNodo(nodoNivel*);
void crearStruct(nodoNivel*,t_player*,int);
int leerNovedad(nodoNivel*,global*,t_player*);

void reordenar(t_list*ready,int);
void cargarAlFinal(t_player*,t_list*,int);

int aLaMierdaConTodo(global);
bool muertePersonaje(int, global);
void matarPersonaje(char,global);
int interrupcion(int,short,answer*,global);
int selectear(answer*,short,fd_set*,int,int,global);

void darInstancia(t_player*,t_stack*,global*);
int asignarRecursos(global*);

int devolverRecursos(global*);

void movimiento(global*,answer);
void dormirJugador(t_player*,t_list*);
void instancia(global);
int calcularDistancia(int,int);
void ubicacion(answer,global);
void recurso(global*,answer);
void cargarAExec(global*);
bool concederTurno(global*);
int atenderJugador(global*);


void *planificador (void *parametro){
	puts("\nHola mundo!!--Yo planifico.");
	nodoNivel*raiz=(nodoNivel*)parametro;
	t_list *t_stack,*ready,*sleeps;

	t_stack=list_create();
	ready=list_create();
	sleeps=list_create();
	t_exec exec;
	exec.player=NULL;
	exec.rem_cuantum=0;
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
		general.algo->remainDist=0;
		general.algo->algo=0;
		general.exe=&exec;
		general.recur=t_stack;
		general.original=&master;
		general.maxfd=&maxfd;
	printf("Nuestro Nivel Se llama: %s\n",raiz->name);
	puts("\nEnviando Saludos al nivel..");
	inicializar(raiz,&general);
	t_player*temp;
	int estado=1;
	short respuesta;
	t_player aux;
	while (1){ 		//Solo por ahora lee conexiones!! (estado!=0)
		estado=leerNovedad(raiz,&general,&aux);	//Si hay una novedad, responde un 1, sino un 0 y se sigue con otra cosa.
		if(estado!=0){
			puts("Avisandole al nivel..");
			sendAnswer(7,0,' ',aux.sym,(short)raiz->nid);	//Le aviso al nivel que hay un nuevo jugador.
			respuesta=selectear(NULL,1,&master,maxfd,raiz->nid,general);	//Selecteo hasta que el nivel me responda 1 (-1 Siempre es una opcion de respuesta).
			switch (respuesta){
				case 1:puts("--El nivel ha dado el ok.--");
				puts("Cargando jugador a la base de datos..");
				temp=malloc(sizeof(t_player));
				*temp=aux;
				cargarAlFinal(temp,ready,general.algo->algo); //Carga al final y reordena si es necesario.
				FD_SET(temp->pid,&master);
				if(temp->pid>maxfd)maxfd=temp->pid;
				raiz->cantJugadores++;		//Si el nivel da el ok, entonces aumento la cantidad de jugadores activos.
				sendAnswer(1,0,' ',' ',temp->pid);
				puts("\nLa lista hasta ahora a quedado asi:");
				list_iterate(ready,_each_Personaje);		//Muestra por pantalla como esta la lista.
				puts("");
				break;
				case -1:puts("--ERROR: El nivel comenta que hubo un error.--");
				sendAnswer(-1,0,' ',' ',aux.pid);
				break;
			}
		}
		if (respuesta==-2) break;
		//puts("Asignando Recursos");
		respuesta=asignarRecursos(&general);
		if (respuesta==-2) break;
		//puts("Devolviendo Recursos");
		respuesta=devolverRecursos(&general);
		if (respuesta==-2) break;
		//puts("Atendiendo al jugador");
		respuesta=atenderJugador(&general);
		if (respuesta==-2) break;
		//puts("Volviendo a empezar");
		usleep((general.algo->retardo)*1000);
	}
puts("El hilo termina ahora!!");

	return 0;
}

void _each_Personaje(void*jug){
	t_player* jugador;
	jugador=(t_player*)jug;
	static int contador;
	printf("El jugador Nº %d ",contador);
	printf("es: %c\n",jugador->sym);
	contador++;
}

void modificarAlgoritmo(answer temp,global general){
	general.algo->algo=temp.cont;
	general.algo->remainDist=(temp.data-'0')*10;
	if(general.algo->algo==0)puts("Se ha elegido usar el Algoritmo SRDF.");
	else printf("Se ha elegido usar el Algoritmo Round Robins Q==%d\n",general.algo->algo);
	printf("El Remaining Distance ahora es de: %d\n\n",general.algo->remainDist);

	printf("letra: %c ---",temp.data);
	printf("letra: %c ---",temp.data);
}
void modificarRetardo(answer temp,global general){
	general.algo->retardo=temp.cont;
	printf("El Retardo entre turnos ahora es de: %d\n\n",general.algo->retardo);
}

void inicializar(nodoNivel*raiz,global*general){
	int estado;
	answer tempo;
	do{
	puts("\nPidiendo algoritmo.");
	sendAnswer(6,0,' ',' ',(short)raiz->nid);
	estado=recvAnswer(&tempo,(short)raiz->nid);
	if(estado!=6) puts("El nivel flasheo cualquiera!");
	}while(estado!=6);
	modificarAlgoritmo(tempo,*general);
	do{
	puts("\nPidiendo retardo entre turnos.");
	sendAnswer(4,0,' ',' ',(short)raiz->nid);
	estado=recvAnswer(&tempo,(short)raiz->nid);
	if(estado!=4) puts("El nivel flasheo cualquiera!");
	}while(estado!=4);
	modificarRetardo(tempo,*general);
}


void borrarNodo(nodoNivel*raiz){
	nuevo*aux;
	aux=raiz->tandaRaiz;
	raiz->tandaRaiz=raiz->tandaRaiz->sgte;
	free(aux);
}
void crearStruct(nodoNivel*raiz,t_player*temp,int RD){
	temp->pid=raiz->tandaRaiz->pid;
	temp->sym=raiz->tandaRaiz->sym;
	temp->data.pos=0;
	temp->data.dist=RD;
	temp->data.recsol=' ';
	temp->t_stack=list_create();
}
int leerNovedad(nodoNivel*raiz,global*general,t_player*temp){
	if (raiz->tandaRaiz->pid==0)return 0;
	else{
		crearStruct(raiz,temp,general->algo->remainDist);
		puts("Se ha conectado un jugador!!");
		borrarNodo(raiz);
	}
	return 1;
}

bool comparator(void*anterior, void*actual){
	t_player *ant,*act;
	ant=(t_player*)anterior;
	act=(t_player*)actual;
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
void cargarAlFinal(t_player*temp,t_list*ready,int RR){
	list_add(ready, (void*)temp);
	reordenar(ready,RR);
}

int aLaMierdaConTodo(global tabla){
	puts("Se cayo el nivel, procesando..");
	usleep(500000);
	close(tabla.cabecera->nid);
	t_player*temp;
	t_stack*tempstack;

	puts("Eliminando jugadores activos.");
	while(!list_is_empty(tabla.ready)){
		sleep(1);
		temp=(t_player*)list_remove(tabla.ready,0);
		while (!list_is_empty(temp->t_stack)){
			puts("Recurso eliminado.");
			tempstack=(t_stack*)list_remove(temp->t_stack,0);
			free(tempstack);
		}
		sendAnswer(0,0,' ',' ',temp->pid);
		close(temp->pid);
		free(temp->t_stack);
		free(temp);
	}
	free(tabla.ready);

	sleep(2);
	puts("Eliminando jugadores dormidos.");
	while(!list_is_empty(tabla.sleeps)){
		temp=(t_player*)list_remove(tabla.sleeps,0);
		while (list_is_empty(temp->t_stack)){
			puts("Recurso eliminado.");
			tempstack=(t_stack*)list_remove(temp->t_stack,0);
			free(tempstack);
		}
		sendAnswer(0,0,' ',' ',temp->pid);
		close(temp->pid);
		free(temp->t_stack);
		free(temp);
	}
	free(tabla.sleeps);

	sleep(2);
	puts("Eliminando recursos.");
	while (!list_is_empty(tabla.recur)){
		tempstack=(t_stack*)list_remove(tabla.recur,0);
		free(tempstack);
	}
	free(tabla.recur);
	free(tabla.algo);
	tabla.cabecera->nid=0;
	sleep(2);
	puts("Nos Vamos todos al carajo!");
	return -2;
}
bool muertePersonaje(int i,global tabla){
	bool chosen=false;
	puts("Personaje Desconectado, Procesando..");
	bool _is_PID(t_player*jugador) {
		    if(jugador->pid==i)return true;
		    return false;
			}
	usleep(500000);
	puts("Localizando cadaver.");
	t_player*aux = list_remove_by_condition(tabla.ready,(void*) _is_PID);
	if(aux == NULL) {
		sleep(2);
		puts("Buscando entre los dormidos.");
		aux=list_remove_by_condition(tabla.sleeps,(void*) _is_PID);
		if(aux==NULL){
			sleep(2);
			puts("Quizas se estaba ejecutando.");
			if(tabla.exe->player==NULL||tabla.exe->player->pid!=i){
				puts("No se ha podido ubicar el fiambre.");
				return false;
			}else{
				aux=tabla.exe->player;
			}
		}
	}
	sleep(2);
	puts("Fiambre localizado");
	FD_CLR(aux->pid,tabla.original);
	puts("Vaciando la lista de recursos..");
	while (!list_is_empty(aux->t_stack)){
		t_stack*tempstack;
		tempstack=(t_stack*)list_remove(aux->t_stack,0);
		list_add(tabla.recur, (void*)tempstack);
		puts("Borrando recurso.");
		usleep(500000);
	}
	sleep(2);
	puts("Lista vaciada.");
	tabla.cabecera->cantJugadores--;
	if(tabla.cabecera->cantJugadores==0)printf("\n--ATENCION--No quedan jugadores!!\n");
	sendAnswer(8,0,0,aux->sym,tabla.cabecera->nid);
	puts("Personaje Completamente eliminado!!\n");
	if(aux->pid==tabla.exe->player->pid){
		tabla.exe->player=NULL;
		chosen =true;
	}
	close(aux->pid);
	free(aux);
	return chosen;

}
void matarPersonaje(char simbolo,global tabla){
	puts("Murio un Personaje.");



	bool _is_Personaje(t_player*jugador) {
		    if(jugador->sym==simbolo)return true;
		    return false;
			}
	t_player*aux = list_find(tabla.ready, (void*) _is_Personaje);
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
int interrupcion(int i,short respuesta,answer* aux,global tabla){
	puts("\nManejando la interrupcion.");
	sleep(1);
	int status;
	if(i==tabla.cabecera->nid){
		puts("La interrupcion no se puede enmascarar, atendiendo..");
		switch(respuesta){
		case 0:status=aLaMierdaConTodo(tabla);
		break;
		case 4:modificarRetardo(*aux,tabla);
		break;
		case 6:modificarAlgoritmo(*aux,tabla);
		break;
		case 8:matarPersonaje(aux->symbol,tabla);
		break;
		}
	}else{
		if(respuesta!=0)puts("Pescado Enmascarado");
		else{
			if(muertePersonaje(i,tabla))return 0;
		}
	}
	return status;
}
int selectear(answer*tempo,short esperado,fd_set*originalfds,int fdmax,int sock,global tabla){
	answer aux;
	short respuesta;
	fd_set readfds;
	int status;
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
			if (i==sock&&respuesta!=0){
				if((respuesta==esperado)||(respuesta==-1)||(esperado==10)){
					if (tempo!=NULL)*tempo=aux;
					return respuesta;
				}else status=interrupcion(i,respuesta,&aux,tabla);
				if (status==0)break;
			}else{
				status=interrupcion(i,respuesta,&aux,tabla);
				if (status==0&&i==sock)break;
			}
			if (status==-2)break;

		}
	}while(1);
	return status;
}


void darInstancia(t_player*jugador,t_stack*instancia,global*tabla){
	list_add(jugador->t_stack,(void*)instancia);
	jugador->data.recsol=' ';
	jugador->data.dist=tabla->algo->remainDist;
	bool _is_PID(t_player*personaje) {
		if(personaje->pid==jugador->pid)return true;
		return false;
	}
	list_remove_by_condition(tabla->sleeps,(void*) _is_PID);
	list_add(tabla->ready,(void*)jugador);
	sendAnswer(1,0,' ',' ',jugador->pid);
}
int asignarRecursos(global*tabla){
	short respuesta;
	answer temp;
	int status;
	void intentarAsignar(void*paquete){
		t_player*jugador;
		jugador=(t_player*)paquete;
		bool hayInstancia(void*dato){
			t_stack*instancia;
			instancia=(t_stack*)dato;
			return instancia->recurso==jugador->data.recsol ? true : false;
		}
		t_stack*recurso=list_remove_by_condition(tabla->recur, hayInstancia);
		if (recurso!=NULL){
			puts("Se encontro un recurso en el armario.");
			darInstancia(jugador,recurso,tabla);
		}else{
			puts("Pidiendole recurso al nivel.");
			sendAnswer(2,1,jugador->data.recsol,jugador->sym,tabla->cabecera->nid);
			respuesta=selectear(&temp,1,tabla->original,*(tabla->maxfd),tabla->cabecera->nid,*tabla);
			if (respuesta==-1)return;
			if (respuesta==-2){
				status=-2;
				return;
			}
			t_stack*recnuevo=(t_stack*)malloc(sizeof(t_stack));
			recnuevo->recurso=jugador->data.recsol;
			darInstancia(jugador,recnuevo,tabla);
		}
		puts("Instancia concedida.");
	}
	list_iterate(tabla->sleeps,intentarAsignar);
	if(status==-2)return -2;
	return 0;
}

int devolverRecursos(global*tabla){
	int status;
	if(!list_is_empty(tabla->recur)){
		t_stack*tempstack;
		sendAnswer(5,0,' ',' ',tabla->cabecera->nid);
		status=selectear(NULL,1,tabla->original,*(tabla->maxfd),tabla->cabecera->nid,*tabla);
		switch(status){
		case -1:
			puts("El nivel no quiere recibir nada por ahora.");
			return 0;
			break;
		case -2:
			return -2;
			break;
		default:
			while (!list_is_empty(tabla->recur)){
				tempstack=(t_stack*)list_remove(tabla->recur,0);
				sendAnswer(2,0,tempstack->recurso,' ',tabla->cabecera->nid);
				status=selectear(NULL,1,tabla->original,*(tabla->maxfd),tabla->cabecera->nid,*tabla);
				switch (status){
				case -1:
					puts("El nivel no quiso recibir nada por ahora.");
					return 0;
					break;
				case -2:
					return -2;
					break;
				}
				free(tempstack);
				usleep(100000);
			}
			sendAnswer(5,0,' ',' ',tabla->cabecera->nid);
			break;
		}
	}
	return 0;
}

void movimiento(global*tabla,answer aux){
	sendAnswer(3,aux.cont,' ',aux.symbol,tabla->cabecera->nid);
	tabla->exe->player->data.pos=aux.cont;
	tabla->exe->player->data.dist--;
	selectear(&aux,1,tabla->original,*(tabla->maxfd),tabla->cabecera->nid,*tabla);
	sendAnswer(1,0,' ',' ',tabla->exe->player->pid);
	if(tabla->exe->rem_cuantum!=0){
		if(tabla->exe->rem_cuantum==1)tabla->exe->rem_cuantum=-1;
		else tabla->exe->rem_cuantum--;
	}
}
void dormirJugador(t_player*jugador,t_list*dormidos){
	list_add(dormidos,(void*)jugador);
}
void instancia(global tabla){
	dormirJugador(tabla.exe->player,tabla.sleeps);
	tabla.exe->player=NULL;
	puts("Jugador a la espera de una instancia.");
}
int calcularDistancia(int inicial,int final){
	int x,y;
	x=abs(final/100-inicial/100);
	y=abs(final%100-inicial%100);
	return x+y;
}
void ubicacion(answer aux,global tabla){
	sendAnswer(2,0,aux.data,aux.symbol,tabla.cabecera->nid);
	selectear(&aux,2,tabla.original,*(tabla.maxfd),tabla.cabecera->nid,tabla);
	sendAnswer(2,aux.cont,' ',' ',tabla.exe->player->pid);
	tabla.exe->player->data.recsol=aux.data;
	tabla.exe->player->data.dist=calcularDistancia(tabla.exe->player->data.pos,aux.cont);
}
void recurso(global*tabla,answer aux){
	if(aux.cont==0) ubicacion(aux,*tabla);
	if(aux.cont==1) instancia(*tabla);
}
void cargarAExec(global*tabla){
	tabla->exe->player=(t_player*)list_remove(tabla->ready,0);
	tabla->exe->rem_cuantum=tabla->algo->algo;
}
bool concederTurno(global*tabla){
	if(tabla->exe->rem_cuantum==-1){
		puts("Hora de que termine un jugador.");
		cargarAlFinal(tabla->exe->player,tabla->ready,tabla->algo->algo);
		tabla->exe->player=NULL;
	}
	if (tabla->exe->player==NULL){
		if(!list_is_empty(tabla->ready)){
			puts("Nuevo Jugador cargado");
			cargarAExec(tabla);
		}else return false;
	}else{
		if(tabla->exe->rem_cuantum==0&&tabla->algo->algo!=0)tabla->exe->rem_cuantum=tabla->algo->algo;
		if(tabla->exe->rem_cuantum!=0&&tabla->algo->algo==0)tabla->exe->rem_cuantum=tabla->algo->algo;
	}
	sendAnswer(7,0,'.','.',(short)tabla->exe->player->pid);
	return true;
}
int atenderJugador(global*tabla){
	int respuesta,sock;
	answer back;
	if(!concederTurno(tabla))return 0;
	sock=tabla->exe->player->pid;
	respuesta=selectear(&back,10,tabla->original,*(tabla->maxfd),sock,*tabla);
	switch(respuesta){
	case -2:
		return -2;
		break;
	case 0:
		puts("Se murio el que estaba jugando, fin de turno.");
		return 0;
		break;
	case 2:
		recurso(tabla,back);
		break;
	case 3:
		movimiento(tabla,back);
		break;
	}

	return 0;
}













