/*
 * main.c
 *
 *  Created on: 30/04/2013
 *      Author: utnso
 */
#include <stdio.h>
#include <stdlib.h>
#include "commons/config.h"

int vidasPersonaje( t_config * cfgPersonaje);

char * identificadorPersonaje ( t_config * cfgPersonaje);

char * nombrePersonaje ( t_config * cfgPersonaje);

char * orquestador( t_config * cfgPersonaje);

char ** niveles( t_config * cfgPersonaje);

int main(int argc, char *argv[]) {

	t_config * cfgPersonaje;

	cfgPersonaje=config_create(argv[1]);

	if (config_has_property(cfgPersonaje,"vidas"))
		printf("Esto anda :-D.\n");



	printf("Cantidad de keys %d.\n", config_keys_amount(cfgPersonaje));
	if (config_has_property(cfgPersonaje,"nombre"))
		printf("Nombre de personaje: %s.\n", nombrePersonaje(cfgPersonaje));
	if (config_has_property(cfgPersonaje,"simbolo"))
		printf("Identificador de personaje: %s.\n", identificadorPersonaje(cfgPersonaje));
	if (config_has_property(cfgPersonaje,"vidas"))
		printf("Vidas de personaje: %d.\n", vidasPersonaje(cfgPersonaje));
	if (config_has_property(cfgPersonaje,"orquestador"))
		printf("ve por tus niveles chango a: %s.\n", orquestador (cfgPersonaje));
	if (config_has_property(cfgPersonaje,"planDeNiveles"))
	{
		char ** planNivelesPersonaje=niveles(cfgPersonaje);
		/*int i;
		for (i=0;i<dictionary_size(planNivelesPersonaje);i++)
			printf("%s\n", *(planNivelesPersonaje+i));*/

		/*Mori en encontrar el fin de las key de nivel*/
	}
	config_destroy(cfgPersonaje);
	return EXIT_SUCCESS;
}

int vidasPersonaje( t_config * cfgPersonaje)
{
	return config_get_int_value(cfgPersonaje,"vidas");
}

char * identificadorPersonaje ( t_config * cfgPersonaje)
{
	return config_get_string_value(cfgPersonaje,"simbolo");

}

char * nombrePersonaje ( t_config * cfgPersonaje)
{
	return config_get_string_value(cfgPersonaje,"nombre");
}

char * orquestador( t_config * cfgPersonaje)
{
	return config_get_string_value(cfgPersonaje,"orquestador");
}

char ** niveles( t_config * cfgPersonaje)
{
	return config_get_array_value(cfgPersonaje,"planDeNiveles");
}


