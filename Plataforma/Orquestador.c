/*
 * orquestador.c
 *
 *  Created on: 27/10/2013
 *      Author: EzequielAyzenberg
 */

#include"Orquestador.h"
//#include"Planificador.h"
#define PROGRAMA "ORQUESTADOR"
#define mostrarOrquestador false


t_list * ganadores;
extern char * CFG_PATH;
extern t_list *listaNiveles;
extern bool mpantalla, mtexto, pantallaTerminada;
logs logsOrquestador;
logs crearLogs_Orquestador(void);
void loggearEstado_Debug(void);
void cerrarLogs_Orquestador(logs tabla);
void nivelesCaidos(void);

void *orquestador(void* infoAux){

	cfgOrquestador cfg = cargarOrquestador(CFG_PATH);
	logsOrquestador = crearLogs_Orquestador();

	loguearDatosIniciales(cfg);
	if(!mpantalla)printf("--ORQUESTADOR-- IP: %s.\n", cfg.ip);
	if(!mpantalla)printf("--ORQUESTADOR-- Puerto: %d.\n", cfg.puerto);
	if(!mpantalla)printf("--ORQUESTADOR-- Path de koopa: %s.\n", cfg.koopa);
	if(!mpantalla)printf("--ORQUESTADOR-- Path del script: %s.\n", cfg.script);

	int socketOrquestador, socketIngresante;
	handshake nuevoHandshake;
	ganadores = list_create();
	t_list *hilosPlanificadores = list_create();
	socketOrquestador = listenGRID(cfg.puerto,cfg.ip);
	fd_set original_FD;
	FD_ZERO(&original_FD);
	FD_SET(socketOrquestador, &original_FD);

	while(!finalizar){
		if(0 == selectGRID_orquestador(socketOrquestador + 1,original_FD, 4)){
			koopaWarning(socketOrquestador + 1,original_FD,hilosPlanificadores,ganadores,cfg.koopa,cfg.script);
			continue;
		}else{
			socketIngresante = acceptGRID(socketOrquestador,mpantalla);
			int protocol = recvHandshake(&nuevoHandshake,socketIngresante);
			loggearRecepcion(nuevoHandshake, socketIngresante);
			switch (protocol){
			 case -1: loggearProtocolo("Conexion imposible, cerrar socket",protocol);
			 	 close(socketIngresante); break;

			 case  0: loggearProtocolo("Nivel nuevo",protocol);
			 	 nivelNuevo(nuevoHandshake,socketIngresante,hilosPlanificadores); break;

			 case  1: loggearProtocolo("Jugador nuevo",protocol);
			 	 clienteNuevo(nuevoHandshake,socketIngresante); break;

			 case  2: loggearProtocolo("Jugador Ganador",protocol);
			 	 clienteViejo(nuevoHandshake,ganadores); break;

			 default: loggearProtocolo("DESCONOCIDO, cerrar socket",protocol);
			 	 close(socketIngresante); break;
			}
			if(!mpantalla)puts("--ORQUESTADOR-- Escuchando de vuelta...");
			else pantallaStatus("Escuchando");
			koopaWarning(socketOrquestador + 1,original_FD,hilosPlanificadores,ganadores,cfg.koopa,cfg.script);
		}
		if(list_is_empty(listaNiveles))nivelesCaidos();
		loggearEstado_Debug();
		mensajeTrace("\t\t\t---------------------------\t\t\t");
	}
	cerrarLogs_Orquestador(logsOrquestador);
	finalizarTodo(ganadores,hilosPlanificadores,socketOrquestador);
	return 0;
}

void borrarTodoNivel(void*temp){
	nodoNivel*nivel;
	nivel=(nodoNivel*)temp;
	nuevo*aux;
	if(!mpantalla)puts("Borrando el nodo");
	while(nivel->tandaActual==NULL){
		aux=nivel->tandaActual;
		nivel->tandaActual=nivel->tandaActual->sgte;
		free(aux);
	}
	free(nivel);
}

void finalizarTodo(t_list*ganadores,t_list*planificadores,int sock){
	/*
	mensajeTrace("Limpiando las listas");
	if(!mpantalla)puts("Limpiando las listas");
	list_clean(planificadores);
	list_clean(ganadores);
	list_iterate(listaNiveles,borrarTodoNivel);
	list_clean(listaNiveles);
	list_destroy(planificadores);
	list_destroy(ganadores);
	list_destroy(listaNiveles);
	pthread_exit(NULL);*/
	return;
}

