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

pthread_mutex_t mutexDibujar =PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutexMatarPersonaje =PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutexLog =PTHREAD_MUTEX_INITIALIZER;
main(){
	newArchLogInfo("nivel1");


	pthread_mutex_t mutexDibujar =PTHREAD_MUTEX_INITIALIZER;
	pthread_t hiloEnemigos;
	nivelConfig config;
	t_list* listaEnemigos;
	t_list* listaJugadoresActivos;
	t_list* listaJugadoresMuertos;
	listaJugadoresActivos=list_create();//provisorio hasta que un proceso se encargue de crearla
	listaJugadoresMuertos=list_create();//idem
	//crearPersonaje(&listaJugadoresActivos,0,0,'V');
	//crearPersonaje(&listaJugadoresActivos,10,10,'@');
	//crearPersonaje(&listaJugadoresActivos,20,20,'F');
	//crearPersonaje(&listaJugadoresActivos,40,15,'A');
	//moverPersonaje(&listaJugadoresActivos,1,1,'@');
	int rows,cols;
	cargarConfig(&config);
	datosConexiones datosConexiones;
	datosConexiones.config=&config;
	datosConexiones.listaJugadoresActivos=listaJugadoresActivos;
	datosConexiones.listaJugadoresMuertos=listaJugadoresMuertos;
	datosConexiones.listaRecursos=config.listaCajas;


	inicializarNivel(config,&rows,&cols);//Crea el nivel por primera vez,carga las cajas y devuelve el tamaño de la pantalla
	infoEnemigosThread infoParaEnemigos;
	listaEnemigos=list_create();
	infoParaEnemigos.listaEnemigos=listaEnemigos;

	infoParaEnemigos.listaCajas=config.listaCajas;
	infoParaEnemigos.listaJugadoresActivos=listaJugadoresActivos;
	infoParaEnemigos.listaJugadoresMuertos=listaJugadoresMuertos;

	infoParaEnemigos.rows=rows;
	infoParaEnemigos.cols=cols;
	infoParaEnemigos.sleepEnemigos=config.sleepEnemigos;
	infoParaEnemigos.nombreNivel=(char*)malloc(strlen(config.nombre)+1);
	strcpy(infoParaEnemigos.nombreNivel,config.nombre);


														//nivel_gui_terminar();
	int i=0;
	pthread_t ejemplo;
	while(i<config.enemigos){
		infoEnemigosThread* infoParaEnemigosBuffer=(infoEnemigosThread*)malloc(sizeof(infoEnemigosThread));
		*infoParaEnemigosBuffer=infoParaEnemigos;
		infoParaEnemigosBuffer->myinfo=(coordenadas*)malloc(sizeof(coordenadas));
		//printf("mi memoria:%d\n",(int)infoParaEnemigosBuffer->myinfo);
		pthread_create(&hiloEnemigos,NULL,(void*)&controlEnemigos,(void*)infoParaEnemigosBuffer);
		infoParaEnemigosBuffer->myinfo->idGoomba=hiloEnemigos;
		list_add(listaEnemigos,infoParaEnemigosBuffer->myinfo);

		i++;
	}

	//actualizarNivel(config.listaCajas,listaEnemigos,listaJugadoresActivos,config.nombre);
	coordenadas recorridoEnemigos[config.enemigos][4];
	//actualizarNivel(config.listaCajas,listaEnemigos,listaJugadoresActivos,config.nombre);
		//nivel_gui_terminar();
	{
		int i=0,j=0;
		for (i=0;i<config.enemigos;i++){
			for(j=0;j<4;j++){
				recorridoEnemigos[i][j].posx=0;
				recorridoEnemigos[i][j].posy=0;
			}
		}
	}


//controlEnemigos(&infoParaEnemigos);


//handshakePlataforma(datosConexiones);
while(1){
	//actualizarNivel(config.listaCajas,listaEnemigos,listaJugadoresActivos,config.nombre);//este while esta para evitar q el main finalice mientras el hilo se ejecuta,proximamente aca va el resto de la implementacion del programa
}
//hiloEnemigos=hiloGRID(&controlEnemigos,&infoParaEnemigos);

/*
	while(1){

	usleep(config.sleepEnemigos*1000);
	moverEnemigos(&listaEnemigos,&config.listaCajas,&listaJugadoresActivos,recorridoEnemigos,rows,cols);
	actualizarNivel(config.listaCajas,listaEnemigos,listaJugadoresActivos,config.nombre);

	};
*/
//	nivel_gui_terminar();
	//actualizarNivel(config.listaCajas,listaEnemigos,listaJugadoresActivos,config.nombre);

	//sleep(2);

	//int as;
	//scanf("%d\n",&as);
	//flush_in();
	//scanf("%d\n",&as);



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

