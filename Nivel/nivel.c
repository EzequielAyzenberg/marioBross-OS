/*
 * nivel.c
 *
 *  Created on: 12/10/2013
 *      Author: utnso
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <curses.h>
#include "cargador.h"
#include "gui.h"
#include "enemigos.h"
#include "conexiones.h"
#include <commons/collections/list.h>
#include <time.h>
#include <theGRID/sockets.h>
#include <theGRID/general.h>
#include "semaforos.h"
#include "interbloqueo.h"
#include <signal.h>

pthread_mutex_t mutexDibujar =PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutexMatarPersonaje =PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutexCrearPersonaje =PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutexLog =PTHREAD_MUTEX_INITIALIZER;



main(int argc, char *argv[]){
	nivelConfig config;
	switch (argc){
		case 2:
			config.path=malloc(strlen(argv[1])+1);
			strcpy(config.path,argv[1]);
			printf("La ruta es %s", config.path);
			break;
		default:
			config.path=malloc(strlen("/home/utnso/GITHUB/tp-2013-2c-the-grid/Nivel/nivel1.cfg")+1);
			strcpy(config.path,"/home/utnso/GITHUB/tp-2013-2c-the-grid/Nivel/nivel1.cfg");
			puts("Se usara la direccion estandar");
			break;
		}




	newArchLogInfo("nivel1");


	pthread_mutex_t mutexDibujar =PTHREAD_MUTEX_INITIALIZER;
	pthread_t hiloEnemigos;

	t_list* listaEnemigos;
	t_list* listaJugadoresActivos;
	t_list* listaJugadoresMuertos;
	t_list* listaJugadoresBloqueados;
	listaJugadoresActivos=list_create();
	listaJugadoresMuertos=list_create();
	listaJugadoresBloqueados=list_create();
	int rows,cols;
	cargarConfig(&config);
	datosConexiones datosConexiones;
	datosConexiones.config=&config;
	datosConexiones.listaJugadoresActivos=listaJugadoresActivos;
	datosConexiones.listaJugadoresMuertos=listaJugadoresMuertos;
	datosConexiones.listaJugadoresBloqueados=listaJugadoresBloqueados;
	datosConexiones.listaRecursos=config.listaCajas;
	datosConexiones.socket=-1;

	inicializarNivel(config,&rows,&cols);//Crea el nivel por primera vez,carga las cajas y devuelve el tamaño de la pantalla
	infoEnemigosThread infoParaEnemigos;
	listaEnemigos=list_create();
	datosConexiones.listaEnemigos=listaEnemigos;
	infoParaEnemigos.listaEnemigos=listaEnemigos;

	infoParaEnemigos.listaCajas=config.listaCajas;
	infoParaEnemigos.listaJugadoresActivos=listaJugadoresActivos;
	infoParaEnemigos.listaJugadoresMuertos=listaJugadoresMuertos;

	infoParaEnemigos.rows=rows;
	infoParaEnemigos.cols=cols;
	infoParaEnemigos.sleepEnemigos=config.sleepEnemigos;
	infoParaEnemigos.nombreNivel=(char*)malloc(strlen(config.nombre)+1);
	infoParaEnemigos.socket=&datosConexiones.socket;
	strcpy(infoParaEnemigos.nombreNivel,config.nombre);


														//nivel_gui_terminar();
	int i=0;
	pthread_t ejemplo;
	while(i<config.enemigos){
		infoEnemigosThread* infoParaEnemigosBuffer=(infoEnemigosThread*)malloc(sizeof(infoEnemigosThread));
		*infoParaEnemigosBuffer=infoParaEnemigos;
		infoParaEnemigosBuffer->myinfo=(coordenadas*)malloc(sizeof(coordenadas));
		infoParaEnemigosBuffer->myinfo->posx=0;
		infoParaEnemigosBuffer->myinfo->posy=0;
		//printf("mi memoria:%d\n",(int)infoParaEnemigosBuffer->myinfo);
		infoParaEnemigosBuffer->myinfo->idGoomba=0;
		pthread_create(&hiloEnemigos,NULL,(void*)&controlEnemigos,(void*)infoParaEnemigosBuffer);
		infoParaEnemigosBuffer->myinfo->idGoomba=hiloEnemigos;
		list_add(listaEnemigos,infoParaEnemigosBuffer->myinfo);

		i++;
	}

	//actualizarNivel(config.listaCajas,listaEnemigos,listaJugadoresActivos,config.nombre);
	coordenadas recorridoEnemigos[config.enemigos][4];
	//actualizarNivel(config.listaCajas,listaEnemigos,listaJugadoresActivos,config.nombre);
		//nivel_gui_terminar();


infoInterbloqueo infoParaInterbloqueo;
infoParaInterbloqueo.listaRecursos=config.listaCajas;
infoParaInterbloqueo.listaJugadores=listaJugadoresActivos;
infoParaInterbloqueo.listaJugadoresMuertos=listaJugadoresMuertos;
infoParaInterbloqueo.tiempoEspera=config.intervaloDeadLock;
infoParaInterbloqueo.socket=&datosConexiones.socket;
pthread_t hiloInterbloqueo;
pthread_create(&hiloInterbloqueo,NULL,(void*)&controlInterbloqueo,(void*)&infoParaInterbloqueo);
handshakePlataforma(&datosConexiones);

/*
free(config.algoritmo);
free(config.nombre);
free(config.orquestador);
list_destroy(config.listaCajas);
list_destroy(listaJugadoresActivos);
list_destroy(listaJugadoresMuertos);
list_destroy(listaEnemigos);
*/
	return 0;
}