bool _caido(nodoNivel*aux){
	if (aux->nid==-1)return true;
	return false;
}

void nivelesCaidos(void){		//AVERIGUAR POR QUE NO FUNCIONA!!!  DEBERIA ELIMINAR LOS NIVELES CAIDOS, QUE TIENEN -1
	nodoNivel*aux=list_remove_by_condition(listaNiveles,(void*)_caido);
	if(aux!=NULL){
		puts("Borrando un nodo!");
		borrarTodoNivel((void*)aux);
	}
}

void koopaWarning(int fdmax, fd_set original, t_list *hilosPlanificadores,t_list *ganadores, char * koopa, char * script){
	if(!chequearKoopa())return;
	char mensaje[64],valor[8];
	int cont;
	mensajeWarning("Esperando jugadores entrantes...");
	if(mtexto)printf("KOOPA WARNING.ON--ORQUESTADOR\n");
	for(cont = 5; cont >= 0; cont--){
		if(mtexto)printf("KOOPA LOOP--ORQUESTADOR\n");
		strcpy(mensaje,"KOOPA: ");
		itoa(cont,valor,10);
		strcat(mensaje,valor);
		if(mpantalla)pantallaKoopa(mensaje);
		if(!chequearKoopa()){
			loguearInfo("***Se recibio un jugador. Koopa interrumpido");
			if(mpantalla) pantallaStatus("Se recibio un jugador");
			return;
		};
		if(selectGRID_orquestador(fdmax,original,2) == 0)	{
			if(mtexto)printf("KOOPA LOOP.MID.1.I--ORQUESTADOR\n");
			continue;
		}else{
			mensajeWarning("**Se recibio una conexion. Koopa retenido");
			if(mpantalla) pantallaStatus("Se recibio una conexion");
			if(mpantalla) settearPantallaKoopa();
			return;
		};
	};
	if(mtexto)printf("KOOPA ACTIVATED.I--ORQUESTADOR\n");
	activarKoopa(hilosPlanificadores, koopa, script);
	return;
};

int selectGRID_orquestador(int fdmax, fd_set original, int tiempo){
	fd_set readfds = original;
	struct timeval intervalo;
	intervalo.tv_usec = 0;
	intervalo.tv_sec = tiempo;
	return select(fdmax,&readfds,NULL,NULL,&intervalo);
};

void responderError(int socketDestino){
	//Enviarle al destino el mensaje de no encontrado o rechazo.
	sendAnswer(-1,0,'\0','\0',socketDestino);
};

void reconectarNivel(nodoNivel *nodo,int nid){
	if( nodo->nid == 0 ){
		nodo->nid = nid;
		char mensaje[32];
		strcpy(mensaje,"*Nivel reconectado: ");
		strcat(mensaje,nodo->name);
		mensajeTrace(mensaje);
		if(mpantalla) pantallaStatus(mensaje);
		return;
	};
	mensajeTrace("*Nivel invasor rechazado");
	if(mpantalla) pantallaStatus("Nivel invasor rechazado");
	responderError(nid);
	return;
};

//no crea una tanda en el buen sentido de la palabra
//solo mete un maldito nodo, pero el nombre le quedo bien
void crearTanda(nuevo** lista){
	nuevo *tempo;
		tempo=(nuevo*)malloc(sizeof(nuevo));
		tempo->pid=0;
		tempo->sym=' ';
		tempo->sgte=NULL;
		*lista=tempo;
}

void crearHiloPlanificador(nodoNivel *nivel,t_list* hilosPlanificadores){
	pthread_t idHilo = hiloGRID(planificador,(void*)nivel);
	nivel->idHilo=idHilo;
	nodoPlanificador *nuevoPlanificador = (nodoPlanificador *)malloc(sizeof(nodoPlanificador));
	nuevoPlanificador->idHilo = idHilo;
	list_add(hilosPlanificadores,nuevoPlanificador);
};

