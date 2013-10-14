/*
 * nivelconfig.h
 *
 *  Created on: 04/05/2013
 *      Author: utnso
 */

#ifndef NIVELCONFIG_H_
#define NIVELCONFIG_H_

typedef struct item {
	char * itemName;
	char  id;
	int posx;
	int posy;
	int quantity;
} Caja;

typedef struct ncfg {
	char * nombre;
	char* orquestador;
	int intervaloDeadLock;
	int cantidadCajas;
	Caja ** cajasNivel;
	int recovery;
	int enemigos;
	int sleepEnemigos;
	char* algoritmo;
	int quantum;
	int retardo;
} nivelConfig;

int obtenerConfiguracionNivel(char * path,nivelConfig * cfgNivel);

#endif /* NIVELCONFIG_H_ */
