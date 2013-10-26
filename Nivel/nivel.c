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

main(){
	nivelConfig config;
	t_list listaEnemigos;
	t_list listaJugadoresActivos;
	listaJugadoresActivos=*list_create();//provisorio hasta que un proceso se encargue de crearla
	int rows,cols;
	cargarConfig(&config);
	inicializarNivel(config,&rows,&cols);//Crea el nivel por primera vez,carga las cajas y devuelve el tamaño de la pantalla
	crearEnemigos(config,&listaEnemigos,rows,cols);
	actualizarNivel(config.listaCajas,listaEnemigos,listaJugadoresActivos,config.nombre);
	coordenadas recorridoEnemigos[config.enemigos][4];
	actualizarNivel(config.listaCajas,listaEnemigos,listaJugadoresActivos,"CHINGON");
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




	while(1){
sleep(1);
	moverEnemigos(&listaEnemigos,config.listaCajas,listaJugadoresActivos,recorridoEnemigos,rows,cols);
	coordenadas* buffer;
/*
	buffer=list_get(&listaEnemigos,0);
		printf("pos X :%d\n",(*buffer).posx);
		printf("pos Y :%d\n",(*buffer).posy);
		buffer=list_get(&listaEnemigos,1);
		printf("pos X :%d\n",(*buffer).posx);
		printf("pos Y :%d\n",(*buffer).posy);
		buffer=list_get(&listaEnemigos,2);
		printf("pos X :%d\n",(*buffer).posx);
		printf("pos Y :%d\n",(*buffer).posy);
*/
	actualizarNivel(config.listaCajas,listaEnemigos,listaJugadoresActivos,"CHINGON");

	};

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

