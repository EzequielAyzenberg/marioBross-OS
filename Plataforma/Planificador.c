#include"Planificador.h"

#define muestreo false

void enviarLog(int,global,int,int,char,char);
void recibirLog(global,int,answer);
int selectGRID_planificador(int,fd_set*);

void modificarAlgoritmo(answer,global);
void modificarRetardo(answer,global);
void inicializar(nodoNivel*,global*);

void borrarNodo(nodoNivel*);
void crearStruct(nodoNivel*,t_player*,int);
int leerNovedad(global*);

bool comparator(void*,void*);
void reordenar(t_list*ready,int);
void cargarAlFinal(t_player*,t_list*,int);

t_player* buscarDormido(int,char,t_list*);
int modoDeRecuperacion(global);
int aLaMierdaConTodo(global);
bool muertePersonaje(int, global);
void matarPersonaje(answer,global);
int interrupcion(int,short,answer*,global);
char buscarSimbolo(int,global);
int selectear(answer*,short,int,global);

void darInstancia(t_player*,t_stack*,global*);
int asignarRecursos(global*);

//int devolverRecursos(global*);

int selectInterrupt(global);
void movimiento(global*,answer);
void dormirJugador(t_player*,t_list*);
void instancia(global);
int calcularDistancia(int,int);
void ubicacion(answer,global);
void recurso(global*,answer);
void cargarAExec(global*);
bool concederTurno(global*);
int atenderJugador(global*);

logs crearLogs(nodoNivel*);
void loggearActivos(global);
void loggearDormidos(global);
void loggearExec(global);
void loggearListas(global);
void cerrarLogging(global);

extern int defaultRD;
//-----------------------------------------------------------------------------------------------------------------------
void *planificador (void *parametro){
	puts("\nHola mundo!!--Yo planifico.");
	nodoNivel*raiz=(nodoNivel*)parametro;
	t_list *stack,*ready,*sleeps,*deads;
	stack=list_create();
	ready=list_create();
	sleeps=list_create();
	deads=list_create();
	t_exec exec;
	exec.player=NULL;
	exec.rem_cuantum=0;
	struct algo algor;
	fd_set master;
	set fdset;
	fdset.original=&master;
	int maxfd;
	FD_ZERO(&master);
	maxfd=0;
	logs logueo;
	logueo=crearLogs(raiz);
	global general;
		general.algo=&algor;
		general.cabecera=raiz;
		general.ready=ready;
		general.deads=deads;
		general.sleeps=sleeps;
		general.algo->remainDist=0;
		general.algo->algo=0;
		general.exe=&exec;
		general.recur=stack;
		general.original=&fdset;
		general.maxfd=&maxfd;
		general.playing=false;
		general.logging=logueo;
	raiz->ready=ready;
	raiz->sleeps=sleeps;
	raiz->deads=deads;
	raiz->exe=&exec;
	raiz->algo=&algor;
	printf("Nuestro Nivel Se llama: %s\n",raiz->name);
	puts("\nEnviando Saludos al nivel..");
	inicializar(raiz,&general);
	short respuesta;
	while (1){
		if(finalizar){
			aLaMierdaConTodo(general);
			break;
		}
		respuesta=selectInterrupt(general);
		if (respuesta==-2) break;
		loggearListas(general);
		respuesta=asignarRecursos(&general);
		if (respuesta==-2) break;
	//	respuesta=devolverRecursos(&general);
	//	if (respuesta==-2) break;
		respuesta=atenderJugador(&general);
		if (respuesta==-2) break;
		respuesta=selectInterrupt(general);
		if (respuesta==-2) break;
		respuesta=leerNovedad(&general);	//Si hay una novedad, responde un 1, sino un 0 y se sigue con otra cosa.
		if (respuesta==-2) break;
		usleep((general.algo->retardo)*1000);
	}
cerrarLogging(general);
puts("El hilo termina ahora!!");
return 0;
}
//-----------------------------------------------------------------------------------------------------------------------
/**/
int selectInterrupt(global tabla){
	printf("INTERRUPCION.I--%s\n",tabla.cabecera->name);
	int i=0,respuesta,status=1,fdmax=*(tabla.maxfd);
	char mensaje[128],mensajeError[64],numero[16],data[8];
	answer aux;
	fd_set readfds;
	bool muerto=false;
	do{
	readfds=*(tabla.original->original);
	if(selectGRID_planificador(fdmax,&readfds)>0){
		while ((!FD_ISSET(i,&readfds))&&(i<=fdmax)){
			i++;
		}
		log_trace(tabla.logging.trace,"\t\t\t------INTERRUPT------\t\t\t","TRACE");
		strcpy(mensaje,"Nº:I");
		itoa(i,numero,10);
		strcat(mensaje,"--Socket:(Esp./Recv.)-Msj:(Esp./Recv.)-(Msj/Cont/Dat/Sym)--Duenio:  --(N/");
		strcat(mensaje,numero);
		strcat(mensaje,")--(");
		if (i>fdmax){
			puts("--ERROR:No se encontro candidato para selectear!!--");
			strcpy(mensajeError,"Nº:I");
			strcat(mensajeError,"-ERROR:No se encontro candidato para selectear!!");
			log_info(tabla.logging.info,mensaje,"ERROR");
			exit(1);
		}else{
			//printf("%d",i);
			printf("INTERRUPCION.MID.I--%s\n",tabla.cabecera->name);
			respuesta=recvAnswer(&aux,i);
			printf("INTERRUPCION.MID.F--%s\n",tabla.cabecera->name);
			//printf("--EspMsg:X");
			strcat(mensaje,"ALL/");
			//printf("--RecvMsg:%d-Cnt:%d-Dta:%c-",(int)aux.msg,(int)aux.cont,aux.data);
			itoa((int)aux.msg,numero,10);
			strcat(mensaje,numero);
			strcat(mensaje,")--(");
			strcat(mensaje,numero);
			itoa((int)aux.cont,numero,10);
			strcat(mensaje,"/");
			strcat(mensaje,numero);
			strcat(mensaje,"/");
			data[0]=aux.data;
			data[1]='\0';
			strcat(mensaje,data);
			strcat(mensaje,"/");
			data[0]=aux.symbol;
			data[1]='\0';
			strcat(mensaje,data);
			strcat(mensaje,")--Duenio:");
			if(i==tabla.cabecera->nid)	strcat(mensaje,tabla.cabecera->name);
			else {
				data[0]=buscarSimbolo(i,tabla);
				data[1]='\0';
				strcat(mensaje,data);
			}
			strcat(mensaje,".");
			log_trace(tabla.logging.trace,mensaje,"TRACE");
			status=interrupcion(i,respuesta,&aux,tabla);
			if(status==-4)muerto=true;
			if(status==0||status==-5)muerto=false;
			log_trace(tabla.logging.trace,"\t\t\t------INTERRUPT------\t\t\t","TRACE");
		}
	}
	printf("INTERRUPCION.F--%s\n",tabla.cabecera->name);
	}while(muerto);
	return status;
}
/**/
char buscarSimbolo(int i,global tabla){
		bool _is_PID(t_player*personaje) {
			if(personaje->pid==i)return true;
			return false;
		}
		t_player*jugador;
		jugador=list_find(tabla.ready,(void*)_is_PID);
		if(jugador==NULL){
			jugador=list_find(tabla.sleeps,(void*)_is_PID);
			if(jugador==NULL){
				jugador=list_find(tabla.deads,(void*)_is_PID);
				if(jugador==NULL){
					if(tabla.exe->player!=NULL)jugador=tabla.exe->player;
					else {
						puts("NO SE ENCONTRO A NADIE");
						printf("Sym:X--%s\n",tabla.cabecera->name);
						return 'X';
					}
				}
			}
		}
		if(jugador==NULL){
			puts("NO SE ENCONTRO A NADIE");
			printf("Sym:X--%s\n",tabla.cabecera->name);
			return 'X';
		}
		printf("Sym:%c--%s\n",jugador->sym,tabla.cabecera->name);
		return jugador->sym;
	}