void agregarNivel(handshake handshakeNivel,int socketNivel, t_list* hilosPlanificadores){
	nuevo* tandaActual;
	char mensaje[32];
	strcpy(mensaje,"*Nivel conectado: ");
	strcat(mensaje,handshakeNivel.name);
	mensajeTrace(mensaje);
	if(mpantalla) pantallaStatus(mensaje);
	nodoNivel *nivel = (nodoNivel*)malloc(sizeof (nodoNivel));
	crearTanda(&(tandaActual));
	strcpy(nivel->name,handshakeNivel.name);
	nivel->cantJugadores = 0;
	nivel->tandaRaiz = tandaActual;
	nivel->tandaActual = tandaActual;
	nivel->nid = socketNivel;
	list_add(listaNiveles,nivel);
	crearHiloPlanificador(nivel,hilosPlanificadores);
	mensajeTrace("*Hilo planificador creado");
};

nodoNivel *buscarNivelEnSistema(char nombreNivel[13]){
	//Declaracion de una funcion de forma temporal y dinamica
	bool _is_Nivel(nodoNivel *nivel) {
	if(strcmp(nivel->name,nombreNivel)==0)return true;
		return false;
	}
	nodoNivel *aux = list_find(listaNiveles, (void*) _is_Nivel);
	return aux;
}

nodoNivel* validarNivel(char nombreNivel[13]){
	nodoNivel *aux = buscarNivelEnSistema(nombreNivel);
	if(aux == NULL) return NULL;
	return aux;
};

void nivelNuevo(handshake handshakeNivel,int socketNivel, t_list* hilosPlanificadores){
	nodoNivel *nodoNIVEL = buscarNivelEnSistema(handshakeNivel.name);
	if(nodoNIVEL == NULL){
		agregarNivel(handshakeNivel,socketNivel, hilosPlanificadores);
		return;
	};
	reconectarNivel(nodoNIVEL,socketNivel);
	return;
};

void clienteNuevo(handshake handshakeJugador,int socketJugador){
	nodoNivel *aux = validarNivel(handshakeJugador.name);
	if( aux == NULL){
		responderError(socketJugador);
		mensajeTrace("*Jugador rechazado por nivel inexistenete. Cerrar socket");
		if(mpantalla) pantallaStatus("Jugador ent. rechazado");
		loggearEnvio(socketJugador,-1,0,' ',' ');
		close(socketJugador);
		return;
	}
	aux->tandaActual->pid=socketJugador;
	aux->tandaActual->sym=handshakeJugador.symbol;
	if( aux->tandaActual->sgte == NULL ) crearTanda( &(aux->tandaActual->sgte) );
	aux->tandaActual = aux->tandaActual->sgte;
};

void clienteViejo(handshake handshakeJugador, t_list *ganadores){
	jugadorGanador *ganador= (jugadorGanador*)malloc(sizeof(jugadorGanador));
	ganador->personaje = handshakeJugador.symbol;
	list_add(ganadores,ganador);
	if(!mpantalla)puts("--ORQUESTADOR-- Jugador Ganador Recibido.");
	else pantallaStatus("Jugador Ganador Recibido");
};

bool _hay_jugadores(nodoNivel *nivel) {
	return nivel->cantJugadores > 0;
	}

bool chequearKoopa(){
	if( list_is_empty(ganadores) > 0 ) return false;
	if(mtexto)printf("NOVEDAD.I--ORQUESTADOR\n");
	t_list* nivelesConJugadores = list_filter(listaNiveles, (void*) _hay_jugadores);
	if(mtexto)printf("NOVEDAD.II--ORQUESTADOR\n");
	if( list_size(nivelesConJugadores) > 0 ) return false;
	if(mtexto)printf("NOVEDAD.III--ORQUESTADOR\n");
	return true;
};

int _matar_hilo(nodoPlanificador *planificador){
	//pthread_cancel( planificador->idHilo );
	pthread_join(planificador->idHilo,NULL);
	return 1;
};

void matarHilos(t_list* hilosPlanificadores){
	list_map(hilosPlanificadores, (void*)_matar_hilo);
};

