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
#include <commons/collections/list.h>

char *path;
t_config config;
char prueba[20];
//int sleep; Este int me genera Segmentatio Fault, por el nombre
t_list cajas;


void reverse(char s[])
{
     int i, j;
     char c;

     for (i = 0, j = strlen(s)-1; i<j; i++, j--) {
         c = s[i];
         s[i] = s[j];
         s[j] = c;
     }
}

//funcion que le paso un int y devuelve un string
void itoa(int n, char s[])
{
     int i, sign;

     if ((sign = n) < 0)
         n = -n;
     i = 0;
     do {
         s[i++] = n % 10 + '0';
     } while ((n /= 10) > 0);
     if (sign < 0)
         s[i++] = '-';
     s[i] = '\0';
     reverse(s);
}
//me da vuelta el string



void cargarNombre(t_config config,char** nombre,int* valCar){
	if (config_has_property(&config,"Nombre")){
		*nombre=(char*)malloc(strlen(config_get_string_value(&config,"Nombre"))+1);
		strcpy(*nombre,config_get_string_value(&config,"Nombre"));
		(*valCar)++;

	}
	else {
		puts("Falta el nombre del nivel");
		exit(1);
	};
	}
void cargarOrquestador(t_config config,char** orquestador,int* valCar){
	if (config_has_property(&config,"orquestador")){
		*orquestador=(char*)malloc(strlen(config_get_string_value(&config,"orquestador"))+1);
		strcpy(*orquestador,config_get_string_value(&config,"orquestador"));
		(*valCar)++;
	}
	else {

		puts("Falta los datos de conexion al orquestador");
		exit(1);
	};
	}
void cargarDeadlock(t_config config,int* deadlock,int* valCar){
	if (config_has_property(&config,"TiempoChequeoDeadlock")){
		*deadlock=config_get_int_value(&config,"TiempoChequeoDeadlock");
		(*valCar)++;
	}
	else {
		puts("Falta el intervalo de deadlock");
		exit(1);
	};
	}
void cargarRecovery(t_config config,int* recovery,int* valCar){
	if (config_has_property(&config,"Recovery")){
		*recovery=config_get_int_value(&config,"Recovery");
		(*valCar)++;
	}
	else {
		puts("Falta el estado de recovery");
		exit(1);
	};
	}
void cargarEnemigos(t_config config,int* enemigos,int* valCar){
	if (config_has_property(&config,"Enemigos")){
		*enemigos=config_get_int_value(&config,"Enemigos");
		(*valCar)++;
	}
	else {
		puts("No hay datos sobre los enemigos");
		exit(1);
	};
	}
void cargarSleepEnemigos(t_config config,int* sleep,int* valCar){
	if (config_has_property(&config,"Sleep_Enemigos")){
		*sleep=config_get_int_value(&config,"Sleep_Enemigos");
		(*valCar)++;
	}
	else {
		puts("Falta el tiempo de sleep de los enemigos");
		exit(1);
	};
	}
void cargarAlgoritmo(t_config config,char** algoritmo,int* valCar){
	if (config_has_property(&config,"algoritmo")){
		*algoritmo=(char*)malloc(strlen(config_get_string_value(&config,"algoritmo"))+1);
		strcpy(*algoritmo,config_get_string_value(&config,"algoritmo"));
		(*valCar)++;
	}
	else {
		puts("Falta el tipo de algoritmo a usar");
		exit(1);
	};
	}
void cargarQuantum(t_config config,int* quantum,int* valCar){
	if (config_has_property(&config,"quantum")){
		*quantum=config_get_int_value(&config,"quantum");
		(*valCar)++;
	}
	else {
		puts("Falta el tiempo de quantum para el algoritmo de Round Robin");
		exit(1);
	};
	}
void cargarRetardo(t_config config,int* retardo,int* valCar){
	if (config_has_property(&config,"retardo")){
		*retardo=config_get_int_value(&config,"retardo");
		(*valCar)++;
	}
	else {
		puts("Falta el tiempo de retardo");
		exit(1);
	};
	}


void cargarRemainingDistance(t_config config,int* distance,int* valCar){
	if (config_has_property(&config,"remainingDistance")){
		*distance=config_get_int_value(&config,"remainingDistance");
		(*valCar)++;
	}
	else {
		puts("Falta la distancia inicial default");
		exit(1);
	};
	}


