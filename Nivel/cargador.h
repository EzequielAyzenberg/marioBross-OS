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

typedef struct item {
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
	Caja** cajasNivel;
	int recovery;
	int enemigos;
	int sleepEnemigos;
	char* algoritmo;
	int quantum;
	int retardo;
} nivelConfig;

void cargarconfig(char *path,nivelConfig);
void cargarNombre(t_config config,nivelConfig configNivel);

#endif /* CARGADOR_H_ */