/**/
void enviarLog(int sock,global tabla,int msg,int cont,char data,char sym){
	printf("EnviarLog.I--%s\n",tabla.cabecera->name);
	char mensaje[256],valor[16];
	strcpy(mensaje,"Nº:S--Destino:(Sock/Sym)-Contenido(Msj/Cont/Dat/Sym)--Duenio:  --(");
	itoa(sock,valor,10);
	strcat(mensaje,valor);
	strcat(mensaje,"/");
	if(sock==tabla.cabecera->nid)strcat(mensaje,tabla.cabecera->name);
		else{
			valor[0]=buscarSimbolo(sock,tabla);
			valor[1]='\0';
			strcat(mensaje,valor);
		}
	strcat(mensaje,")--(");
	itoa(msg,valor,10);
	strcat(mensaje,valor);
	strcat(mensaje,"/");
	itoa(cont,valor,10);
	strcat(mensaje,valor);
	strcat(mensaje,"/");
	valor[0]=data;
	valor[1]='/';
	valor[2]=sym;
	valor[3]='\0';
	strcat(mensaje,valor);
	strcat(mensaje,")--Duenio:Plani");
	log_trace(tabla.logging.trace,mensaje,"TRACE");
	printf("\t\tEnviarLog.F--%s\n",tabla.cabecera->name);
}
void recibirLog(global tabla,int sock,answer temp){
	printf("RecibirLog.I--%s\n",tabla.cabecera->name);
	char mensaje[256],valor[16];
	strcpy(mensaje,"Nº:R--Socket:(Esp./Recv.)-Msj:(Esp./Recv.)-(Msj/Cont/Dat/Sym)--Duenio:  --(");
	itoa(sock,valor,10);
	strcat(mensaje,valor);
	strcat(mensaje,"/");
	strcat(mensaje,valor);
	strcat(mensaje,")--(ALL/");
	itoa(temp.msg,valor,10);
	strcat(mensaje,valor);
	strcat(mensaje,")--(");
	strcat(mensaje,valor);
	strcat(mensaje,"/");
	itoa(temp.cont,valor,10);
	strcat(mensaje,valor);
	strcat(mensaje,"/");
	valor[0]=temp.data;
	valor[1]='/';
	valor[2]=temp.symbol;
	valor[3]='\0';
	strcat(mensaje,valor);
	strcat(mensaje,")--Duenio:");
	if(sock==tabla.cabecera->nid)strcat(mensaje,tabla.cabecera->name);
	else{
		valor[0]=buscarSimbolo(sock,tabla);
		valor[1]='\0';
		strcat(mensaje,valor);
	}
	log_trace(tabla.logging.trace,mensaje,"TRACE");
	printf("\t\tRecibirLog.F--%s\n",tabla.cabecera->name);
}
/**/
int selectGRID_planificador(int fdmax,fd_set*original){
	struct timeval intervalo;
	intervalo.tv_usec = 100000;
	intervalo.tv_sec = 0;
	return select(fdmax +1,original,NULL,NULL,&intervalo);
};
/**/
void modificarAlgoritmo(answer temp,global general){
	printf("\t\t\t\tModificarAlgo.I--%s\n",general.cabecera->name);
	general.algo->algo=temp.cont;
	char mensaje[128],numero[16];
	strcpy(mensaje,"Nuevo Algoritmo--");
	if(general.algo->algo==0){
		strcat(mensaje,"SRDF--");
		puts("Se ha elegido usar el Algoritmo SRDF.");
	}
	else {
		strcat(mensaje,"RR-Q=");
		itoa(general.algo->algo,numero,10);
		strcat(mensaje,numero);
		strcat(mensaje,"--");
		printf("Se ha elegido usar el Algoritmo Round Robins Q==%d\n",general.algo->algo);
	}
	if(defaultRD == -1){
		general.algo->remainDist=(temp.data-'0')*10;
		printf("El Remaining Distance ahora es de: %d\n\n",general.algo->remainDist);
		strcat(mensaje,"\t--Remaining Distance: ");
		itoa(general.algo->remainDist,numero,10);
		strcat(mensaje,numero);
		strcat(mensaje,"--MODIFICADO");
	}else{
	  	general.algo->remainDist = defaultRD;
		strcat(mensaje,"\t--Remaining Distance: ");
		itoa(general.algo->remainDist,numero,10);
		strcat(mensaje,numero);
		strcat(mensaje,"--SIN MODIFICAR");
	}
	log_info(general.logging.info,mensaje,"WARNING");
	printf("\t\t\t\tModificarAlgo.F--%s\n",general.cabecera->name);
}
void modificarRetardo(answer temp,global general){
	printf("\t\t\t\tModificarRetardo.I--%s\n",general.cabecera->name);
	general.algo->retardo=temp.cont;
	printf("El Retardo entre turnos ahora es de: %d\n\n",general.algo->retardo);
	char mensaje[64],numero[12];
	strcpy(mensaje,"Nuevo retardo entre turnos: ");
	itoa(general.algo->retardo,numero,10);
	strcat(mensaje,numero);
	strcat(mensaje," mSeg--");
	log_info(general.logging.info,mensaje,"WARNING");
	printf("\t\t\t\tModificarRetardo.I--%s\n",general.cabecera->name);
}
void inicializar(nodoNivel*raiz,global*general){
	int estado;
	answer tempo;
	FD_SET(raiz->nid,general->original->original);
	if(raiz->nid>*(general->maxfd))*(general->maxfd)=raiz->nid;
	do{
	puts("\nPidiendo algoritmo.");
	sendAnswer(6,0,' ',' ',(short)raiz->nid);
	enviarLog(raiz->nid,*general,6,0,' ',' ');
	estado=recvAnswer(&tempo,(short)raiz->nid);
	recibirLog(*general,raiz->nid,tempo);
	if(estado!=6) puts("El nivel flasheo cualquiera!");
	}while(estado!=6);
	modificarAlgoritmo(tempo,*general);
	do{
	puts("\nPidiendo retardo entre turnos.");
	sendAnswer(4,0,' ',' ',(short)raiz->nid);
	enviarLog(raiz->nid,*general,4,0,' ',' ');
	estado=recvAnswer(&tempo,(short)raiz->nid);
	recibirLog(*general,raiz->nid,tempo);
	if(estado!=4) puts("El nivel flasheo cualquiera!");
	}while(estado!=4);
	modificarRetardo(tempo,*general);
}
/**/
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
	printf("NOVEDAD.I--%s\n",tanda->cabecera->name);
	if (tanda->cabecera->tandaRaiz->pid==0)return 0;
	else{
		int respuesta;
		t_player*temp;
		puts("Se ha conectado un jugador!!");
		puts("Avisandole al nivel..");
		sendAnswer(7,0,' ',tanda->cabecera->tandaRaiz->sym,(short)tanda->cabecera->nid);	//Le aviso al nivel que hay un nuevo jugador.
		enviarLog(tanda->cabecera->nid,*tanda,7,0,' ',tanda->cabecera->tandaRaiz->sym);
		char mensaje[128],numero[16];
		strcpy(mensaje,"Salida");
		strcat(mensaje,"\t--Msg:7--Cont:0--Data:' '--Sym:");
		numero[0]=tanda->cabecera->tandaRaiz->sym;
		numero[1]='\0';
		strcat(mensaje,numero);
		strcat(mensaje,"--\t --Cliente:");
		strcat(mensaje,tanda->cabecera->name);
		log_info(tanda->logging.info,mensaje,"INFO");
		printf("NOVEDAD.MID.A.I--%s\n",tanda->cabecera->name);
		respuesta=selectear(NULL,1,tanda->cabecera->nid,*tanda);	//Selecteo hasta que el nivel me responda 1 (-1 Siempre es una opcion de respuesta).
		printf("NOVEDAD.MID.A.F--%s\n",tanda->cabecera->name);
		switch (respuesta){
			case 1:puts("--El nivel ha dado el ok.--");
			puts("Cargando jugador a la base de datos..");
			temp=malloc(sizeof(t_player));
			crearStruct(tanda->cabecera,temp,tanda->algo->remainDist);
			cargarAlFinal(temp,tanda->ready,tanda->algo->algo); //Carga al final y reordena si es necesario.
			borrarNodo(tanda->cabecera);
			FD_SET(temp->pid,tanda->original->original);
			if(temp->pid>*(tanda->maxfd))*(tanda->maxfd)=temp->pid;
			tanda->cabecera->cantJugadores++;		//Si el nivel da el ok, entonces aumento la cantidad de jugadores activos.
			sendAnswer(1,0,' ',' ',temp->pid);
			puts("\nLa lista hasta ahora a quedado asi:");
			int i=0;
			void _each_Personaje(void*jug){
				t_player* jugador;
				jugador=(t_player*)jug;
				printf("El jugador Nº %d ",i);
				printf("es: %c\n",jugador->sym);
				i++;
			}
			list_iterate(tanda->ready,_each_Personaje);		//Muestra por pantalla como esta la lista.
			puts("");
			char mensaje[64],numero[16];
			itoa(temp->pid,numero,10);
			strcpy(mensaje,"Nuevo Jugador--Socket Nº:");
			strcat(mensaje,numero);
			strcat(mensaje,"--Simbolo:");
			numero[0]=temp->sym;
			numero[1]='\0';
			strcat(mensaje,numero);
			strcat(mensaje,".");
			log_info(tanda->logging.info,mensaje,"INFO");
			break;
			case -1:puts("--ERROR: El nivel comenta que hubo un error.--");
			sendAnswer(-1,0,' ',' ',tanda->cabecera->tandaRaiz->pid);
			borrarNodo(tanda->cabecera);
			strcpy(mensaje,"Jugador rechazado:");
			numero[0]=tanda->cabecera->tandaRaiz->sym;
			numero[1]='\0';
			strcat(mensaje,numero);
			itoa(tanda->cabecera->tandaRaiz->pid,numero,10);
			strcat(mensaje,"--Socekt Nº:");
			strcat(mensaje,numero);
			strcat(mensaje,".");
			log_info(tanda->logging.info,mensaje,"WARNING");
			break;
			case -2:return -2;
			break;
			case -3:return 1;
			break;
		}
	}
	printf("\t\tNOVEDAD.F--%s\n",tanda->cabecera->name);
	return 1;
}
/**/
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
		if(!list_is_empty(ready))list_sort(ready,comparator);
	}
}
void cargarAlFinal(t_player*temp,t_list*ready,int RR){
	list_add(ready, (void*)temp);
	reordenar(ready,RR);
}
/**/
t_player* buscarDormido(int sock,char sym,t_list* sleeps){
	//if(list_size(sleeps)>0){
		t_player*aux;
		int i;
		i=list_size(sleeps)-1;
		while(i>=0){
				aux=list_remove(sleeps,i);
				if(sock==aux->pid&&sym==' '){puts("LO ENCONTRAMOS!!");return aux;}
				else if(sym==aux->sym&&sock==0){puts("LO ENCONTRAMOS!!");return aux;}
					else list_add(sleeps,(void*)aux);
				i--;
			}
		if(i==-1)puts("Ni aun aca lo encontramos!!");
	//}
	return NULL;
}
/**/
int modoDeRecuperacion(global tabla){
	int status;
	puts("El nivel se ha caido, limpiando registros..");
	log_debug(tabla.logging.debug,"Nivel desconectado, aguardando reconexion.","DEBUG");
	loggearListas(tabla);
	FD_CLR(tabla.cabecera->nid,tabla.original->original);
	tabla.cabecera->nid=0;
	list_clean(tabla.recur);
	puts("Esperando por la reconexion.");
	int i,cont=0;
	do{
		for(i=0;i<5;i++){
			sleep(1);
			if(finalizar){
				puts("Abortando intento de reconexion.");
				aLaMierdaConTodo(tabla);
				return -2;
			}
		}
		puts("Intentando establecer conexion.");
		if(cont==5 || finalizar){
			puts("Abortando intento de reconexion.");
			aLaMierdaConTodo(tabla);
			printf("RECUPERACION.F--%s\n",tabla.cabecera->name);
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
		enviarLog(tabla.cabecera->nid,tabla,7,0,' ',jugador->sym);
		usleep(20000);
		recvAnswer(&temp,nid);
		recibirLog(tabla,tabla.cabecera->nid,temp);
		sendAnswer(3,jugador->data.pos,' ',jugador->sym,nid);
		enviarLog(tabla.cabecera->nid,tabla,3,jugador->data.pos,' ',jugador->sym);
		usleep(30000);
		recvAnswer(&temp,nid);
		recibirLog(tabla,tabla.cabecera->nid,temp);
		void _Pedir_El_Recurso(void*package){
			t_stack*recurso;
			recurso=(t_stack*)package;
			sendAnswer(2,1,recurso->recurso,jugador->sym,nid);
			enviarLog(tabla.cabecera->nid,tabla,2,1,recurso->recurso,jugador->sym);
			usleep(50000);
			recvAnswer(&temp,nid);
			recibirLog(tabla,tabla.cabecera->nid,temp);
		}
		if(!list_is_empty(jugador->t_stack))list_iterate(jugador->t_stack,_Pedir_El_Recurso);

	}
	printf("RECUPERACION.I--%s\n",tabla.cabecera->name);
	puts("El nivel se ha reconectado, reestableciendo la informacion..");
	log_trace(tabla.logging.trace,"\t\t\t--RECONEXION--\t\t\t","TRACE");
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
	usleep(300000);
	log_debug(tabla.logging.debug,"Nivel reconectado.","DEBUG");
	log_info(tabla.logging.info,"Nivel reconectado.","INFO");
	printf("RECUPERACION.F--%s\n",tabla.cabecera->name);
	return status;
}
int aLaMierdaConTodo(global tabla){
	printf("CIERRE.I--%s\n",tabla.cabecera->name);
	t_player*temp;
	t_stack*tempstack;
	nuevo*aux;

	puts("Eliminando jugadores activos.");
	while(!list_is_empty(tabla.ready)){
		usleep(200000);
		temp=(t_player*)list_remove(tabla.ready,0);
		while (!list_is_empty(temp->t_stack)){
			puts("Recurso eliminado.");
			tempstack=(t_stack*)list_remove(temp->t_stack,0);
			free(tempstack);
		}
		sendAnswer(0,0,' ',' ',temp->pid);
		enviarLog(temp->pid,tabla,0,0,' ',' ');
		close(temp->pid);
		free(temp->t_stack);
		free(temp);
	}
	free(tabla.ready);

	usleep(200000);
	puts("Eliminando jugadores dormidos.");
	while(!list_is_empty(tabla.sleeps)){
		temp=(t_player*)list_remove(tabla.sleeps,0);
		while (!list_is_empty(temp->t_stack)){
			puts("Recurso eliminado.");
			tempstack=(t_stack*)list_remove(temp->t_stack,0);
			free(tempstack);
		}
		sendAnswer(0,0,' ',' ',temp->pid);
		enviarLog(temp->pid,tabla,0,0,' ',' ');
		close(temp->pid);
		free(temp->t_stack);
		free(temp);
	}
	free(tabla.sleeps);

	usleep(200000);
	puts("Eliminando jugador en ejecucion.");
	if(tabla.exe->player!=NULL){
		sendAnswer(0,0,' ',' ',tabla.exe->player->pid);
		enviarLog(tabla.exe->player->pid,tabla,0,0,' ',' ');
		close(tabla.exe->player->pid);
		while (!list_is_empty(tabla.exe->player->t_stack)){
			puts("Recurso eliminado.");
			tempstack=(t_stack*)list_remove(tabla.exe->player->t_stack,0);
			free(tempstack);
		}
		free(tabla.exe->player->t_stack);
		free(tabla.exe->player);
	}
	usleep(200000);
	puts("Eliminando jugadores nuevos.");
	while(tabla.cabecera->tandaRaiz!=NULL){
		aux=tabla.cabecera->tandaRaiz;
		tabla.cabecera->tandaRaiz=tabla.cabecera->tandaRaiz->sgte;
		sendAnswer(0,0,' ',' ',aux->pid);
		close(aux->pid);
		free(aux);
	}

	usleep(200000);
	puts("Eliminando recursos.");
	while (!list_is_empty(tabla.recur)){
		tempstack=(t_stack*)list_remove(tabla.recur,0);
		free(tempstack);
	}
	free(tabla.recur);
	free(tabla.algo);
	sendAnswer(0,0,' ',' ',tabla.cabecera->nid);
	enviarLog(tabla.cabecera->nid,tabla,0,0,' ',' ');
	close(tabla.cabecera->nid);
	usleep(400000);
	//printf("Mi ID-Hilo es: %d",(int)tabla.cabecera->idHilo);
	//pthread_cancel(tabla.cabecera->idHilo);
	puts("Nos Vamos todos al carajo!");
	log_info(tabla.logging.info,"Nivel terminado por desconexion.","ERROR");
	printf("CIERRE.F--%s\n",tabla.cabecera->name);
	return -2;
}
bool muertePersonaje(int i,global tabla){
	printf("MUERTE.I--%s\n",tabla.cabecera->name);
	bool chosen=false;
	bool _is_PID(t_player*jugador) {
		    if(i==jugador->pid)return true;
		    return false;
			}
	t_player*aux ;

	aux=list_remove_by_condition(tabla.deads,(void*)_is_PID);
		if(aux==NULL)	{
			printf("MUERTE.MID--No_Estaba en Muertos--%s\n",tabla.cabecera->name);
			aux=list_remove_by_condition(tabla.ready,(void*)_is_PID);
		}
		if(aux==NULL) 	aux=list_remove_by_condition(tabla.sleeps,(void*)_is_PID);
		if(aux==NULL) 	aux=buscarDormido(i,' ',tabla.sleeps);
		if(aux==NULL){
			if(tabla.exe->player==NULL){
				printf("\t\t\t\tMUERTE.F--No_Estaba--%s\n",tabla.cabecera->name);
				return false;
			}else{
				if(tabla.exe->player->pid!=i){
					printf("\t\t\t\tMUERTE.F--No_Estaba--%s\n",tabla.cabecera->name);
					return false;
				}else aux=tabla.exe->player;
			}
		}
	char mensaje[128],numero[16],*string;
	strcpy(mensaje,"Personaje desconectado:");
	string=ctos(aux->sym);
	strcat(mensaje,string);
	free(string);
	strcat(mensaje,"\t-Socket Nº:");
	itoa(aux->pid,numero,10);
	strcat(mensaje,numero);
	strcat(mensaje,"\t-Recursos liberados: -");
	FD_CLR(aux->pid,tabla.original->original);
	while (!list_is_empty(aux->t_stack)){
		t_stack*tempstack;
		tempstack=(t_stack*)list_remove(aux->t_stack,0);
		numero[0]=tempstack->recurso;
		numero[1]='-';
		numero[2]='\0';
		strcat(mensaje,numero);
		//list_add(tabla.recur, (void*)tempstack);
	}
	tabla.cabecera->cantJugadores--;
	if(tabla.cabecera->cantJugadores==0)printf("--ATENCION--No quedan jugadores!!\n");
	sendAnswer(8,0,0,aux->sym,tabla.cabecera->nid);
	enviarLog(tabla.cabecera->nid,tabla,8,0,'Z',aux->sym);
	if(tabla.exe->player!=NULL){
		if(aux->pid==tabla.exe->player->pid){
			tabla.exe->player=NULL;
			chosen =true;
		}
	}
	log_info(tabla.logging.info,mensaje,"WARNING");
	//puts("Personaje Completamente eliminado!!");
	printf("\t\tMUERTE.F--%d--%s\n",aux->pid,tabla.cabecera->name);
	FD_CLR(aux->pid,tabla.original->original);
	close(aux->pid);
	free(aux);
	loggearListas(tabla);
	return chosen;
}/**/
void matarPersonaje(answer auxiliar,global tabla){
	char simbolo;
	simbolo=auxiliar.symbol;
	printf("MATAR.I--%s\n",tabla.cabecera->name);
	bool _is_Personaje(t_player*jugador) {
		    if(jugador->sym==simbolo)return true;
		    return false;
			}
	t_player*aux;
	aux=list_remove_by_condition(tabla.ready,(void*)_is_Personaje);
	if(aux==NULL){
		aux=list_remove_by_condition(tabla.sleeps,(void*)_is_Personaje);
		//if(aux==NULL)aux=buscarDormido(0,simbolo,tabla.sleeps);
		if(aux==NULL){
			if(tabla.exe->player!=NULL){
				aux=tabla.exe->player;
				tabla.exe->player=NULL;
			}else{
				puts("\t\t\t\tNO ENCONTRADO!!!");
				printf("\t\tMATAR.F--%s\n",tabla.cabecera->name);
				return;
			}
		}
	}
	char mensaje[64],dato[3];
	//puts("Avisandole sobre la situacion al pobre..");
	strcpy(mensaje,"Muerte de un personaje:-");
	dato[0]=aux->sym;
	dato[1]='\0';
	dato[2]='-';
	strcat(mensaje,dato);
	log_info(tabla.logging.info,mensaje,"INFO");
	list_add(tabla.deads,(void*)aux);
	sendAnswer(8,auxiliar.cont,' ',' ',aux->pid);
	enviarLog(aux->pid,tabla,8,0,'Z','Z');
	printf("\t\tMATAR.F--%d--%s\n",aux->pid,tabla.cabecera->name);
	loggearListas(tabla);
}
int interrupcion(int i,short respuesta,answer* aux,global tabla){
	puts("\n--SLI--");
	int status=-1;
	char mensaje[128],numero[8];
	strcpy(mensaje,"Interrupcion generada por ");
	if(i==tabla.cabecera->nid){
		strcat(mensaje,tabla.cabecera->name);
		puts("La interrupcion no se puede enmascarar, atendiendo..");
		switch(respuesta){
		case 0:status=modoDeRecuperacion(tabla);
		break;
		//case 2:asignarRecurso(tabla,aux);//CREARLA FUNCION!!!
		//break;
		case 4:modificarRetardo(*aux,tabla);
		break;
		case 6:modificarAlgoritmo(*aux,tabla);
		break;
		case 8:matarPersonaje(*aux,tabla);
		status=-4;
		break;
		default:puts("Ni la interrupcion se puede atender!!");
		break;
		}
	}else{
		strcat(mensaje,"Pers.Nº:");
		itoa(i,numero,10);
		strcat(mensaje,numero);
		if(respuesta!=0)puts("Pescado Enmascarado");
		else{
			if(muertePersonaje(i,tabla))status=0;
			else status=-5;
		}
	}
	strcat(mensaje,"\t\t--msg:");
	itoa((int)aux->msg,numero,10);
	strcat(mensaje,numero);
	strcat(mensaje,"\tcont:");
	itoa((int)aux->cont,numero,10);
	strcat(mensaje,numero);
	strcat(mensaje,"\tdata:");
	numero[0]=aux->data;
	numero[1]='\0';
	strcat(mensaje,numero);
	strcat(mensaje,"\tsym:");
	numero[0]=aux->symbol;
	numero[1]='\0';
	strcat(mensaje,numero);
	strcat(mensaje,"--");
	log_info(tabla.logging.info,mensaje,"WARNING");
	puts("--CLI--\n");
	return status;
}
/**/
int selectear(answer*tempo,short esperado,int sock,global tabla){
	printf("\t\tSELECTEAR.I--%s\n",tabla.cabecera->name);
	answer aux;
	short respuesta;
	fd_set readfds;
	int status,cantidad,fdmax;
	char mensaje[128],mensajeError[64],numero[16],simbolo;
	char data[2];
	static int cantSelecteos=0;
	do{
		fd_set original=*(tabla.original->original);
		fdmax=*(tabla.maxfd);
		itoa(cantSelecteos,numero,10);
		readfds=original;
		strcpy(mensaje,"Nº:");
		strcat(mensaje,numero);
		//printf("Selecteando..ID/ID: %d/",sock);
		itoa(sock,numero,10);
		strcat(mensaje,"--Socket:(Esp./Recv.)-Msj:(Esp./Recv.)-(Msj/Cont/Dat/Sym)--Duenio:  --(");
		strcat(mensaje,numero);
		strcat(mensaje,"/");
		printf("\t\tSELECTEAR.MID.A.I--%s\n",tabla.cabecera->name);
		cantidad=selectGRID(fdmax,&readfds);
		int i=0;
		while ((!FD_ISSET(i,&readfds))&&(i<=fdmax))	{
			i++;
			if(cantidad>1&&i==sock)i++;
		}
		printf("\t\tSELECTEAR.MID.A.F--%s\n",tabla.cabecera->name);
		itoa(i,numero,10);
		strcat(mensaje,numero);
		strcat(mensaje,")--(");
		if (i>fdmax){
			puts("--ERROR:No se encontro candidato para selectear!!--");
			itoa(cantSelecteos,numero,10);
			strcpy(mensajeError,"Nº:");
			strcat(mensajeError,numero);
			strcat(mensajeError,"-ERROR:No se encontro candidato para selectear!!");
			log_info(tabla.logging.info,mensaje,"ERROR");
			exit(1);
		}else{
			//printf("%d",i);
			printf("\t\tSELECTEAR.MID.B.I--%s\n",tabla.cabecera->name);
			respuesta=recvAnswer(&aux,i);
			printf("\t\tSELECTEAR.MID.B.F--%s\n",tabla.cabecera->name);
			if (esperado==10){
				//printf("--EspMsg:X");
				strcat(mensaje,"ALL/");
			}else {
				//printf("--EspMsg:%d",(int)esperado);
				itoa((int)esperado,numero,10);
				strcat(mensaje,numero);
				strcat(mensaje,"/");
			}
			//printf("--RecvMsg:%d-Cnt:%d-Dta:%c-",(int)aux.msg,(int)aux.cont,aux.data);
			itoa((int)aux.msg,numero,10);
			strcat(mensaje,numero);
			strcat(mensaje,")--(");
			strcat(mensaje,numero);
			itoa((int)aux.cont,numero,10);
			strcat(mensaje,"/");
			strcat(mensaje,numero);
			strcat(mensaje,"/");
			data[0]=aux.data;
			data[1]='\0';
			strcat(mensaje,data);
			strcat(mensaje,"/");
			data[0]=aux.symbol;
			data[1]='\0';
			strcat(mensaje,data);
			strcat(mensaje,")--Duenio:");

			if(i==tabla.cabecera->nid){
				//printf("==>NID\n");
				strcat(mensaje,tabla.cabecera->name);
			}
			else {
				simbolo=buscarSimbolo(i,tabla);
				//printf("=> %c\n",simbolo);
				data[0]=simbolo;
				data[1]='\0';
				strcat(mensaje,data);
			}
			strcat(mensaje,".");
			log_trace(tabla.logging.trace,mensaje,"TRACE");
			if (i==sock&&respuesta!=0){
				if((respuesta==esperado)||(respuesta==-1)||(esperado==10)){
					if (tempo!=NULL)*tempo=aux;
					cantSelecteos++;
					log_trace(tabla.logging.trace,"\t\t\t--------A--------------------\t\t\t","TRACE");
					printf("\t\tSELECTEAR.F--%s\n",tabla.cabecera->name);
					return respuesta;
				}else status=interrupcion(i,respuesta,&aux,tabla);
				if (status==0){
					cantSelecteos++;
					log_trace(tabla.logging.trace,"\t\t\t--------B--------------------\t\t\t","TRACE");
					break;
				}
			}else{
				status=interrupcion(i,respuesta,&aux,tabla);
				if (status==0&&i==sock)break;
				if(status>0&&i==sock){
					answer temp;
					if(tabla.playing){
						if(tabla.exe->player->data.recsol!=' '){
							sendAnswer(2,0,tabla.exe->player->data.recsol,tabla.exe->player->sym,tabla.cabecera->nid);
							enviarLog(tabla.cabecera->nid,tabla,2,0,tabla.exe->player->data.recsol,tabla.exe->player->sym);
							usleep(30000);
							recvAnswer(&temp,tabla.cabecera->nid);
							recibirLog(tabla,tabla.cabecera->nid,temp);
							if(calcularDistancia(tabla.exe->player->data.pos,temp.cont)!=tabla.exe->player->data.dist){
								sendAnswer(2,temp.cont,tabla.exe->player->data.recsol,' ',tabla.exe->player->pid);
								enviarLog(tabla.exe->player->pid,tabla,2,temp.cont,tabla.exe->player->data.recsol,' ');
								tabla.exe->player->data.dist=calcularDistancia(tabla.exe->player->data.pos,temp.cont);
							}else {
								sendAnswer(1,0,' ',' ',tabla.exe->player->pid);
								enviarLog(tabla.exe->player->pid,tabla,1,0,' ',' ');
							}
						}
					}
				printf("\t\tSELECTEAR.F--%s\n",tabla.cabecera->name);
				return -3;
				}
			}
			if (status==-2)break;

		}
	}while(1);
	printf("\t\tSELECTEAR.F--%s\n",tabla.cabecera->name);
	return status;
}
/**/
void darInstancia(t_player*jugador,t_stack*instancia,global*tabla){
	char mensaje[128],numero[8],*string;
	strcpy(mensaje,"Instancia concedida:");
	string=ctos(instancia->recurso);
	strcat(mensaje,string);
	free(string);
	strcat(mensaje,"\t\tJugador Nº:");
	itoa(jugador->pid,numero,10);
	strcat(mensaje,numero);
	strcat(mensaje,"\tSimbolo:");
	string=ctos(jugador->sym);
	strcat(mensaje,string);
	free(string);
	strcat(mensaje,".");
	list_add(jugador->t_stack,(void*)instancia);
	jugador->data.recsol=' ';
	jugador->data.dist=tabla->algo->remainDist;


	//list_add(tabla->ready,(void*)jugador);
	log_info(tabla->logging.info,mensaje,"INFO");
	//sendAnswer(1,0,' ',' ',jugador->pid);

}
int asignarRecursos(global*tabla){
	printf("ASIGNAR.I--%s\n",tabla->cabecera->name);
	short respuesta;
	answer temp;
	int status=1;
	//---------------------------------------------------------------
	bool intentarAsignar(t_player*jugador){
		char mensaje[128],letra[8];
		if(jugador!=NULL){
			if((jugador->data.recsol>='A')&(jugador->data.recsol<='Z')){
				//printf("Pidiendole recurso al nivel: ");
				//printf("Rec:%c--",jugador->data.recsol);
				//printf("Pers:%c\n",jugador->sym);
				strcpy(mensaje,"Pidiendo recurso: ");
				letra[0]=jugador->data.recsol;
				letra[1]='\0';
				strcat(mensaje,letra);
				strcat(mensaje,"\tPara:");
				letra[0]=jugador->sym;
				letra[1]='\0';
				strcat(mensaje,letra);
				log_info(tabla->logging.info,mensaje,"INFO");
				sendAnswer(2,1,jugador->data.recsol,jugador->sym,tabla->cabecera->nid);
				enviarLog(tabla->cabecera->nid,*tabla,2,1,jugador->data.recsol,jugador->sym);
				//puts("Entrando a selectear.");
				respuesta=selectear(&temp,1,tabla->cabecera->nid,*tabla);
				if (respuesta==-1)return false;
				if (respuesta==-2){status=-2;return false;}
			}else return false;
		}else return false;
		if(jugador!=NULL){
			t_stack*recnuevo;
			recnuevo=(t_stack*)malloc(sizeof(t_stack));
			recnuevo->recurso=jugador->data.recsol;
			darInstancia(jugador,recnuevo,tabla);
			//puts("Instancia concedida.");x
			return true;
		}
		return false;
	}
	//---------------------------------------------------------------
	int i=list_size(tabla->sleeps)-1;
	t_player*auxiliar;
	char carater;
	while(i>=0){
		auxiliar=list_get(tabla->sleeps,i);
		carater=auxiliar->sym;
		bool _is_SYM(t_player*jugador) {
				    if(carater==jugador->sym)return true;
				    return false;
					}
		if(intentarAsignar(auxiliar)){
			auxiliar=list_remove_by_condition(tabla->sleeps,(void*)_is_SYM);
			if(auxiliar!=NULL){
				list_add(tabla->ready,(void*)auxiliar);
				sendAnswer(1,0,' ',' ',auxiliar->pid);
			}
		}else {
			//if(auxiliar!=NULL) list_add(tabla->sleeps,(void*)auxiliar);
		}
		i--;
	}


	//list_iterate(tabla->sleeps,(void*)intentarAsignar);
	printf("ASIGNAR.F--%s\n",tabla->cabecera->name);
	if(status==-2)return -2;
	return 0;
}

/*int devolverRecursos(global*tabla){
	int status;
	if(!list_is_empty(tabla->recur)){
		t_stack*tempstack;
		sendAnswer(5,0,' ',' ',tabla->cabecera->nid);
		enviarLog(tabla->cabecera->nid,*tabla,5,0,' ',' ');
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
				enviarLog(tabla->cabecera->nid,*tabla,2,0,tempstack->recurso,' ');
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
				usleep(300000);
			}
			sendAnswer(5,0,' ',' ',tabla->cabecera->nid);
			break;
		}
	}
	return 0;
}*/
/**/
void movimiento(global*tabla,answer aux){
	printf("MOVIMIENTO.I--%s\n",tabla->cabecera->name);
	tabla->playing=true;
	sendAnswer(3,aux.cont,' ',aux.symbol,tabla->cabecera->nid);
	enviarLog(tabla->cabecera->nid,*tabla,3,aux.cont,' ',aux.symbol);
	tabla->exe->player->data.pos=aux.cont;
	tabla->exe->player->data.dist--;
	if(selectear(&aux,1,tabla->cabecera->nid,*tabla)==-3)return;
	if (tabla->exe->player!=NULL){
	sendAnswer(1,0,' ',' ',tabla->exe->player->pid);
		tabla->playing=false;
		if(tabla->exe->rem_cuantum!=0){
			if(tabla->exe->rem_cuantum==1)tabla->exe->rem_cuantum=-1;
			else tabla->exe->rem_cuantum--;
		}
	}printf("\t\tMOVIMIENTO.F--%s\n",tabla->cabecera->name);
}
void dormirJugador(t_player*jugador,t_list*dormidos){
	list_add(dormidos,(void*)jugador);
}
void instancia(global tabla){
	char mensaje[64],numero[8];
	strcpy(mensaje,"Dormir jugador Nº:");
	itoa(tabla.exe->player->pid,numero,10);
	strcat(mensaje,numero);
	strcat(mensaje,"\tSimbolo:");
	numero[0]=tabla.exe->player->sym;
	numero[1]='\0';
	strcat(mensaje,numero);
	strcat(mensaje,"\t--Peticion de instancia:");
	numero[0]=tabla.exe->player->data.recsol;
	strcat(mensaje,numero);
	strcat(mensaje,"--");
	log_info(tabla.logging.info,mensaje,"INFO");
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
	enviarLog(tabla.cabecera->nid,tabla,2,0,aux.data,aux.symbol);
	if(selectear(&aux,2,tabla.cabecera->nid,tabla)==-3)return;
	if (tabla.exe->player!=NULL){
		sendAnswer(2,aux.cont,aux.symbol,' ',tabla.exe->player->pid);
		tabla.playing=false;
		tabla.exe->player->data.dist=calcularDistancia(tabla.exe->player->data.pos,aux.cont);
	}
}/**/
void recurso(global*tabla,answer aux){
	if(aux.cont==0) ubicacion(aux,*tabla);
	if(aux.cont==1) instancia(*tabla);
}
void cargarAExec(global*tabla){
	tabla->exe->player=(t_player*)list_remove(tabla->ready,0);
	tabla->exe->rem_cuantum=tabla->algo->algo;
}
bool concederTurno(global*tabla){
	printf("CONCEDER.I--%s\n",tabla->cabecera->name);
	if(tabla->exe->player!=NULL){
		if(tabla->exe->rem_cuantum==-1){
			cargarAlFinal(tabla->exe->player,tabla->ready,tabla->algo->algo);
			tabla->exe->player=NULL;
		}
	}
	if (tabla->exe->player==NULL){
		if(!list_is_empty(tabla->ready)){
			printf("CONCEDER.CARGAR.I--%s\n",tabla->cabecera->name);
			cargarAExec(tabla);
			printf("CONCEDER.CARGAR.F--%s\n",tabla->cabecera->name);
		}else {
			printf("\t\tCONCEDER.F--X--%s\n",tabla->cabecera->name);
			return false;
		}
	}else{
		printf("CONCEDER.SIGA.I--%s\n",tabla->cabecera->name);
		if(tabla->exe->rem_cuantum==0&&tabla->algo->algo!=0)tabla->exe->rem_cuantum=tabla->algo->algo;
		if(tabla->exe->rem_cuantum!=0&&tabla->algo->algo==0)tabla->exe->rem_cuantum=tabla->algo->algo;
		printf("CONCEDER.SIGA.F--%s\n",tabla->cabecera->name);
	}
	sendAnswer(7,0,'.','.',(short)tabla->exe->player->pid);
	enviarLog(tabla->exe->player->pid,*tabla,7,0,' ',' ');
	printf("\t\tCONCEDER.F--%d--%s\n",tabla->exe->player->pid,tabla->cabecera->name);
	return true;
}
int atenderJugador(global*tabla){
	printf("TURNO.I--%s\n",tabla->cabecera->name);
	int respuesta,sock,cont=0;
	answer back;
	do{
		cont++;
		if(!concederTurno(tabla)){
			printf("\t\tTURNO.F--%s\n",tabla->cabecera->name);
			return 0;
		}
		if(cont==5){
			puts("ESTO NO ESTA RESULTANDO!!");
			exit(1);
		}
	}while(tabla->exe->player==NULL);
	sock=tabla->exe->player->pid;
	printf("\t\tTURNO.MID.I--%s\n",tabla->cabecera->name);
	respuesta=selectear(&back,10,sock,*tabla);
	printf("\t\tTURNO.MID.F--%s\n",tabla->cabecera->name);
	switch(respuesta){
	case -2:
		return -2;
		break;
	case 0:
		puts("Se murio el que estaba jugando, fin de turno.");
		break;
	case 2:
		recurso(tabla,back);
		break;
	case 3:
		movimiento(tabla,back);
		break;
	}
	printf("\t\tTURNO.F--%s\n",tabla->cabecera->name);
	return 0;
}
/**/
logs crearLogs(nodoNivel*raiz){
	char file[128],program_name[32];
	logs paquete;
	strcpy(file,LOCAL_LOG);
	strcat(file,"Plani_");
	strcat(file,raiz->name);
	strcat(file,"-Trace");
	strcat(file,".txt");
	strcpy(program_name,"PLANIFICADOR_");
	strcat(program_name,raiz->name);
	paquete.trace=log_create(file,program_name,muestreo,LOG_LEVEL_TRACE);
	strcpy(file,LOCAL_LOG);
	strcat(file,"Plani_");
	strcat(file,raiz->name);
	strcat(file,"-Debug");
	strcat(file,".txt");
	paquete.debug=log_create(file,program_name,muestreo,LOG_LEVEL_DEBUG);
	strcpy(file,LOCAL_LOG);
	strcat(file,"Plani_");
	strcat(file,raiz->name);
	strcat(file,"-Info");
	strcat(file,".txt");
	paquete.info=log_create(file,program_name,muestreo,LOG_LEVEL_INFO);
	/*strcpy(file,LOCAL_LOG);
	strcat(file,raiz->name);
	strcat(file,"-Warning");
	strcat(file,".txt");
	strcpy(program_name,"PLANIFICADOR_");
	strcat(program_name,raiz->name);
	paquete.warning=log_create(file,program_name,muestreo,LOG_LEVEL_WARNING);
	strcpy(file,LOCAL_LOG);
	strcat(file,raiz->name);
	strcat(file,"-Error");
	strcat(file,".txt");
	strcpy(program_name,"PLANIFICADOR_");
	strcat(program_name,raiz->name);
	paquete.error=log_create(file,program_name,muestreo,LOG_LEVEL_ERROR);*/
	return paquete;
}
void loggearActivos(global tabla){
	char mensaje[256],valor[16];
	static int contador=0;
	strcpy(mensaje,"Nº:");
	itoa(contador,valor,10);
	contador++;
	strcat(mensaje,valor);
	strcat(mensaje,"--");
	strcat(mensaje,"Estado_Activos(Sock./Jug./Rec.Sol./[-Rec.Obt.-]):");
	void _logRecurso(t_stack*recurso){
		valor[0]=recurso->recurso;
		valor[1]='-';
		valor[2]='\0';
		strcat(mensaje,valor);
	}
	void _logPersonaje(t_player*personaje){
		strcat(mensaje,"--(");
		itoa(personaje->pid,valor,10);
		strcat(mensaje,valor);
		valor[0]='/';
		valor[1]=personaje->sym;
		valor[2]='/';
		valor[3]=personaje->data.recsol;
		valor[4]='/';
		valor[5]='[';
		valor[6]='-';
		valor[7]='\0';
		strcat(mensaje,valor);
		list_iterate(personaje->t_stack,(void*)_logRecurso);
		strcat(mensaje,"])");
	}
	if (!list_is_empty(tabla.ready))list_iterate(tabla.ready,(void*)_logPersonaje);
	else strcat(mensaje,"--SIN JUGADORES EN LA LISTA--");
	log_debug(tabla.logging.debug,mensaje,"DEBUG");
}
void loggearDormidos(global tabla){
	char mensaje[128],valor[16];
	static int contador=0;
	strcpy(mensaje,"Nº:");
	itoa(contador,valor,10);
	contador++;
	strcat(mensaje,valor);
	strcat(mensaje,"--");
	strcat(mensaje,"Estado_Dormidos(Sock./Jug./Rec.Sol./[-Rec.Obt.-]): ");
	void _logRecurso(t_stack*recurso){
		valor[0]=recurso->recurso;
		valor[1]='-';
		valor[2]='\0';
		strcat(mensaje,valor);
	}
	void _logPersonaje(t_player*personaje){
		strcat(mensaje,"--(");
		itoa(personaje->pid,valor,10);
		strcat(mensaje,valor);
		valor[0]='/';
		valor[1]=personaje->sym;
		valor[2]='/';
		valor[3]=personaje->data.recsol;
		valor[4]='/';
		valor[5]='[';
		valor[6]='-';
		valor[7]='\0';
		strcat(mensaje,valor);
		list_iterate(personaje->t_stack,(void*)_logRecurso);
		strcat(mensaje,"])");
	}
	if (!list_is_empty(tabla.sleeps))list_iterate(tabla.sleeps,(void*)_logPersonaje);
	else strcat(mensaje,"--SIN JUGADORES EN LA LISTA--");
	log_debug(tabla.logging.debug,mensaje,"DEBUG");
}
void loggearMuertos(global tabla){
	char mensaje[128],valor[16];
	static int contador=0;
	strcpy(mensaje,"Nº:");
	itoa(contador,valor,10);
	contador++;
	strcat(mensaje,valor);
	strcat(mensaje,"--");
	strcat(mensaje,"Estado_Muertos(Sock./Jug./Rec.Sol./[-Rec.Obt.-]): ");
	void _logRecurso(t_stack*recurso){
		valor[0]=recurso->recurso;
		valor[1]='-';
		valor[2]='\0';
		strcat(mensaje,valor);
	}
	void _logPersonaje(t_player*personaje){
		strcat(mensaje,"--(");
		itoa(personaje->pid,valor,10);
		strcat(mensaje,valor);
		valor[0]='/';
		valor[1]=personaje->sym;
		valor[2]='/';
		valor[3]=personaje->data.recsol;
		valor[4]='/';
		valor[5]='[';
		valor[6]='-';
		valor[7]='\0';
		strcat(mensaje,valor);
		list_iterate(personaje->t_stack,(void*)_logRecurso);
		strcat(mensaje,"])");
	}
	if (!list_is_empty(tabla.deads))list_iterate(tabla.deads,(void*)_logPersonaje);
	else strcat(mensaje,"--SIN JUGADORES EN LA LISTA--");
	log_debug(tabla.logging.debug,mensaje,"DEBUG");
}
void loggearExec(global tabla){
	char mensaje[128],valor[16];
	static int contador=0;
	strcpy(mensaje,"Nº:");
	itoa(contador,valor,10);
	contador++;
	strcat(mensaje,valor);
	strcat(mensaje,"--");
	strcat(mensaje,"Estado_Ejecutando(Sock./Jug./Rec.Sol./[-Rec.Obt.-]): ");
	void _logRecurso(t_stack*recurso){
		valor[0]=recurso->recurso;
		valor[1]='-';
		valor[2]='\0';
		strcat(mensaje,valor);
	}
	if(tabla.exe->player!=NULL){
		strcat(mensaje,"--(");
		itoa(tabla.exe->player->pid,valor,10);
		strcat(mensaje,valor);
		valor[0]='/';
		valor[1]=tabla.exe->player->sym;
		valor[2]='/';
		valor[3]=tabla.exe->player->data.recsol;
		valor[4]='/';
		valor[5]='[';
		valor[6]='-';
		valor[7]='\0';
		strcat(mensaje,valor);
		list_iterate(tabla.exe->player->t_stack,(void*)_logRecurso);
		strcat(mensaje,"])");
	}else strcat(mensaje,"--SIN JUGADOR EN EJECUCION--");
	log_debug(tabla.logging.debug,mensaje,"DEBUG");
}
void loggearListas(global tabla){
	loggearActivos(tabla);
	loggearDormidos(tabla);
	loggearMuertos(tabla);
	loggearExec(tabla);
	log_debug(tabla.logging.debug,"\t\t\t---------------------------\t\t\t","DEBUG");
}
void cerrarLogging(global tabla){
	log_destroy(tabla.logging.debug);
	log_destroy(tabla.logging.trace);
	log_destroy(tabla.logging.info);
	/*log_destroy(tabla.logging.error);
	log_destroy(tabla.logging.warning);*/
}
