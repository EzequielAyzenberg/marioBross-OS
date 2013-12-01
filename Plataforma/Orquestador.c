/*
 * orquestador.c
 *
 *  Created on: 27/10/2013
 *      Author: EzequielAyzenberg
 */

#include"Orquestador.h"
#include"Planificador.h"
#define PROGRAMA "ORQUESTADOR"

void cerrarTodo(int senial){
	finalizar=true;
	signal(SIGINT,SIG_DFL);
}

extern char * CFG_PATH;
extern t_list *listaNiveles;

void *orquestador(void* infoAux){
	signal(SIGINT,cerrarTodo);
	finalizar=false;

	cfgOrquestador cfg = cargarOrquestador(CFG_PATH);

	printf("--ORQUESTADOR-- IP: %s.\n", cfg.ip);
	printf("--ORQUESTADOR-- Puerto: %d.\n", cfg.puerto);
	printf("--ORQUESTADOR-- Path de koopa: %s.\n", cfg.koopa);
	printf("--ORQUESTADOR-- Path del script: %s.\n", cfg.script);

	int socketOrquestador, socketIngresante;
	handshake nuevoHandshake;
	t_list *ganadores = list_create();
	t_list *hilosPlanificadores = list_create();
	socketOrquestador = listenGRID(cfg.puerto,cfg.ip);
	fd_set original_FD;
	FD_ZERO(&original_FD);
	FD_SET(socketOrquestador, &original_FD);

	while(!finalizar){
		if(0 == selectGRID_orquestador(socketOrquestador + 1,original_FD, 5)){
			if(chequearKoopa(ganadores))puts("chequearkoopa true");
			koopaWarning(socketOrquestador + 1,original_FD,hilosPlanificadores,ganadores,cfg.koopa,cfg.script);
			continue;
		}else{
			char mensaje[64],valor[8];
			socketIngresante = acceptGRID(socketOrquestador);
			strcpy(mensaje,"Se escuchara al socket numero ");
			itoa(socketIngresante,valor,10);
			strcat(mensaje,valor);
			loguearInfo(mensaje);
			switch (recvHandshake(&nuevoHandshake,socketIngresante)){
			 case -1: close(socketIngresante); break;
			 case  0: nivelNuevo(nuevoHandshake,socketIngresante,hilosPlanificadores); break;
			 case  1: clienteNuevo(nuevoHandshake,socketIngresante); break;
			 case  2: clienteViejo(nuevoHandshake,ganadores); break;
			 default: loguearWarning("Protocolo de mensaje no encontrado"); close(socketIngresante); break;
			}
			puts("--ORQUESTADOR-- Escuchando de vuelta...");
			koopaWarning(socketOrquestador + 1,original_FD,hilosPlanificadores,ganadores,cfg.koopa,cfg.script);
		}
	}
	finalizarTodo(ganadores,hilosPlanificadores,socketOrquestador);
	return 0;
}

void borrarTodoNivel(void*temp){
	nodoNivel*nivel;
	nivel=(nodoNivel*)temp;
	nuevo*aux;
	puts("Borrando el nodo");
	while(nivel->tandaActual==NULL){
		aux=nivel->tandaActual;
		nivel->tandaActual=nivel->tandaActual->sgte;
		free(aux);
	}
	free(nivel->tandaRaiz);
	free(nivel);
}

void finalizarTodo(t_list*ganadores,t_list*planificadores,int sock){
    loguearInfo("Matando hilos planificadores");
    puts("Matando hilos planificadores");
	matarHilos(planificadores);
	loguearInfo("Limpiando las listas");
	puts("Limpiando las listas");
	list_clean(planificadores);
	list_clean(ganadores);
	list_iterate(listaNiveles,borrarTodoNivel);
	list_clean(listaNiveles);
	list_destroy(planificadores);
	list_destroy(ganadores);
	list_destroy(listaNiveles);
	pthread_exit(NULL);
}

