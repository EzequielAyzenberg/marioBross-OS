

#include"Planificador.h"


#define RD_INICIAL 0

int selectGRID_planificador(int,fd_set*);
void _each_Personaje(void*);

void modificarAlgoritmo(answer,global);
void modificarRetardo(answer,global);

void inicializar(nodoNivel*,global*);

void borrarNodo(nodoNivel*);
void crearStruct(nodoNivel*,t_player*,int);
int leerNovedad(global*);

void reordenar(t_list*ready,int);
void cargarAlFinal(t_player*,t_list*,int);

int modoDeRecuperacion(global);
int aLaMierdaConTodo(global);
bool muertePersonaje(int, global);
void matarPersonaje(char,global);
int interrupcion(int,short,answer*,global);
//int selectear2(answer*,short,int,global);
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
	t_list *stack,*ready,*sleeps;

	stack=list_create();
	ready=list_create();
	sleeps=list_create();
	t_exec exec;
	exec.player=NULL;
	exec.rem_cuantum=0;
	fd_set master;
	int maxfd;
	FD_ZERO(&master);
	maxfd=0;
	global general;
		general.algo=(struct algo*)malloc(sizeof(struct algo));
		general.cabecera=raiz;
		general.ready=ready;
		general.sleeps=sleeps;
		general.algo->remainDist=0;
		general.algo->algo=0;
		general.exe=&exec;
		general.recur=stack;
		general.original=&master;
		general.maxfd=&maxfd;
		general.playing=false;
	printf("Nuestro Nivel Se llama: %s\n",raiz->name);
	puts("\nEnviando Saludos al nivel..");
	inicializar(raiz,&general);
	short respuesta;
	while (1){
		if(finalizar){
			aLaMierdaConTodo(general);
			break;
		}
		if(general.cabecera->cantJugadores==0){
			if(selectGRID_planificador(*(general.maxfd),general.original)>0){
				answer temp;
				respuesta=interrupcion(general.cabecera->nid,0,&temp,general);
			}
		}
		respuesta=leerNovedad(&general);	//Si hay una novedad, responde un 1, sino un 0 y se sigue con otra cosa.
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
//free(raiz);
return 0;
}