void cargarCajas(t_config config,t_list** listaCajas,int* valCar){
	//Los numeros de las cajas debes ser consecutivos,sino el algoritmo no los cargara
	int flag=1;
	int cont=1;
	char numCja[2];
	char caja[8];
	char** arrayCaja;
	char* aux;
	aux=(char*)malloc(75);
	arrayCaja=(char**)malloc(50);
	Caja cajaBuffer;              //En esta caja se van a volcar los valores de cada caja sacados directamente del archivo de config.
	*listaCajas=list_create();//LINEA DEL PANICO
	while(flag){

		strcpy(caja,"Caja");
		itoa(cont,numCja);
		strcat(caja,numCja);
		//printf("%s\n",caja);
		if (config_has_property(&config,caja)){

			strcpy(aux,"[");
			strcat(aux,config_get_string_value(&config,caja));
			strcat(aux,"]");
			arrayCaja=(char**)string_get_string_as_array(aux);
			cajaBuffer.itemName=(char*)malloc(strlen(arrayCaja[0])+1);
			strcpy(cajaBuffer.itemName,arrayCaja[0]);
			cajaBuffer.id=*arrayCaja[1];
			cajaBuffer.quantity=atoi(arrayCaja[2]);
			cajaBuffer.posx=atoi(arrayCaja[3]);
			cajaBuffer.posy=atoi(arrayCaja[4]);
			Caja* cajaTemp;  //Esta caja es creada para asignarle memoria con malloc, y asi podes guardar esa direccion de memoria en la lista
			cajaTemp=(Caja*)malloc((sizeof (Caja)));
			*cajaTemp=cajaBuffer;
			list_add(*listaCajas,cajaTemp);//despues para obtener los datos, se deben guardar en un puntero ya que devuelve un void*
			(*valCar)++;
			cont++;
		}
		else flag=0;
	//free(aux);
	//free(arrayCaja);

	};

if (cont==1){
	puts("El nivel no tiene cajas"); //si el contador al terminar vale 1,significa que no se cargo ni una caja
	exit(1);
}


}


verificarCargados(t_config config,int valCar){
	if(config_keys_amount(&config)==valCar){
		//printf("Se han cargado %d valores de %d\n",valCar,config_keys_amount(&config));
		//puts("Archivo de configuracion cargado exitosamente");
	}
	else{
		printf("Se han cargado %d valores de %d\n",valCar,config_keys_amount(&config));
		puts("Revise el archivo de configuracion, que no haya keys de mas y que las cajas esten bien numeradas en orden ascendente desde 1 sin saltear ningun numero, el programaa abortara");
		exit(1);
	}
}


comprobarSuperposicion(t_list* listaCajas){

	int posiciones[list_size(listaCajas)];

	int i=0;
	Caja* Buffer;
	for(i=0;i<list_size(listaCajas);i++){
		Buffer=list_get(listaCajas,i);
		posiciones[i]=((*Buffer).posx)*100+(*Buffer).posy;

		}
	i=0;
	int flag=0;
	int j=0;
	for(i=0;i<list_size(listaCajas);i++){
		for(j=i+1;j<list_size(listaCajas);j++){
			if(posiciones[i]==posiciones[j]){
				printf("La Caja%d se superpone con la Caja%d\n",i+1,j+1);
				flag=1;
			}
		}

	}
	if (flag==1){
		puts("Corrija las superposiciones y cargue el archivo de nuevo");
		exit(1);
	}
}


//Recibe la estructura donde va a guardar los datos
void cargarConfig(nivelConfig* configNivel){
	//puts("Por favor,ingrese la ruta del archivo de configuracion");
	t_config* config;
	char* path;
	path=(char*)malloc(250);
	strcpy(path,configNivel->path);
	nivelConfig configTemp; //archivo de configuracion temporal donde se guardan los datos extraidos del archivo de configuracion, luego al final se vuela en configNivel que retornara al main program
	configTemp.path=malloc(strlen(configNivel->path)+1);
	strcpy(configTemp.path,configNivel->path);
	config=config_create(path);
	free(path);
	int valCar=0;// cantidad de valores que se cargaron exitosamente
	//configTemp.listaCajas=list_create();
	cargarNombre(*config,&configTemp.nombre,&valCar);

	cargarCajas(*config,&configTemp.listaCajas,&valCar);

	cargarOrquestador(*config,&configTemp.orquestador,&valCar);
	cargarDeadlock(*config,&configTemp.intervaloDeadLock,&valCar);//carga el intervalo de Deadlock
	cargarRecovery(*config,&configTemp.recovery,&valCar);
	cargarEnemigos(*config,&configTemp.enemigos,&valCar);
	cargarSleepEnemigos(*config,&configTemp.sleepEnemigos,&valCar);
	cargarAlgoritmo(*config,&configTemp.algoritmo,&valCar);
	cargarQuantum(*config,&configTemp.quantum,&valCar);
	cargarRetardo(*config,&configTemp.retardo,&valCar);
	cargarRemainingDistance(*config,&configTemp.remainingDistance,&valCar);


	comprobarSuperposicion(configTemp.listaCajas);

	verificarCargados(*config,valCar);//compara cuantas keys se cargaron y cuantas tiene el archivo, si difieren,aborta

	*configNivel=configTemp; //paso final, carga los datos en configNivel antes de regresar al main program
	config_destroy(config);



}



