#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include "src/commons/config.h"
#include "src/commons/collections/list.h"
#include "sockets.h"
#define PORT 2968

//Variables
typedef struct infoNivel{
	char *recurso;
	int *sgte;
}tinfoNivel;

typedef struct info{
	char *nivel;
	t_list *recursos;
}tinfo;

typedef struct personaje {
	char *nombre;
	char *simbolo;
	t_list *planDeNiveles;
	int vidas;
	char *orquestador[];
}tpersonaje;



int sockfd,socketEscucha,personajeCargado,nuevo;
tpersonaje personaje;
char *path;
t_list *lista;

//Prototipos
int vidasPersonaje( t_config * cfgPersonaje);
char * identificadorPersonaje ( t_config * cfgPersonaje);
char * nombrePersonaje ( t_config * cfgPersonaje);
char * orquestador( t_config * cfgPersonaje);
char ** niveles( t_config * cfgPersonaje);
int cargaPersonaje(char *argv[]);



int main(int argc, char *argv[]) {
	sockfd=connectGRID(PORT,*argv);
	while(1){
		socketEscucha=listenGRID(PORT);
		nuevo=acceptGRID(socketEscucha);
		//recvHandshake(handshake *,path);

	}
	personajeCargado=cargaPersonaje(&path);
	return 0;
};


int cargaPersonaje(char *argv[]){

	t_config * cfgPersonaje;
	cfgPersonaje=config_create(argv[1]);
	tinfo infoNivel;
	tinfoNivel recursosNivel;
	if (config_has_property(cfgPersonaje,"vidas")){ //Si tiene cargadas las vidas las mete en la variable vidas
		printf("Vidas de personaje: %d.\n", vidasPersonaje(cfgPersonaje));
		personaje.vidas=vidasPersonaje(cfgPersonaje);
	}
	if (config_has_property(cfgPersonaje,"nombre")){ //Si tiene cargado el nombre lo mete en la variable nombre
		printf("Nombre de personaje: %s.\n", nombrePersonaje(cfgPersonaje));
		personaje.nombre=nombrePersonaje(cfgPersonaje);
	}
	if (config_has_property(cfgPersonaje,"simbolo")){ //Si tiene cargado el simbolo lo mete en la variable simbolo
		printf("Identificador de personaje: %s.\n", identificadorPersonaje(cfgPersonaje));
		personaje.simbolo=identificadorPersonaje(cfgPersonaje);
	}
	if (config_has_property(cfgPersonaje,"orquestador")){ //Si tiene cargado el orquestador lo mete en la variable orquestador
		printf("El orquestador esta en: %s.\n", orquestador (cfgPersonaje));
		*personaje.orquestador=orquestador(cfgPersonaje);
	}
	if (config_has_property(cfgPersonaje,"planDeNiveles")){ //Si tiene el plan de niveles cargado lo mete en los nodos de niveles en la lista
		char ** planNivelesPersonaje=niveles(cfgPersonaje);
		int i,tamanioArrayNivel,j;
		lista=list_create();
		tamanioArrayNivel=sizeof(niveles(cfgPersonaje))/sizeof(planNivelesPersonaje[0]);
		for (i=0;i<tamanioArrayNivel;i++){ //Carga en nodos los niveles sin los recursos
			printf("%s\n", *(planNivelesPersonaje+i));
			list_add(lista,planNivelesPersonaje[i]);
			if(i==0) personaje.planDeNiveles=lista;
			infoNivel.nivel=planNivelesPersonaje[i];
			/*while (1){
				if(strcmp(&infoNivel.nivel,&planNivelesPersonaje[j]){
					list_add(infoNivel.nivel,)
				}: Deberia cargar los recursos para cada nivel
			}*/
			list_get(personaje.planDeNiveles,(personaje.planDeNiveles->elements_count)-1); //Devuelve posicion de ultimo nodo para cargar recursos
			}
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


