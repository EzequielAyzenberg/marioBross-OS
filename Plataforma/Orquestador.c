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
 * ClienteNuevo
 * ClienteViejo             --OK--
 * CrearHiloPlanificador    --OK--
 * NivelNuevo
 * ValidarNiver xD
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

	while(1){
		socketOrquestador = listenGRID(info.puerto);
		socketIngresante = acceptGRID(socketOrquestador);
		switch ( recvHandshake(&nuevoHandshake,socketIngresante) ){
		case 0:   nivelNuevo(nuevoHandshake,socketIngresante,listaNiveles); break;
		case 1: clienteNuevo(nuevoHandshake,socketIngresante,listaNiveles); break;
		case 2: clienteViejo(nuevoHandshake,ganadores);
		}
	}
	return 0;
}

void nivelNuevo(handshake handshakeNivel,int socketNivel, t_list* listaNiveles){
		puts("Tenemos un nivel conectado!!");
		crearHiloPlanificador(handshakeNivel, socketNivel);
		nodoNivel *nivel = (nodoNivel*)malloc(sizeof (nodoNivel));
		nivel->cantJugadores = 0;
		strcpy(nivel->name,handshakeNivel.name);
		nivel->tandaRaiz = NULL;
		nivel->tandaActual = NULL;
		list_add(listaNiveles,nivel);
};

void clienteNuevo(handshake handshakeJugador,int socketJugador, t_list* listaNiveles){
	nuevo* tandaActual;

	if( NULL == (tandaActual = validarNivel(handshakeJugador.name,listaNiveles))){
		responder(socketJugador); return;}
	if( tandaActual == NULL) crearTanda(&(tandaActual));
	    nuevo* novedadActual = tandaActual;
	while(novedadActual->pid == 0)
		novedadActual = novedadActual->sgte;
		puts("Se ha recibido un nuevo Personaje");
		puts("Recibiendo info del Personaje..");
		novedadActual->pid=socketJugador;
		novedadActual->sym=handshakeJugador.symbol;
	if( novedadActual->sgte == NULL ) tandaActual = NULL;
};

void clienteViejo(handshake handshakeJugador, t_list *ganadores){
	jugadorGanador *ganador;
	ganador->personaje = handshakeJugador.symbol;
	list_add(ganadores,ganador);
};

void crearHiloPlanificador(handshake handshakeNivel,int socketNivel){
	nodoNivel registro;
	strcpy(registro.name,handshakeNivel.name);
	registro.nid=socketNivel;
	registro.cantJugadores=0;
	registro.tandaRaiz=NULL;

	pthread_t idHilo;
		if(pthread_create(&idHilo, NULL, planificador, (void*)&registro)==0)
		puts("Hilo creado correctamente.");
		else puts("Hubo un problema en la creacion del hilo.");

}

void responder(int socketJugador){

};

nuevo* validarNivel(char nombreNivel[13],t_list* listaNiveles){
	return NULL;
};
