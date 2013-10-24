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
#include <commons/collections/list.h>
#include <time.h>
#include <sockets.h>

main(){
	nivelConfig config;
	t_list listaEnemigos;
	t_list listaJugadoresActivos;
	listaJugadoresActivos=*list_create();//provisorio hasta que un proceso se encargue de crearla
	int rows,cols;
	cargarConfig(&config);
	inicializarNivel(config,&rows,&cols);//Crea el nivel por primera vez,carga las cajas y devuelve el tamaño de la pantalla
	crearEnemigos(config,&listaEnemigos,rows,cols);

	sleep(1);
	actualizarNivel(config.listaCajas,listaEnemigos,listaJugadoresActivos,config.nombre);

	int as;
	scanf("%d\n",&as);
	//flush_in();
	scanf("%d\n",&as);
	nivel_gui_terminar();

	/*
	coordenadas* buffer;
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

	/*
	puts("Main program bitch");
	printf("El nombre del Nivel es %s\n",config.nombre);
	printf("Los datos de conexion son %s\n",config.orquestador);
	printf("El interveo de cheque de Deadlock es %d\n",config.intervaloDeadLock);
	printf("El revery se encuentra en estado %d\n",config.recovery);
	printf("Hay %d enemigos\n",config.enemigos);
	printf("El tiempo de retardo de los enemigos es %d\n",config.sleepEnemigos);
	printf("Se usa una planificacion del tipo %s\n",config.algoritmo);
	printf("El tiempo de quantum es %d\n",config.quantum);
	printf("El tiempo de retardo es %d\n",config.retardo);
	printf("Hay %d cajas de recursos \n",list_size(&config.listaCajas));

	Caja* buffer;

	buffer=list_get(&config.listaCajas,2);
	printf("%c\n",(*buffer).id);
*/
	return 0;
}

