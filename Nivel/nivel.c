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

main(){
	pthread_mutex_t mutexEnemigos =PTHREAD_MUTEX_INITIALIZER;
	nivelConfig config;
	t_list listaEnemigos;
	t_list listaJugadoresActivos;
	t_list listaJugadoresMuertos;
	listaJugadoresActivos=*list_create();//provisorio hasta que un proceso se encargue de crearla
	listaJugadoresMuertos=*list_create();//idem
	crearPersonaje(&listaJugadoresActivos,4,4,'V');
	crearPersonaje(&listaJugadoresActivos,10,10,'@');
	crearPersonaje(&listaJugadoresActivos,20,20,'F');
	crearPersonaje(&listaJugadoresActivos,40,15,'A');
	int rows,cols;
	cargarConfig(&config);

	inicializarNivel(config,&rows,&cols);//Crea el nivel por primera vez,carga las cajas y devuelve el tamaño de la pantalla
	crearEnemigos(config,&listaEnemigos,rows,cols);
	actualizarNivel(config.listaCajas,listaEnemigos,listaJugadoresActivos,config.nombre);
	coordenadas recorridoEnemigos[config.enemigos][4];
	actualizarNivel(config.listaCajas,listaEnemigos,listaJugadoresActivos,config.nombre);
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

infoEnemigosThread infoParaEnemigos;
infoParaEnemigos.listaEnemigos=&listaEnemigos;

infoParaEnemigos.listaCajas=&config.listaCajas;
infoParaEnemigos.listaJugadoresActivos=&listaJugadoresActivos;
infoParaEnemigos.listaJugadoresMuertos=&listaJugadoresMuertos;
infoParaEnemigos.cantEne=list_size(&listaEnemigos);
infoParaEnemigos.rows=rows;
infoParaEnemigos.cols=cols;
infoParaEnemigos.sleepEnemigos=config.sleepEnemigos;
infoParaEnemigos.nombreNivel=malloc(sizeof(config.nombre));
strcpy(infoParaEnemigos.nombreNivel,config.nombre);

//controlEnemigos(&infoParaEnemigos);
pthread_t hiloEnemigos;
pthread_create(&hiloEnemigos,NULL,(void*)&controlEnemigos,(void*)&infoParaEnemigos);
while(1){
//este while esta para evitar q el main finalice mientras el hilo se ejecuta,proximamente aca va el resto de la implementacion del programa
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
	//handshakePlataforma(config);
	//sleep(2);

	//int as;
	//scanf("%d\n",&as);
	//flush_in();
	//scanf("%d\n",&as);





	return 0;
}

