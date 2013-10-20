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
#include <commons/collections/list.h>


main(){
	nivelConfig config;
	cargarconfig(&config);

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
	printf("%s\n",(*buffer).itemName);

	return 0;
}