void activarKoopa(t_list* hilosPlanificadores, char * koopa, char * script){
	int status;
	pid_t child_pid;
	mpantalla = false;
	finalizar = true;
	matarHilos(hilosPlanificadores);
while((pantallaTerminada == false) && (mtexto == false));
	if((child_pid = fork()) < 0 ){
		perror("fork failure");
	    exit(1);
	}
	if(child_pid == 0){ //koopa
		mensajeWarning("Ejecutando koopa...");
		if(!mpantalla)puts("Ejecutando Koopa...");
		mpantalla = false;
		if(mtexto)
		execlp(koopa, "koopa","/home/utnso/tmp",script,"--text",(char *)0);
		else
		execlp(koopa, "koopa","/home/utnso/tmp",script, (char *)0);
	//si se ejecuta esto es pórque hubo un problema con el exec
	    perror("execl() failure!\n");
	    mensajeWarning("exect fallido");
	    _exit(1);
	}else{ //Orquestador
	    wait(&status);
	    mensajeWarning("Proceso Koopa finalizado");
	}
	endwin();
	return;
};

cfgOrquestador cargarOrquestador(char *path){
		t_config * cfgPlataforma;
		cfgOrquestador registroOrquestador;
		cfgPlataforma=config_create(path);

		if (config_has_property(cfgPlataforma,"puerto")){
			registroOrquestador.puerto=puertoPlataforma(cfgPlataforma);
			if(!mpantalla)printf("Puerto: %d.\n", registroOrquestador.puerto);
		}else{
			if(!mpantalla)printf("Archivo de configuracion incompleto, falta campo: puerto\n");
			exit(0);
		}

		if (config_has_property(cfgPlataforma,"ip")){
			registroOrquestador.ip=ipPlataforma(cfgPlataforma);
			if(!mpantalla)printf("IP: %s.\n", registroOrquestador.ip);
		}else{
			if(!mpantalla)printf("Archivo de configuracion incompleto, falta campo: ip\n");
			exit(0);
		}

		if (config_has_property(cfgPlataforma,"koopa")){
			registroOrquestador.koopa=pathKoopaPlataforma(cfgPlataforma);
			if(!mpantalla)printf("Path de koopa: %s.\n", registroOrquestador.koopa);
		}else{
			if(!mpantalla)printf("Archivo de configuracion incompleto, falta campo: koopa\n");
			exit(0);
		}

		if (config_has_property(cfgPlataforma,"script")){
			registroOrquestador.script=pathScriptPlataforma(cfgPlataforma);
			if(!mpantalla)printf("Path del script: %s.\n", registroOrquestador.script);
		}else{
			if(!mpantalla)printf("Archivo de configuracion incompleto, falta campo: script\n");
			exit(0);
		}

		return registroOrquestador;
};

int puertoPlataforma( t_config * cfgPlataforma){
	return config_get_int_value(cfgPlataforma,"puerto");
}

char * pathKoopaPlataforma( t_config * cfgPlataforma){
	return config_get_string_value(cfgPlataforma,"koopa");
}

char * pathScriptPlataforma( t_config * cfgPlataforma){
	return config_get_string_value(cfgPlataforma,"script");
}

char * ipPlataforma( t_config * cfgPlataforma){
	return config_get_string_value(cfgPlataforma,"ip");
}

logs crearLogs_Orquestador(){
	char file[64];
	strcpy(file,LOCAL_LOG);
	strcat(file,PROGRAMA);
	strcat(file,"-Trace");
	strcat(file,".txt");
	logsOrquestador.trace=log_create(file,PROGRAMA,mostrarOrquestador,LOG_LEVEL_TRACE);
	strcpy(file,LOCAL_LOG);
	strcat(file,PROGRAMA);
	strcat(file,"-Debug");
	strcat(file,".txt");
	logsOrquestador.debug=log_create(file,PROGRAMA,mostrarOrquestador,LOG_LEVEL_DEBUG);
	strcpy(file,LOCAL_LOG);
	strcat(file,PROGRAMA);
	strcat(file,"-Warning");
	strcat(file,".txt");
	//logsOrquestador.warning=log_create(file,PROGRAMA,mostrarOrquestador,LOG_LEVEL_WARNING);
	return logsOrquestador;
}

void cerrarLogs_Orquestador(logs tabla){
	log_destroy(tabla.debug);
	log_destroy(tabla.trace);
}

