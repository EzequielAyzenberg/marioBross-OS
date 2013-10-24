/*
 * cargador.h
 *
 *  Created on: 12/10/2013
 *      Author: utnso
 */
#ifndef CARGADOR_H_
#define CARGADOR_H_

#include <stdio.h>
#include <commons/config.h>
#include <commons/collections/list.h>

typedef struct Caja {
	char * itemName;
	char  id;
	int posx;
	int posy;
	int quantity;
} Caja;

typedef struct ncfg {
	char* nombre;
	char* orquestador;
	int intervaloDeadLock;
	int cantidadCajas;
	t_list listaCajas;
	Caja* cajasNivel;
	int recovery;
	int enemigos;
	int sleepEnemigos;
	char* algoritmo;
	int quantum;
	int retardo;
} nivelConfig;

//void cargarconfig(nivelConfig*,t_list*);

#endif /* CARGADOR_H_ */
