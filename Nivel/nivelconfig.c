/*
 * main.c
 *
 *  Created on: 30/04/2013
 *      Author: utnso
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <curses.h>
#include "commons/config.h"
#include "commons/string.h"
#include "nivelconfig.h"

//PROTOTIPO DE FUNCIONES


char * nombre ( t_config * cfg);

char * orquestador( t_config * cfg);

int chequeoDeadLock (t_config *cfg);

int llenarCajas (t_config *cfg, Caja **);

int recovery( t_config * cfg);

int enemigos(t_config * cfg);

int sleepEnemigos( t_config * cfg);

char* algoritmo( t_config * cfg);

int quantum( t_config * cfg);

int quantum( t_config * cfg);

int retardo( t_config * cfg);

void generarCaja(t_config * cfg, char* box, Caja * cajaGenerada);

char** config_get_box_array_value(char* value_in_dictionary);

int cantKeysString (char* str);

char * listar( t_config * cfg, char* data);

char ** itemsLista( t_config * cfg, char* key);

//PUSE UN ITOA IMPLEMENTADO
void itoa(int n, char s[]);
void reverse(char s[]);

//FIN DE PROTOTIPO DE FUNCIONES

int main2(int argc, char *argv[]) {
	int resultado;

	nivelConfig cfgNivel;

	resultado=obtenerConfiguracionNivel(argv[1],&cfgNivel);

	if(!resultado){
		printf("Archivo de configuracion \"%s\" cargado.\n",cfgNivel.nombre);

	}
	else
		printf("Se intento cargar el nivel pero Koopa se comio el archivo de configuracion\n.");


	return 0;
}


//Recibe las propiedades de un nivel y devuelve  el nombre
char * nombre ( t_config * cfg)
{
	return config_get_string_value(cfg,"Nombre");
}

//Recibe las propiedades de un nivel y devuelve  la direccion del orquestador como string ip:puerto
char * orquestador( t_config * cfg)
{
	return config_get_string_value(cfg,"orquestador");
}

//Recibe las propiedades de un nivel y devuelve el recovery
int recovery( t_config * cfg)
{
	return config_get_int_value(cfg,"Recovery");
}
//Recibe las propiedades de un nivel y devuelve la cantidad de enemigos
int enemigos( t_config * cfg)
{
	return config_get_int_value(cfg,"Enemigos");
}

//Recibe las propiedades de un nivel y devuelve cuantod milisegundos dormiran los enemigos
int sleepEnemigos( t_config * cfg)
{
	return config_get_int_value(cfg,"Sleep_Enemigos");
}
//Recibe las propiedades de un nivel y devuelve el algoritmo con cual fue cargado el nivel
char * algoritmo( t_config * cfg)
{
	return config_get_string_value(cfg,"algoritmo");
}
//Recibe las propiedades de un nivel y devuelve los quantos conlos que fue cargado el nivel
int quantum( t_config * cfg)
{
	return config_get_int_value(cfg,"quantum");
}
//Recibe las propiedades de un nivel y devuelve el tiempo de retardo con que se ejecuta el nivel
int retardo( t_config * cfg)
{
	return config_get_int_value(cfg,"retardo");
}


//Recibe las propiedades de un nivel y devuelve el puntero a los niveles
char ** itemsLista( t_config * cfg, char* key)
{
	return config_get_array_value(cfg,key);
}

//Recibe las propiedades de un nivel y devuelve plan de niveles en formato [N1,N2,..,NN]
char * listar( t_config * cfg, char* data)
{
	return config_get_string_value(cfg,data);
}

//Recibe un string en formato [key1,key2,..,keyN] y devuelve la cantidad de keys.
int cantKeysString (char* str)
{
	int cant=0,i;
	for (i=0; str[i]!='\0'; i++)
		if (str[i]==',')
			cant++;

	return cant+1;
}

//Recibe las propiedades de un nivel y devuelve el intervalo de chequeo de deadlock
int chequeoDeadLock (t_config *cfg)
{
	return config_get_int_value(cfg,"TiempoChequeoDeadlock");
}


// LLena una lista con todos los datos de cajas
int llenarCajas (t_config *cfg, Caja ** cajas)
{

	//char ** planNivelesPersonaje;
	//planNivelesPersonaje= (char **) malloc(strlen(listar(cfgPersonaje,dataPlanDeNivel))-(cantNiveles+1)); //Length Str-cantidad ',' + [ ]
	//planNivelesPersonaje=itemsLista(cfgPersonaje, dataPlanDeNivel);
	int i=1;
	int cargadas =0 ;
	//int dynAlloc=0;
	char * number;
	number= (char*) malloc (5000);

	char * box;

	box = (char *) malloc(5000);
	//Caja ** listaCajas;
	Caja * listaCajas[20];
	//listaCajas = (Caja ** )malloc(10000);

	Caja * punteroCaja;

	punteroCaja = (Caja * )malloc(5000);
	while(1){

		itoa(i,number);
		box= string_new();
		strcat(box,"Caja");
		strcat(box,number);


		if (config_has_property(cfg,box))
		{
			printf("¡Mas cajas! %s: ",box);

			generarCaja(cfg,box,punteroCaja+cargadas);
			////realloc(listaCajas,dynAlloc+(sizeof (int)))	;
			listaCajas[cargadas]=punteroCaja+cargadas;
			cargadas++;

		}
		else
			break;


		i++;
	};

	return cargadas;
}





//Devuelve un puntero a un struct con los datos de la caja leida
void generarCaja(t_config * cfg, char* box, Caja * cajaGenerada)
{
	cajaGenerada->itemName=(char*)malloc(5000);


	//	int cantNiveles =cantKeysString(listar(cfg,dataPlanDeNivel));


	char ** dataStruct;
	char * boxAux;
	boxAux = (char *) malloc(5000);


	strcat(boxAux, "[");
	strcat(boxAux, listar(cfg,box));
	strcat(boxAux, "]");

	printf(" %s. \n",boxAux);
	dataStruct= (char **) malloc(100);
	dataStruct=config_get_box_array_value(boxAux);

	/*printf("nombreItem: %s\n",dataStruct[0]);
	printf("nombrecorto: %s\n",dataStruct[1]);
	printf("instancias: %s\n",dataStruct[2]);
	printf("x: %s\n",dataStruct[3]);
	printf("y: %s\n",dataStruct[4]);*/

	strcpy((cajaGenerada->itemName),dataStruct[0]);
	cajaGenerada->id=(char)dataStruct[1];
	cajaGenerada->quantity=atoi(dataStruct[2]);
	cajaGenerada->posx=(int)atoi(dataStruct[3]);
	cajaGenerada->posy=(int)atoi(dataStruct[4]);}


