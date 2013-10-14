/*
 * cargador.c
 *
 *  Created on: 12/10/2013
 *      Author: utnso
 */
#include <stdio.h>
#include <commons/config.h>
#include "cargador.h"
#include <string.h>

char *path;
t_config config;
char prueba[20];
int sleep;


void cargarNombre(t_config config,nivelConfig configNivel){
	if (config_has_property(&config,"Nombre")){
		configNivel.nombre=(char*)malloc(strlen(config_get_string_value(&config,"Nombre")));
		strcpy(configNivel.nombre,config_get_string_value(&config,"Nombre"));
		}

	}



//Recibe la direccion del archivo de configuracion y la estructura donde va a guardar los datos
void cargarconfig(char *path,nivelConfig configNivel){
	config=*config_create(path);
	cargarNombre(config,configNivel);
	configNivel.algoritmo=(char*)malloc(strlen(config_get_string_value(&config,"algoritmo")));
	strcpy(configNivel.algoritmo,config_get_string_value(&config,"algoritmo"));
	sleep=config_get_int_value(&config,"Sleep_Enemigos");
	configNivel.sleepEnemigos=sleep;
	printf("%s\n",configNivel.algoritmo);
	printf("%d\n",configNivel.sleepEnemigos);


}


