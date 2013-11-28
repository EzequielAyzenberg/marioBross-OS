/*
 * personaje.c
 *
 *  Created on: 11/05/2013
 *      Author: utnso
 */

#include <stdio.h>
#include "personajeConfig.h"
#include <signal.h>

personajeConfig cfgPersonaje;

void vidas1up (int sig);
void adiosMundoCruel(void);


int main(void)
{
	signal(SIGUSR1,vidas1up);

	int resultado;

	resultado=obtenerConfiguracionPersonaje("../personajes/personaje.cfg",&cfgPersonaje);
	if(!resultado){
		printf("Archivo de configuracion del personaje \"%s\" cargado.\n",cfgPersonaje.nombre);

	}
	else
		printf("Se intento cargar el personaje pero Koopa se comio el archivo de configuracion\n.");

// Hasta que cfgPersonaje.proximoNivel= NULL
	//Conectarse al orquestador y pedir acceso a cfgPersonaje.proximoNivel
		//Conectarse a cfgPersonaje.proximoNivel
		// Hasta que cfgPersonaje.proximoNivel->proximoObjetivo = NULL
			// Pedir ubicacion de cfgPersonaje.proximoNivel->proximoObjetivo
			// Hasta llegar a cfgPersonaje.proximoNivel->proximoObjetivo
				// Esperar quantum del planificador
				// Realizar un movimiento (Mover en Nivel cfgPersonaje.proximoNivel)
			//Mover puntero de cfgPersonaje.proximoNivel->proximoObjetivo


	//Si se pierde conexion (kill Personaje)
		//Rollback punteros nivel y objetivo nivel (Personaje)
		adiosMundoCruel();

	return 0;
}

void vidas1up (int sig)
{
	cfgPersonaje.vidas++;
	printf("%s ahora tiene %d vidas.\n",cfgPersonaje.nombre, cfgPersonaje.vidas);
}

void adiosMundoCruel(void) // o "Bye bye" (lease con voz de gusanito del Worms http://www.youtube.com/watch?v=B4b48dRWm2U)
{
	printf("%s dijo: Adios mundo cruel.\n",cfgPersonaje.nombre);
	cfgPersonaje.proximoNivel=&cfgPersonaje.planDeNiveles[0];
	(*cfgPersonaje.proximoNivel).proximoObjetivo=&(*cfgPersonaje.proximoNivel).objetivos[0];
}