void loggearEstado_Debug(){
	char mensaje[128],valor[16];

	void _logNodoNivel(nodoNivel* nivel){
		strcpy(mensaje," ");
		strcat(mensaje,nivel->name);
		strcat(mensaje," (socket/hiloID/cantJugadores) :- (");
		itoa(nivel->nid,valor,10);
		strcat(mensaje,valor);
		strcat(mensaje,"/");
		itoa(nivel->idHilo,valor,10);
		strcat(mensaje,valor);
		strcat(mensaje,"/");
		itoa(nivel->cantJugadores,valor,10);
		strcat(mensaje,valor);
		strcat(mensaje,") ");
		mensajeDebug(mensaje);
	}

	mensajeDebug("Estado de la lista de Niveles:");

	if( list_is_empty(listaNiveles) ){
		mensajeDebug("--NO HAY NIVELES CONECTADOS--");
	}else{
		list_iterate(listaNiveles,(void*)_logNodoNivel);
	}

	void _logGanador(jugadorGanador* player){
		valor[0] = '-';
		valor[1] = ' ';
		valor[2] = '(';
		valor[3] = player->personaje;
		valor[4] = ')';
		valor[5] = ' ';
		valor[6] = '\0';
		strcat(mensaje,valor);
		}

	strcpy(mensaje,"Estado de la lista de ganadores:");

	if( list_is_empty(ganadores) ){
		strcat(mensaje," --NO HAY JUGADORES GANADORES--");
	}else{
		list_iterate(ganadores,(void*)_logGanador);
	}
	mensajeDebug(mensaje);

	strcpy(mensaje,"¿KOOPA? ");
	if(chequearKoopa(ganadores))
		 strcat(mensaje,"HABILITADO - Cumple con las condiciones");
	else strcat(mensaje,"IMPOSIBLE DE EJECUTAR");
	mensajeDebug(mensaje);

	mensajeDebug("\t\t\t---------------------------\t\t\t");
}

void loguearDatosIniciales(cfgOrquestador cfg){
	char mensaje[128],valor[16];
	strcpy(mensaje,"Programa: ");
	strcat(mensaje,PROGRAMA);
	mensajeTrace(mensaje);
	strcpy(mensaje,"IP: ");
	strcat(mensaje,cfg.ip);
	mensajeTrace(mensaje);
	strcpy(mensaje,"Puerto: ");
	itoa(cfg.puerto,valor,10);
	strcat(mensaje,valor);
	mensajeTrace(mensaje);
	strcpy(mensaje,"Path del script: ");
	strcat(mensaje,cfg.script);
	mensajeTrace(mensaje);
	strcpy(mensaje,"Path de KOOPA: ");
	strcat(mensaje,cfg.koopa);
	mensajeTrace(mensaje);
	mensajeTrace("\t\t\t---------------------------\t\t\t");
}

void loggearEnvio(int sock,int msg,int cont,char data,char sym){
	char mensaje[256],valor[16];
	strcpy(mensaje,"Answer(Msj/Cont/Dat/Sym) :- (");
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
	strcat(mensaje,") SocketDestino: ");
	itoa(sock,valor,10);
	strcat(mensaje,valor);
	mensajeTrace(mensaje);
}
void loggearRecepcion(handshake paquete, int sock){
	char mensaje[256],valor[16];
	strcpy(mensaje,"Handshake(Protocolo/Mensaje/Simbolo) :- (");
	itoa(paquete.type,valor,10);
	strcat(mensaje,valor);
	strcat(mensaje,"/");
	strcat(mensaje,paquete.name);
	strcat(mensaje,"/");
	valor[0] = paquete.symbol;
	valor[1] = '\0';
	strcat(mensaje,valor);
	strcat(mensaje,") Socket: ");
	itoa(sock,valor,10);
	strcat(mensaje,valor);
	mensajeTrace(mensaje);
}

void loggearProtocolo(char* accion, int protocol){
	char mensaje[256],valor[16];
	strcpy(mensaje,"Protocolo ");
	itoa(protocol,valor,10);
	strcat(mensaje,valor);
	strcat(mensaje,": ");
	strcat(mensaje,accion);
	mensajeTrace(mensaje);
}

void mensajeTrace(char*mensaje){
	log_trace(logsOrquestador.trace,mensaje,"TRACE");
}

void mensajeWarning(char*mensaje){
	//log_warning(logsOrquestador.warning,mensaje,"WARNING");
}

void mensajeDebug(char*mensaje){
	log_debug(logsOrquestador.debug,mensaje,"DEBUG");
}

