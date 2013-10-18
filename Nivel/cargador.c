/*
 * cargador.c
 *
 *  Created on: 12/10/2013
 *      Author: utnso
 */
#include <stdio.h>
#include <stdlib.h>
#include <commons/config.h>
#include "cargador.h"
#include <string.h>
#include <malloc.h>

char *path;
t_config config;
char prueba[20];
int sleep;


void cargarNombre(t_config config,char** nombre){
	if (config_has_property(&config,"Nombre")){
		*nombre=(char*)malloc(strlen(config_get_string_value(&config,"Nombre")));
		strcpy(*nombre,config_get_string_value(&config,"Nombre"));
	}
	else {
		puts("Falta el nombre del nivel");
		exit(1);
	};
	}
void cargarOrquestador(t_config config,char** orquestador){
	if (config_has_property(&config,"orquestador")){
		*orquestador=(char*)malloc(strlen(config_get_string_value(&config,"orquestador")));
		strcpy(*orquestador,config_get_string_value(&config,"orquestador"));
	}
	else {

		puts("Falta los datos de conexion al orquestador");
		exit(1);
	};
	}
void cargarDeadlock(t_config config,int* deadlock){
	if (config_has_property(&config,"TiempoChequeoDeadlock")){
		*deadlock=config_get_int_value(&config,"TiempoChequeoDeadlock");
	}
	else {
		puts("Falta el intervalo de deadlock");
		exit(1);
	};
	}
void cargarRecovery(t_config config,int* recovery){
	if (config_has_property(&config,"Recovery")){
		*recovery=config_get_int_value(&config,"Recovery");
	}
	else {
		puts("Falta el estado de recovery");
		exit(1);
	};
	}
void cargarEnemigos(t_config config,int* enemigos){
	if (config_has_property(&config,"Enemigos")){
		*enemigos=config_get_int_value(&config,"Enemigos");
	}
	else {
		puts("No hay datos sobre los enemigos");
		exit(1);
	};
	}
void cargarSleepEnemigos(t_config config,int* sleep){
	if (config_has_property(&config,"Sleep_Enemigos")){
		*sleep=config_get_int_value(&config,"Sleep_Enemigos");
	}
	else {
		puts("Falta el tiempo de sleep de los enemigos");
		exit(1);
	};
	}
void cargarAlgoritmo(t_config config,char** algoritmo){
	if (config_has_property(&config,"algoritmo")){
		*algoritmo=(char*)malloc(strlen(config_get_string_value(&config,"algoritmo")));
		strcpy(*algoritmo,config_get_string_value(&config,"algoritmo"));
	}
	else {
		puts("Falta el tipo de algoritmo a usar");
		exit(1);
	};
	}
void cargarQuantum(t_config config,int* quantum){
	if (config_has_property(&config,"quantum")){
		*quantum=config_get_int_value(&config,"quantum");
	}
	else {
		puts("Falta el tiempo de quantum para el algoritmo de Round Robin");
		exit(1);
	};
	}
void cargarRetardo(t_config config,int* retardo){
	if (config_has_property(&config,"retardo")){
		*retardo=config_get_int_value(&config,"retardo");
	}
	else {
		puts("Falta el tiempo de retardo");
		exit(1);
	};
	}

//Recibe la direccion del archivo de configuracion y la estructura donde va a guardar los datos
void cargarconfig(char *path,nivelConfig configNivel){
	config=*config_create(path);
	cargarNombre(config,&configNivel.nombre);
	cargarOrquestador(config,&configNivel.orquestador);
	cargarDeadlock(config,&configNivel.intervaloDeadLock);//carga el intervalo de Deadlock
	cargarRecovery(config,&configNivel.recovery);
	cargarEnemigos(config,&configNivel.enemigos);
	cargarSleepEnemigos(config,&configNivel.sleepEnemigos);
	cargarAlgoritmo(config,&configNivel.algoritmo);
	cargarQuantum(config,&configNivel.quantum);
	cargarRetardo(config,&configNivel.retardo);


	configNivel.algoritmo=(char*)malloc(strlen(config_get_string_value(&config,"algoritmo")));
	strcpy(configNivel.algoritmo,config_get_string_value(&config,"algoritmo"));
	sleep=config_get_int_value(&config,"Sleep_Enemigos");
	configNivel.sleepEnemigos=sleep;
	printf("%s\n",configNivel.nombre);
	printf("%s\n",configNivel.orquestador);
	printf("%d\n",configNivel.intervaloDeadLock);
	printf("%d\n",configNivel.recovery);
	printf("%d\n",configNivel.enemigos);
	printf("%d\n",configNivel.sleepEnemigos);
	printf("%s\n",configNivel.algoritmo);
	printf("%d\n",configNivel.quantum);
	printf("%d\n",configNivel.retardo);
}


