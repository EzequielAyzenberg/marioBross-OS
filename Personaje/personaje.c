#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <commons/config.h>
#include <commons/collections/list.h>
#include <cadenas.h>
#include <sockets.h>
#define PORT 2968

//Variables
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
char *path,*recurso;
t_list *lista,*listaRecursos;

//Prototipos
int vidasPersonaje( t_config * cfgPersonaje);
char * identificadorPersonaje ( t_config * cfgPersonaje);
char * nombrePersonaje ( t_config * cfgPersonaje);
char * orquestador( t_config * cfgPersonaje);
char ** niveles( t_config * cfgPersonaje);
char ** recursos( t_config * cfgPersonaje, char *nivel);
int cargaPersonaje(char *argv[]);




int main(int argc, char *argv[]) {
	path="/home/utnso/workspace/Personaje/mario.cfg"; //De prueba
	/*sockfd=connectGRID(PORT,*argv);
	while(1){
		socketEscucha=listenGRID(PORT);
		nuevo=acceptGRID(socketEscucha);
		//recvHandshake(handshake *,path);

	}*/
	personajeCargado=cargaPersonaje(&path);
	return 0;
};


int cargaPersonaje(char *argv[]){

	t_config * cfgPersonaje;
	cfgPersonaje=config_create(argv[0]);
	tinfo *infoNivel;
	infoNivel=malloc(sizeof(tinfo));
	//tinfoNivel recursosNivel;

	if (config_has_property(cfgPersonaje,"vidas")){ //Si tiene cargadas las vidas las mete en la variable vidas
		personaje.vidas=vidasPersonaje(cfgPersonaje);
		printf("Vidas de personaje: %d.\n", personaje.vidas);
	}
	if (config_has_property(cfgPersonaje,"nombre")){ //Si tiene cargado el nombre lo mete en la variable nombre
		personaje.nombre=nombrePersonaje(cfgPersonaje);
		printf("Nombre de personaje: %s.\n", personaje.nombre);
	}
	if (config_has_property(cfgPersonaje,"simbolo")){ //Si tiene cargado el simbolo lo mete en la variable simbolo
		personaje.simbolo=identificadorPersonaje(cfgPersonaje);
		printf("Identificador de personaje: %s.\n", personaje.simbolo);
	}
	if (config_has_property(cfgPersonaje,"orquestador")){ //Si tiene cargado el orquestador lo mete en la variable orquestador
		*personaje.orquestador=orquestador(cfgPersonaje);
		printf("El orquestador esta en: %s.\n", *personaje.orquestador);
	}
	if (config_has_property(cfgPersonaje,"planDeNiveles")){ //Si tiene el plan de niveles cargado lo mete en los nodos de niveles en la lista
		char ** planNivelesPersonaje=niveles(cfgPersonaje);
		char ** recursosNivel;
		int i,j,tamanioArrayNivel,contador,tamanioArrayRecursos;
		personaje.planDeNiveles=(t_list*)malloc(sizeof(t_list));
		tinfo *ptrAux;
		char *ptrAux2;
		char recursoNivel[20];

		lista=list_create();

		tamanioArrayNivel=sizeof(niveles(cfgPersonaje))-1;
		for (i=0;i<tamanioArrayNivel;i++){ //Carga en nodos los niveles
				printf("Se carga el nivel: %s\n", *(planNivelesPersonaje+i));
				strcpy(infoNivel->nivel,*(planNivelesPersonaje+i));
				recursosNivel=recursos(cfgPersonaje,infoNivel->nivel);
				listaRecursos=list_create();
				tamanioArrayRecursos=sizeof(recursosNivel)-1;
/*				for (j=0;j<tamanioArrayRecursos;j++){
					list_add(listaRecursos,(void**)(recursosNivel+j));
					if (j==0) infoNivel->recursos=listaRecursos;
					printf("Cargando recurso: %s\n",*(recursosNivel+j));
				}*/
				for (j=0;j<tamanioArrayRecursos;j++){
				     char*temp;
				     temp=(char*)malloc(sizeof(char));
				     *temp=(char)**(recursosNivel+j);
				     list_add(listaRecursos,temp);
				     if (j==0) infoNivel->recursos=listaRecursos;
				     printf("Cargando recurso: %c\n",*temp);
				    }
				contador=list_add(lista,(void*)infoNivel);
				if(i==0) personaje.planDeNiveles=lista;
				ptrAux=list_get(lista,contador);
				printf("Nivel cargado: %s\n",ptrAux->nivel);
				char *temp2;
				    for (j=0;j<tamanioArrayRecursos;j++){
				     temp2=(char*)list_get(ptrAux->recursos,j);
				     printf("Recurso cargado: %c\n",*temp2);
				    }
				/*for (j=0;j<tamanioArrayRecursos;j++){
					ptrAux2=list_get(ptrAux->recursos,j);
					strcpy(recursoNivel,ptrAux2);
					printf("Recurso cargado: %s \n",recursoNivel);
				}*/
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

char ** recursos( t_config * cfgPersonaje, char *nivel)
{
	char cadenaAuxiliar[20];
	char cadenaFinal[20];
	strcpy(cadenaAuxiliar,"obj[");
	strcat(cadenaAuxiliar+4,nivel);
	strcat(cadenaAuxiliar+strlen(nivel)+4,"]");
	strcpy(cadenaFinal,cadenaAuxiliar);
	return config_get_array_value(cfgPersonaje,cadenaFinal);
}