int selectGRID_planificador(int fdmax,fd_set*original){
	fd_set readfds = *original;
	struct timeval intervalo;
	intervalo.tv_usec = 100000;
	intervalo.tv_sec = 0;
	return select(fdmax +1,&readfds,NULL,NULL,&intervalo);
};

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
	FD_SET(raiz->nid,general->original);
	if(raiz->nid>*(general->maxfd))*(general->maxfd)=raiz->nid;
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
int leerNovedad(global*tanda){
	if (tanda->cabecera->tandaRaiz->pid==0)return 0;
	else{
		int respuesta;
		t_player*temp;
		puts("Se ha conectado un jugador!!");
		puts("Avisandole al nivel..");
		sendAnswer(7,0,' ',tanda->cabecera->tandaRaiz->sym,(short)tanda->cabecera->nid);	//Le aviso al nivel que hay un nuevo jugador.
		//respuesta=selectear2(NULL,1,tanda->cabecera->nid,*tanda);
		respuesta=selectear(NULL,1,tanda->original,*(tanda->maxfd),tanda->cabecera->nid,*tanda);	//Selecteo hasta que el nivel me responda 1 (-1 Siempre es una opcion de respuesta).
		switch (respuesta){
			case 1:puts("--El nivel ha dado el ok.--");
			puts("Cargando jugador a la base de datos..");
			temp=malloc(sizeof(t_player));
			crearStruct(tanda->cabecera,temp,tanda->algo->remainDist);
			cargarAlFinal(temp,tanda->ready,tanda->algo->algo); //Carga al final y reordena si es necesario.
			borrarNodo(tanda->cabecera);
			FD_SET(temp->pid,tanda->original);
			if(temp->pid>*(tanda->maxfd))*(tanda->maxfd)=temp->pid;
			tanda->cabecera->cantJugadores++;		//Si el nivel da el ok, entonces aumento la cantidad de jugadores activos.
			sendAnswer(1,0,' ',' ',temp->pid);
			puts("\nLa lista hasta ahora a quedado asi:");
			list_iterate(tanda->ready,_each_Personaje);		//Muestra por pantalla como esta la lista.
			puts("");
			break;
			case -1:puts("--ERROR: El nivel comenta que hubo un error.--");
			sendAnswer(-1,0,' ',' ',tanda->cabecera->tandaRaiz->pid);
			borrarNodo(tanda->cabecera);
			break;
			case -2:return -2;
			break;
			case -3:return 1;
			break;
		}
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

int modoDeRecuperacion(global tabla){
	int status;
	puts("El nivel se ha caido, limpiando registros..");
	FD_CLR(tabla.cabecera->nid,tabla.original);
	tabla.cabecera->nid=0;
	list_clean(tabla.recur);
	puts("Esperando por la reconexion.");
	int cont=0;
	do{
		sleep(5);
		puts("Intentando establecer conexion.");
		if(cont==5 || finalizar){
			puts("Abortando intento de reconexion.");
			aLaMierdaConTodo(tabla);
			return -2;
		}
		cont++;
	}while(tabla.cabecera->nid==0);
	int nid=tabla.cabecera->nid;
	answer temp;

	void _Reestablecer_Recursos(void*paquete){
		t_player*jugador;
		jugador=(t_player*)paquete;
		sendAnswer(7,0,' ',jugador->sym,nid);
		usleep(50000);
		recvAnswer(&temp,nid);
		sendAnswer(3,jugador->data.pos,' ',jugador->sym,nid);
		usleep(50000);
		recvAnswer(&temp,nid);
		void _Pedir_El_Recurso(void*package){
			t_stack*recurso;
			recurso=(t_stack*)package;
			sendAnswer(2,1,recurso->recurso,jugador->sym,nid);
			usleep(50000);
			recvAnswer(&temp,nid);
		}
		if(!list_is_empty(jugador->t_stack))list_iterate(jugador->t_stack,_Pedir_El_Recurso);

	}
	puts("El nivel se ha reconectado, reestableciendo la informacion..");
	inicializar(tabla.cabecera,&tabla);
	if(!list_is_empty(tabla.ready)){
		puts("Cargando Personajes Activos..");
		list_iterate(tabla.ready,_Reestablecer_Recursos);
	}
	if(!list_is_empty(tabla.sleeps)){
		puts("Cargando Personajes Dormidos..");
		list_iterate(tabla.sleeps,_Reestablecer_Recursos);
	}
	if(tabla.exe->player!=NULL){
		puts("Cargando jugador en Ejecucion..");
		_Reestablecer_Recursos((void*)tabla.exe->player);
	}
	puts("Esta todo listo para seguir con la ejecucion!");
	if(tabla.exe->player==NULL)return 1;
	status=tabla.exe->player->pid;
	sleep(1);
	return status;
}

int aLaMierdaConTodo(global tabla){
	usleep(500000);
	t_player*temp;
	t_stack*tempstack;
	nuevo*aux;

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

	sleep(1);
	puts("Eliminando jugadores dormidos.");
	while(!list_is_empty(tabla.sleeps)){
		temp=(t_player*)list_remove(tabla.sleeps,0);
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
	free(tabla.sleeps);

	sleep(1);
	puts("Eliminando jugador en ejecucion.");
	if(tabla.exe->player!=NULL){
		sendAnswer(0,0,' ',' ',tabla.exe->player->pid);
		close(tabla.exe->player->pid);
		while (!list_is_empty(tabla.exe->player->t_stack)){
			puts("Recurso eliminado.");
			tempstack=(t_stack*)list_remove(tabla.exe->player->t_stack,0);
			free(tempstack);
		}
		free(tabla.exe->player->t_stack);
		free(tabla.exe->player);
	}
	sleep(1);
	puts("Eliminando jugadores nuevos.");
	while(tabla.cabecera->tandaRaiz!=NULL){
		aux=tabla.cabecera->tandaRaiz;
		tabla.cabecera->tandaRaiz=tabla.cabecera->tandaRaiz->sgte;
		sendAnswer(0,0,' ',' ',aux->pid);
		close(aux->pid);
		free(aux);
	}

	sleep(1);
	puts("Eliminando recursos.");
	while (!list_is_empty(tabla.recur)){
		tempstack=(t_stack*)list_remove(tabla.recur,0);
		free(tempstack);
	}
	free(tabla.recur);
	free(tabla.algo);
	sendAnswer(0,0,' ',' ',tabla.cabecera->nid);
	close(tabla.cabecera->nid);
	sleep(2);
	printf("Mi ID-Hilo es: %d",(int)tabla.cabecera->idHilo);
	//pthread_cancel(tabla.cabecera->idHilo);
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
	if(tabla.exe->player!=NULL){
		if(aux->pid==tabla.exe->player->pid){
			tabla.exe->player=NULL;
			chosen =true;
		}
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
	int status=-1;
	if(i==tabla.cabecera->nid){
		puts("La interrupcion no se puede enmascarar, atendiendo..");
		switch(respuesta){
		case 0:status=modoDeRecuperacion(tabla);
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

/*int selectear2(answer*paqueteDeRespuesta,short respuestaEsperada,int socketEsperado,global tabla){
	answer aux;
	short respuesta;
	fd_set readfds;
	int status, fdmax=*(tabla.maxfd);
	do{
		readfds=*(tabla.original);
		puts("Selecteando..");
		selectGRID(fdmax,&readfds);
		int i=0;
		while ((!FD_ISSET(i,&readfds))&&(i<=fdmax))	i++;
		printf("Escuchamos al socket Nº: %d",i);
		if(i==tabla.cabecera->nid)printf(" -- NID\n");
		else printf("\n");
		respuesta=recvAnswer(&aux,i);
		if(i==socketEsperado){	//Me habla quien yo esperaba que me hable
			puts("Me hablo quien YO queria");
			if((respuesta==respuestaEsperada)||(respuesta==-1)||(respuestaEsperada==10)){	//Me dice lo que esperaba me dijera
				if (paqueteDeRespuesta!=NULL)*paqueteDeRespuesta=aux;
				return respuesta;
			}else {puts("Me estadiciendo cualquier otra cosa");status=interrupcion(i,respuesta,&aux,tabla); }//Me dice otra cosa
			if (status==0)break;	//Si alguien murio y soy yo, No tiene sentido seguir esperando una respuesta.
			if(status>0){		//Sirve para la reconexion del Nivel
				answer temp;
				if(tabla.playing){
					if(tabla.exe->player->data.recsol!=' '){
						sendAnswer(2,0,tabla.exe->player->data.recsol,tabla.exe->player->sym,tabla.cabecera->nid);
						usleep(50000);
						recvAnswer(&temp,tabla.cabecera->nid);
						if(calcularDistancia(tabla.exe->player->data.pos,temp.cont)!=tabla.exe->player->data.dist){
							sendAnswer(2,temp.cont,tabla.exe->player->data.recsol,' ',tabla.exe->player->pid);
							tabla.exe->player->data.dist=calcularDistancia(tabla.exe->player->data.pos,temp.cont);
						}else sendAnswer(1,0,' ',' ',tabla.exe->player->pid);
					}
				}
			return -3;
			}
		}else {puts("QUIEN CARAJO ME HABLO!!");status=interrupcion(i,respuesta,&aux,tabla);	}	//Me hablo otra persona que nada que ver.
		if(status==-2)break;
		puts("Termine una iteracion");
	}while(1);
	return status;
}*/


char buscarSimbolo(int i, global tabla){
	bool _is_PID(t_player*personaje) {
		if(personaje->pid==i)return true;
		return false;
	}
	t_player*jugador;
	jugador=list_find(tabla.ready,(void*)_is_PID);
	if(jugador==NULL){
		jugador=list_find(tabla.sleeps,(void*)_is_PID);
		if(jugador==NULL&&tabla.exe->player!=NULL)jugador=tabla.exe->player;
		else puts("NO SE ENCONTRO A NADIE");
	}
	return jugador->sym;
}

int selectear(answer*tempo,short esperado,fd_set*originalfds,int fdmax,int sock,global tabla){
	answer aux;
	short respuesta;
	fd_set readfds;
	int status;
	do{
		readfds=*originalfds;
		printf("Selecteando..Esperado/Recibido: %d-/",sock);
		selectGRID(fdmax,&readfds);
		int i=0;
		while ((!FD_ISSET(i,&readfds))&&(i<=fdmax))	i++;
		if (i>fdmax){
			puts("--ERROR:No se encontro candidato para selectear!!--");
			exit(1);
		}else{
			printf("=>%d",i);
			respuesta=recvAnswer(&aux,i);
			printf("--Msg:%d-Cnt:%d-Dta:%c-",(int)aux.msg,(int)aux.cont,aux.data);
			if(i==tabla.cabecera->nid)printf("==>NID\n");
			else {
				printf("-- %c\n",buscarSimbolo(i,tabla));
			}
			if (i==sock&&respuesta!=0){
				if((respuesta==esperado)||(respuesta==-1)||(esperado==10)){
					if (tempo!=NULL)*tempo=aux;
					return respuesta;
				}else status=interrupcion(i,respuesta,&aux,tabla);
				if (status==0)break;
			}else{
				status=interrupcion(i,respuesta,&aux,tabla);
				if (status==0&&i==sock)break;
				if(status>0&&i==sock){
					answer temp;
					if(tabla.playing){
						if(tabla.exe->player->data.recsol!=' '){
							sendAnswer(2,0,tabla.exe->player->data.recsol,tabla.exe->player->sym,tabla.cabecera->nid);
							usleep(50000);
							recvAnswer(&temp,tabla.cabecera->nid);
							if(calcularDistancia(tabla.exe->player->data.pos,temp.cont)!=tabla.exe->player->data.dist){
								sendAnswer(2,temp.cont,tabla.exe->player->data.recsol,' ',tabla.exe->player->pid);
								tabla.exe->player->data.dist=calcularDistancia(tabla.exe->player->data.pos,temp.cont);
							}else sendAnswer(1,0,' ',' ',tabla.exe->player->pid);
						}
					}
				return -3;
				}
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
	int status=1;
	void intentarAsignar(void*paquete){
		t_player*jugador;
		jugador=(t_player*)paquete;
		puts("Pidiendole recurso al nivel.");
		sendAnswer(2,1,jugador->data.recsol,jugador->sym,tabla->cabecera->nid);
		//respuesta=selectear2(&temp,1,tabla->cabecera->nid,*tabla);
		respuesta=selectear(&temp,1,tabla->original,*(tabla->maxfd),tabla->cabecera->nid,*tabla);
		if (respuesta==-1)return;
		if (respuesta==-2){
			status=-2;
			return;
		}
		t_stack*recnuevo;
		recnuevo=(t_stack*)malloc(sizeof(t_stack));
		recnuevo->recurso=jugador->data.recsol;
		darInstancia(jugador,recnuevo,tabla);
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
		//status=selectear2(NULL,1,tabla->cabecera->nid,*tabla);
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
				//status=selectear2(NULL,1,tabla->cabecera->nid,*tabla);
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
	tabla->playing=true;
	sendAnswer(3,aux.cont,' ',aux.symbol,tabla->cabecera->nid);
	tabla->exe->player->data.pos=aux.cont;
	tabla->exe->player->data.dist--;
	//if(selectear2(&aux,1,tabla->cabecera->nid,*tabla)==-3)return;
	if(selectear(&aux,1,tabla->original,*(tabla->maxfd),tabla->cabecera->nid,*tabla)==-3)return;
	sendAnswer(1,0,' ',' ',tabla->exe->player->pid);
	tabla->playing=false;
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
	tabla.exe->player->data.recsol=aux.data;
	tabla.playing=true;
	sendAnswer(2,0,aux.data,aux.symbol,tabla.cabecera->nid);
	//if(selectear2(&aux,2,tabla.cabecera->nid,tabla)==-3)return;
	if(selectear(&aux,2,tabla.original,*(tabla.maxfd),tabla.cabecera->nid,tabla)==-3)return;
	sendAnswer(2,aux.cont,aux.symbol,' ',tabla.exe->player->pid);
	tabla.playing=false;
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
		//puts("Hora de que termine su turno un jugador.");
		cargarAlFinal(tabla->exe->player,tabla->ready,tabla->algo->algo);
		tabla->exe->player=NULL;
	}
	if (tabla->exe->player==NULL){
		if(!list_is_empty(tabla->ready)){
			//puts("Nuevo Jugador cargado a exec");
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
	//respuesta=selectear2(&back,10,sock,*tabla);
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