void koopaWarning(int fdmax, fd_set original, t_list *hilosPlanificadores,t_list *ganadores, char * koopa, char * script){
	if(!chequearKoopa(ganadores))return;
	char mensaje[64],valor[8];
	int cont;
	loguearWarning("--ORQUESTADOR-- Esperando jugadores entrantes...");
	for(cont = 5; cont >= 0; cont--){
		strcpy(mensaje,"--ORQUESTADOR-- Se ejecutara Koopa en: ");
		itoa(cont,valor,10);
		strcat(mensaje,valor);
		loguearWarning(mensaje);
		if(!chequearKoopa(ganadores)){
			loguearInfo("--ORQUESTADOR-- Se recibio un jugador. Koopa interrumpido");
			return;
		};
		if(selectGRID_orquestador(fdmax,original,2) == 0)
			continue;
		else{
			loguearInfo("--ORQUESTADOR-- Se recibio una conexion. Koopa retenido");
			return;
		};
	};
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
		return;
	};
	loguearInfo("--ORQUESTADOR-- Nivel invasor rechazado");
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
	nuevo* tandaActual;//=(nuevo*)malloc(sizeof(nuevo));
	char mensaje[64];
	strcpy(mensaje,"Nivel conectado: ");
	strcat(mensaje,handshakeNivel.name);
	loguearInfo(mensaje);
	nodoNivel *nivel = (nodoNivel*)malloc(sizeof (nodoNivel));
	crearTanda(&(tandaActual));
	strcpy(nivel->name,handshakeNivel.name);
	nivel->cantJugadores = 0;
	nivel->tandaRaiz = tandaActual;
	nivel->tandaActual = tandaActual;
	nivel->nid = socketNivel;
	list_add(listaNiveles,nivel);
	crearHiloPlanificador(nivel,hilosPlanificadores);
	loguearInfo("Hilo planificador creado");
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
		loguearInfo("--ORQUESTADOR-- Jugador rechazado por nivel inexistenete");
		responderError(socketJugador);
		return;
	}
	loguearInfo("Se ha recibido un nuevo Personaje");
	aux->tandaActual->pid=socketJugador;
	aux->tandaActual->sym=handshakeJugador.symbol;
	if( aux->tandaActual->sgte == NULL ) crearTanda( &(aux->tandaActual->sgte) );
	aux->tandaActual = aux->tandaActual->sgte;
	loguearInfo("Info del Personaje recibida");
};

void clienteViejo(handshake handshakeJugador, t_list *ganadores){
	jugadorGanador *ganador= (jugadorGanador*)malloc(sizeof(jugadorGanador));
	ganador->personaje = handshakeJugador.symbol;
	list_add(ganadores,ganador);
	puts("--ORQUESTADOR-- Jugador Ganador Recibido.");
	char mensaje[64],valor[8];
	valor[0]=handshakeJugador.symbol;
	valor[1]='\0';
	strcpy(mensaje,"--ORQUESTADOR-- Jugador ganador: ");
	strcat(mensaje,valor);
	loguearInfo(mensaje);
};

bool _hay_jugadores(nodoNivel *nivel) {
	return nivel->cantJugadores > 0;
	}

bool chequearKoopa(t_list *ganadores){
	if( list_is_empty(ganadores) > 0 ) return false;
	t_list* nivelesConJugadores = list_filter(listaNiveles, (void*) _hay_jugadores);
	if( list_size(nivelesConJugadores) > 0 ) return false;
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
	if((child_pid = fork()) < 0 ){
		perror("fork failure");
	    exit(1);
	}
	if(child_pid == 0){ //koopa
		loguearInfo("Ejecutando koopa...");
	    puts("Ejecutando Koopa...");

		execlp(koopa, "koopa", "/home/utnso/temp",script, (char *)0);
	//si se ejecuta esto es pórque hubo un problema con el exec
	    perror("execl() failure!\n");
	    loguearError("exect fallido");
	    _exit(1);
	}else{ //Orquestador
	    wait(&status);
	    loguearInfo("Proceso Koopa finalizado");
	}
	finalizar=true;
	return;
};

cfgOrquestador cargarOrquestador(char *path){
		t_config * cfgPlataforma;
		cfgOrquestador registroOrquestador;
		cfgPlataforma=config_create(path);

		if (config_has_property(cfgPlataforma,"puerto")){
			registroOrquestador.puerto=puertoPlataforma(cfgPlataforma);
			printf("Puerto: %d.\n", registroOrquestador.puerto);
		}else{
			printf("Archivo de configuracion incompleto, falta campo: puerto\n");
			exit(0);
		}

		if (config_has_property(cfgPlataforma,"ip")){
			registroOrquestador.ip=ipPlataforma(cfgPlataforma);
			printf("IP: %s.\n", registroOrquestador.ip);
		}else{
			printf("Archivo de configuracion incompleto, falta campo: ip\n");
			exit(0);
		}

		if (config_has_property(cfgPlataforma,"koopa")){
			registroOrquestador.koopa=pathKoopaPlataforma(cfgPlataforma);
			printf("Path de koopa: %s.\n", registroOrquestador.koopa);
		}else{
			printf("Archivo de configuracion incompleto, falta campo: koopa\n");
			exit(0);
		}

		if (config_has_property(cfgPlataforma,"script")){
			registroOrquestador.script=pathScriptPlataforma(cfgPlataforma);
			printf("Path del script: %s.\n", registroOrquestador.script);
		}else{
			printf("Archivo de configuracion incompleto, falta campo: script\n");
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