char** config_get_box_array_value(char* value_in_dictionary) {
	return string_get_string_as_array(value_in_dictionary);
}

int obtenerConfiguracionNivel(char * path,nivelConfig * cfgNivel){


	t_config * archCfgNivel;
	printf("Inicializando archivo desde path: %s\n",path);

	if (access(path, R_OK))
	{

		//Loguear no se encontro archivo o no se poseen permisos de lectura;
		return -1;
	}


	archCfgNivel=config_create(path);

	//Nombre de nivel: Formato Nombre=<nombre. Ej. Nombre=nivel1

	if (config_has_property(archCfgNivel,"Nombre")){
		cfgNivel->nombre=(char*)malloc(strlen(nombre(archCfgNivel)));
		strcpy(cfgNivel->nombre,nombre(archCfgNivel));
		printf("Nombre de Nivel: %s.\n",cfgNivel->nombre);
	}
	else
	{
		//loguear nivel sin nombre
		return -1;
	}


	//Chequea que haya almenos alguna caja, sino estaria en deadlock permanente los personajes que vayan al nivel.

	if (config_has_property(archCfgNivel,"Caja1")){
		cfgNivel->cantidadCajas =	llenarCajas(archCfgNivel,cfgNivel->cajasNivel);
		printf("Se cargaron %d cajas en el nivel \"%s\"\n",cfgNivel->cantidadCajas,cfgNivel->nombre);
	}
	else {
		//loguear nivel sin cajas
		return -1;
	}





	if (config_has_property(archCfgNivel,"orquestador")){
		cfgNivel->orquestador=(char*)malloc(strlen(orquestador(archCfgNivel)));
		strcpy(cfgNivel->orquestador,orquestador (archCfgNivel));
		printf("Conectarse a: %s.\n", cfgNivel->orquestador);
	}
	else
	{
		//loguear nivel sin orquestador
		return -1;
	}

	if (config_has_property(archCfgNivel,"TiempoChequeoDeadlock"))
	{
		cfgNivel->intervaloDeadLock=chequeoDeadLock(archCfgNivel);
		printf("Chequeo de DeadLock (ms): %d.\n", cfgNivel->intervaloDeadLock);
	}

	else
	{
		//loguear nivel sin configuracion Dead Lock;
		return -1;
	}

	if (config_has_property(archCfgNivel,"Recovery"))
	{
		cfgNivel->recovery=recovery(archCfgNivel);
		printf("Recovery: %d.\n", cfgNivel->recovery);
	}
	else
	{
		//loguear nivel sin configuracion de Recovery
		return -1;
	}
	
	if (config_has_property(archCfgNivel,"Enemigos"))
	{
		cfgNivel->enemigos=enemigos(archCfgNivel);
		printf("Enemigos: %d.\n", cfgNivel->enemigos);
	}	
	else
	{
		//loguear, no hay enemigos
		return 0;
	}

	if (config_has_property(archCfgNivel,"Sleep_Enemigos"))
		{
			cfgNivel->sleepEnemigos=sleepEnemigos(archCfgNivel);
			printf("Sleeps de los Enemigos: %d.\n", cfgNivel->sleepEnemigos);
		}
		else
		{
			//loguear, no hay sleep
			return 0;
		}

	if (config_has_property(archCfgNivel,"algoritmo")){
			cfgNivel->algoritmo=(char*)malloc(strlen(algoritmo(archCfgNivel)));
			strcpy(cfgNivel->algoritmo,algoritmo (archCfgNivel));
			printf("uTiliza este algoritmo: %s.\n", cfgNivel->algoritmo);
		}
		else
		{
			//loguear nivel sin algoritmo
			return -1;
		}

	if (config_has_property(archCfgNivel,"quantum"))
			{
				cfgNivel->quantum=quantum(archCfgNivel);
				printf("quantum asignado: %d.\n", cfgNivel->quantum);
			}
			else
			{
				//loguear, no quantum ¡ho!
				return 0;
			}

	if (config_has_property(archCfgNivel,"retardo"))
			{
				cfgNivel->retardo=retardo(archCfgNivel);
				printf("tiempo de retardo: %d.\n", cfgNivel->retardo);
			}
			else
			{
				//loguear, si no hay retardo is mode hyper fast
				return 0;
			}
		

	config_destroy(archCfgNivel);

	return 0;

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

