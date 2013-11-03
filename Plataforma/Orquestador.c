/*
 * orquestador.c
 *
 *  Created on: 27/10/2013
 *      Author: EzequielAyzenberg
 */

/* Lista de Eventos tipo Analisis de Sistemas xD
 *
 * --DELEGACION DE CONEXIONES--
 *
 * RecepcionDeConexiones    --OK--
 * ClienteNuevo				--OK--
 * ClienteViejo             --OK--
 * CrearHiloPlanificador    --OK--
 * NivelNuevo				--OK--
 * ValidarNiver xD          --OK--
 * CrearNuevaTanda          --OK--
 *
 * --KOOPA--
 *
 * ChequearKoopa			--OK--
 * ActivarKoopa
 * MatarHilos				--OK--
 */

#include"Orquestador.h"
#include"Planificador.h"

void *orquestador(void* infoAux){
	// Desrefereciacion de la info que recibe el orquestador
	infoOrquestador *infoBis=(infoOrquestador*)infoAux;
	infoOrquestador info= *infoBis;

	int socketOrquestador, socketIngresante;
	handshake nuevoHandshake;
	t_list *listaNiveles = info.listaNiveles;
	t_list *ganadores = list_create();
	t_list *hilosPlanificadores = list_create();
	socketOrquestador = listenGRID(info.puerto);

	while(1){
		socketIngresante = acceptGRID(socketOrquestador);
		switch (recvHandshake(&nuevoHandshake,socketIngresante)){
		case 0:   nivelNuevo(nuevoHandshake,socketIngresante,listaNiveles,hilosPlanificadores); break;
		case 1: clienteNuevo(nuevoHandshake,socketIngresante,listaNiveles); break;
		case 2: clienteViejo(nuevoHandshake,ganadores);
		}
		puts("--ORQUESTADOR-- Escuchando de vuelta..");
	}
	return 0;
}

void responderError(int socketDestino){
	//Enviarle al destino el mensaje de no encontrado o rechazo.
	sendAnswer(-1,0,'\0','\0',socketDestino);
};

void reconectarNivel(nodoNivel *nodo,int nid){
	if( nodo->nid == 0 ){
		nodo->nid = nid;
		return;
	};
	puts("--ORQUESTADOR-- Nivel invasor rechazado");
	responderError(nid);
	return;
};

//no crea una tanda en el buen sentido de la palabra
//solo mete un maldito nodo, pero el nombre le quedo bien :3
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
		nodoPlanificador *nuevoPlanificador = (nodoPlanificador *)malloc(sizeof(nodoPlanificador));
		nuevoPlanificador->idHilo = idHilo;
		list_add(hilosPlanificadores,nuevoPlanificador);
};

void agregarNivel(handshake handshakeNivel,int socketNivel, t_list* listaNiveles, t_list* hilosPlanificadores){
	nuevo* tandaActual=(nuevo*)malloc(sizeof(nuevo));
			puts("--ORQUESTADOR--Tenemos un nivel conectado!!");
			nodoNivel *nivel = (nodoNivel*)malloc(sizeof (nodoNivel));
			crearTanda(&(tandaActual));
			strcpy(nivel->name,handshakeNivel.name);
			nivel->cantJugadores = 0;
			nivel->tandaRaiz = tandaActual;
			nivel->tandaActual = tandaActual;
			nivel->nid = socketNivel;
			list_add(listaNiveles,nivel);
			crearHiloPlanificador(nivel,hilosPlanificadores);
};

nodoNivel *buscarNivelEnSistema(char nombreNivel[13],t_list* listaNiveles){
	//Declaracion de una funcion de forma temporal y dinamica
	bool _is_Nivel(nodoNivel *nivel) {
		    if(strcmp(nivel->name,nombreNivel)==0)return true;
		    return false;
	}
	nodoNivel *aux = list_find(listaNiveles, (void*) _is_Nivel);
	return aux;
}

nodoNivel* validarNivel(char nombreNivel[13],t_list* listaNiveles){
	nodoNivel *aux = buscarNivelEnSistema( nombreNivel,listaNiveles );
	if(aux == NULL) return NULL;
	return aux;
};

void nivelNuevo(handshake handshakeNivel,int socketNivel, t_list* listaNiveles, t_list* hilosPlanificadores){
	nodoNivel *nodoNIVEL = buscarNivelEnSistema(handshakeNivel.name, listaNiveles);
	if(nodoNIVEL == NULL){
		agregarNivel(handshakeNivel,socketNivel,listaNiveles, hilosPlanificadores);
		return;
	};
	reconectarNivel(nodoNIVEL,socketNivel);
	return;
};

void clienteNuevo(handshake handshakeJugador,int socketJugador, t_list* listaNiveles){
	nodoNivel *aux = validarNivel(handshakeJugador.name,listaNiveles);
	if( aux == NULL){
		puts("--ORQUESTADOR-- Jugador rechazado por nivel inexistenete");
		responderError(socketJugador);
		return;
	}
		puts("--ORQUESTADOR--Se ha recibido un nuevo Personaje");
		aux->tandaActual->pid=socketJugador;
		aux->tandaActual->sym=handshakeJugador.symbol;
		if( aux->tandaActual->sgte == NULL )
			crearTanda( &(aux->tandaActual->sgte) );
	    aux->tandaActual = aux->tandaActual->sgte;
	    puts("--ORQUESTADOR--Info del Personaje recibida");
};

void clienteViejo(handshake handshakeJugador, t_list *ganadores){
	jugadorGanador *ganador= (jugadorGanador*)malloc(sizeof(jugadorGanador));
	ganador->personaje = handshakeJugador.symbol;
	list_add(ganadores,ganador);
};

bool _hay_jugadores(nodoNivel *nivel) {
	return nivel->cantJugadores > 0;
	}

bool chequearKoopa(t_list *ganadores, t_list* listaNiveles){
	if( list_is_empty(ganadores) == 0 ) return false;

	t_list* nivelesConJugadores = list_filter(listaNiveles, (void*) _hay_jugadores);
	if( list_size(nivelesConJugadores) > 0 ) return false;

	return true;
};

int _matar_hilo(nodoPlanificador *planificador){
	return pthread_cancel( planificador->idHilo );
};

void matarHilos(t_list* hilosPlanificadores){
	list_map(hilosPlanificadores, (void*)_matar_hilo);
};

void activarKoopa(void){

};
