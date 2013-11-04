#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <commons/config.h>
#include <commons/collections/list.h>
#include <cadenas.h>
#include <general.h>
#include <sockets.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>

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
	int orquestadorPort;
	char *orquestadorIP[];
}tpersonaje;

int sockfd,socketEscucha,personajeCargado,nuevo;
tpersonaje personaje;
char *recurso;
t_list *lista,*listaRecursos;
pid_t pid;
pthread_t hilo;
char * path;


//Prototipos
int vidasPersonaje( t_config * cfgPersonaje);
char * identificadorPersonaje ( t_config * cfgPersonaje);
char * nombrePersonaje ( t_config * cfgPersonaje);
char * orquestador( t_config * cfgPersonaje);
char ** niveles( t_config * cfgPersonaje);
char ** recursos( t_config * cfgPersonaje, char *nivel);
int cargaPersonaje(char *argv[]);
void * jugar(void* niveles);




int main(int argc, char *argv[]) {
	//path="/home/utnso/workspace/Personaje/mario.cfg"; //De prueba
	personajeCargado=cargaPersonaje(&path);
	t_list *nivelesParametro;
	tpersonaje personajeHilo;
	int i;
	while(1){
		for(i=0;i<personaje.planDeNiveles->elements_count;i++){
			nivelesParametro=list_get(personaje.planDeNiveles->head->data,i);
			personajeHilo.planDeNiveles=nivelesParametro;
			hilo=hiloGRID(jugar,(void*)&personajeHilo);
			pthread_join(hilo, NULL);
		}

	}
	return 0;
};


void *jugar (void *personaje){
	answer * answerPlanificador;
	//Recastea el parametro al tipo original
	tpersonaje *infoBis=(tpersonaje*)personaje;
	tpersonaje info= *infoBis;
	sockfd=connectGRID(info.orquestadorPort,*info.orquestadorIP);
	socketEscucha=listenGRID(info.orquestadorPort);
	sendHandshake('1',info.nombre,*info.simbolo,socketEscucha);
	nuevo=atoi(recvAnswer(answerPlanificador,(short)socketEscucha));
	switch(nuevo){
		case 0: //Desconexion del nivel

			break;
		case 1: //Conexion al nivel
			while(1){

			}
			break;
	}



	return NULL;
};


/*
 * Carga el personaje en el
 * struct personaje para su uso
 *
 */
int cargaPersonaje(char *argv[]){

	t_config * cfgPersonaje;
	cfgPersonaje=config_create(argv[0]);
	tinfo *infoNivel;
	infoNivel=malloc(sizeof(tinfo));
	*personaje.orquestadorIP=malloc(sizeof(*personaje.orquestadorIP));

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
	if (config_has_property(cfgPersonaje,"orquestador")){//Si tiene cargado el orquestador lo mete en la variable orquestador
		char *orquestadorAux[21],*aux1,*aux2;
		*orquestadorAux=orquestador(cfgPersonaje);
		aux1=(strchr(orquestadorAux[0],':'))+1;
		aux2=string_substring_until(orquestadorAux[0],strlen(orquestadorAux[0])-strlen(((aux1)))-1);
		personaje.orquestadorPort=atoi(aux1);
		strcpy(*personaje.orquestadorIP,aux2);
		printf("El orquestador esta en la IP: %s y el puerto: %i.\n", personaje.orquestadorIP[0],personaje.orquestadorPort);
	}
	if (config_has_property(cfgPersonaje,"planDeNiveles")){ //Si tiene el plan de niveles cargado lo mete en los nodos de niveles en la lista
		char ** planNivelesPersonaje=niveles(cfgPersonaje);
		char ** recursosNivel;
		int i,j,tamanioArrayNivel,contador,tamanioArrayRecursos;
		personaje.planDeNiveles=(t_list*)malloc(sizeof(t_list));
		tinfo *ptrAux;

		lista=list_create();

		tamanioArrayNivel=sizeof(niveles(cfgPersonaje))-1;
		for (i=0;i<tamanioArrayNivel;i++){ //Carga en nodos los niveles
				printf("Se carga el nivel: %s\n", *(planNivelesPersonaje+i));
				strcpy(infoNivel->nivel,*(planNivelesPersonaje+i));
				recursosNivel=recursos(cfgPersonaje,infoNivel->nivel);
				listaRecursos=list_create();
				tamanioArrayRecursos=sizeof(recursosNivel)-1;
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

