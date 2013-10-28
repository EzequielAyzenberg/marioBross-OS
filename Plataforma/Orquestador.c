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
 * ChequearKoopa
 * ActivarKoopa
 * MatarHilos
 */

#include"Orquestador.h"
#include"Planificador.h"

//infoOrquestador
void *orquestador(void* infoAux){
	infoOrquestador *infoBis=(infoOrquestador*)infoAux;
	infoOrquestador info= *infoBis;
	int socketOrquestador, socketIngresante;
	handshake nuevoHandshake;
	t_list *listaNiveles = info.listaNiveles;
	t_list *ganadores = list_create();

	socketOrquestador = listenGRID(info.puerto);
	while(1){
		socketIngresante = acceptGRID(socketOrquestador);
		switch ( recvHandshake(&nuevoHandshake,socketIngresante) ){
		case 0:   nivelNuevo(nuevoHandshake,socketIngresante,listaNiveles); break;
		case 1: clienteNuevo(nuevoHandshake,socketIngresante,listaNiveles); break;
		case 2: clienteViejo(nuevoHandshake,ganadores);
		}
	puts("--ORQUESTADOR-- Escuchando de vuelta..");
	}
	return 0;
}

void nivelNuevo(handshake handshakeNivel,int socketNivel, t_list* listaNiveles){
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
		crearHiloPlanificador(handshakeNivel, nivel);

};

void clienteNuevo(handshake handshakeJugador,int socketJugador, t_list* listaNiveles){
	nuevo* tandaActual;

	if( NULL == (tandaActual = validarNivel(handshakeJugador.name,listaNiveles))){
		responder(socketJugador);
		return;
	}
		puts("--ORQUESTADOR--Se ha recibido un nuevo Personaje");
		tandaActual->pid=socketJugador;
		tandaActual->sym=handshakeJugador.symbol;
		if( tandaActual->sgte == NULL ) crearTanda(&(tandaActual->sgte));
	    tandaActual = tandaActual->sgte;
	    puts("--ORQUESTADOR--Info del Personaje recibida");
};

void clienteViejo(handshake handshakeJugador, t_list *ganadores){
	jugadorGanador *ganador = (jugadorGanador*)malloc(sizeof(jugadorGanador));
	ganador->personaje = handshakeJugador.symbol;
	list_add(ganadores,ganador);
};

void crearHiloPlanificador(handshake handshakeNivel,nodoNivel* nivel){

	pthread_t idHilo;
		idHilo=hiloGRID(planificador,(void*)nivel);
}

void responder(int socketJugador){
	//Enviarle al Personaje el mensaje de no encontrado.
	sendAnswer(-1,0,'\0','\0',socketJugador);
};

nuevo* validarNivel(char nombreNivel[13],t_list* listaNiveles){
		int _is_Nivel(nodoNivel *nivel) {
	    if(strcmp(nivel->name,nombreNivel)==0)return true;
	    return false;
		}
	nodoNivel *aux = list_find(listaNiveles, (void*) _is_Nivel);
	if(aux == NULL) return NULL;
	return aux->tandaActual;
};

void crearTanda(nuevo** lista){
	int i;
	nuevo *tempo,*aux;
	aux=*lista;
	for (i=1;i<=5;i++){
		tempo=(nuevo*)malloc(sizeof(nuevo));
		tempo->pid=0;
		tempo->sym=' ';
		tempo->sgte=NULL;
		if(*lista==NULL)*lista=tempo;
		else{
			aux->sgte=tempo;
			aux=tempo;
		}
	}
}
