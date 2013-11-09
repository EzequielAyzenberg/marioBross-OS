#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <commons/config.h>
#include <commons/collections/list.h>
#include <theGRID/general.h>
#include <theGRID/sockets.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>

//Variables
typedef struct info{
	char *nivel;
}tinfo;

typedef struct personaje {
	char *nombre;
	char simbolo;
	t_list *planDeNiveles;
	int vidas;
	int orquestadorPort;
	char *orquestadorIP[];
}tpersonaje;

typedef struct miniPersonaje{
	char simbolo;
	char *nivel;
	t_list *planDeRecursos;
	int orquestadorSocket;
	int posX;
	int posY;
}tminipersonaje;

typedef struct recurso{
	char tipo;
	int posX;
	int posY;
	bool checked;
}trecurso;

int sockfd,socketEscucha,personajeCargado,nuevo;
tpersonaje personaje;
char *recurso;
t_list *lista,*listaRecursos;
pid_t pid;
pthread_t hilo;
char * path;


//Prototipos
int vidasPersonaje( t_config * cfgPersonaje);
char identificadorPersonaje ( t_config * cfgPersonaje);
char * nombrePersonaje ( t_config * cfgPersonaje);
char * orquestador( t_config * cfgPersonaje);
char ** niveles( t_config * cfgPersonaje);
char ** recursos( t_config * cfgPersonaje, char *nivel);
int cargaPersonaje(char *argv[]);
void * jugar(void* niveles);
int cantidadElementosArray(char **array);
int estoyMuerto();
int actualizarRP();
int gestionTurno(t_list*,int,int,int,char);
int instanciaConc();
int movimientoConc();




int main(int argc, char *argv[]) {
	path="/home/utnso/GITHUB/tp-2013-2c-the-grid/Personaje/mario.cfg"; //De prueba
	personajeCargado=cargaPersonaje(&path);
	return 0;
};


void *jugar (void *minipersonaje){
	answer ordenPlanificador;
	int esInstancia;
	//Recastea el parametro al tipo original
	tminipersonaje *infoBis=(tminipersonaje*)minipersonaje;
	tminipersonaje info= *infoBis;
	info.posX=0;
	info.posY=0;
	while(info.planDeRecursos->elements_count>0){
		recvAnswer(&ordenPlanificador,info.orquestadorSocket);
		switch(ordenPlanificador.msg){
			case 8: //Estoy muerto
				estoyMuerto();
				break;
			case 1: //Instancia o movimiento concedido
				if(esInstancia==1)instanciaConc();
				else movimientoConc();
				break;
			case 2: //Actualizar RP
				actualizarRP();
				break;
			case 7: //Gestion turno
				esInstancia=gestionTurno(info.planDeRecursos,info.orquestadorSocket,info.posX,info.posY,info.simbolo);
				break;
			case 0: //Limbo
				return 0;
				break;
		}
	}



	return NULL;
};


/*
 * Carga el personaje en el
 * struct personaje para su uso
 *
 */
int cargaPersonaje(char *argv[]){
	answer conexionSaliente;
	tminipersonaje *miniPersonaje;
	t_config * cfgPersonaje;
	cfgPersonaje=config_create(argv[0]);
	tinfo *infoNivel;
	infoNivel=(tinfo*)malloc(sizeof(tinfo));
	miniPersonaje=(tminipersonaje*)malloc(sizeof(tminipersonaje));
	miniPersonaje->nivel=(char*)malloc(16);
	*personaje.orquestadorIP=malloc(sizeof(*personaje.orquestadorIP));

	if (config_has_property(cfgPersonaje,"vidas")){ //Si tiene cargadas las vidas las mete en la variable vidas
		personaje.vidas=vidasPersonaje(cfgPersonaje);
		printf("Vidas de personaje: %d.\n", personaje.vidas);
	} else return -1;

	if (config_has_property(cfgPersonaje,"nombre")){ //Si tiene cargado el nombre lo mete en la variable nombre
		personaje.nombre=nombrePersonaje(cfgPersonaje);
		printf("Nombre de personaje: %s.\n", personaje.nombre);
	} else return -1;

	if (config_has_property(cfgPersonaje,"simbolo")){ //Si tiene cargado el simbolo lo mete en la variable simbolo
		personaje.simbolo=identificadorPersonaje(cfgPersonaje);
		miniPersonaje->simbolo=personaje.simbolo;
		printf("Identificador de personaje: %c.\n", personaje.simbolo);
	} else return -1;

	if (config_has_property(cfgPersonaje,"orquestador")){//Si tiene cargado el orquestador lo mete en la variable orquestador
		char *orquestadorAux[21],*aux1,*aux2;
		*orquestadorAux=orquestador(cfgPersonaje);
		aux1=(strchr(orquestadorAux[0],':'))+1;
		aux2=string_substring_until(orquestadorAux[0],strlen(orquestadorAux[0])-strlen(((aux1)))-1);
		personaje.orquestadorPort=atoi(aux1);
		strcpy(*personaje.orquestadorIP,aux2);
		printf("El orquestador esta en la IP: %s y el puerto: %i.\n", personaje.orquestadorIP[0],personaje.orquestadorPort);
	} else return -1;

	if (config_has_property(cfgPersonaje,"planDeNiveles")){ //Si tiene el plan de niveles cargado lo mete en los nodos de niveles en la lista
		char ** planNivelesPersonaje=niveles(cfgPersonaje);
		printf("El primer nivel es %s\n",planNivelesPersonaje[0]);
		char ** recursosNivel;
		int i,j,tamanioArrayNivel,contador,tamanioArrayRecursos,tamanioBuffer;
		personaje.planDeNiveles=(t_list*)malloc(sizeof(t_list));
		tamanioBuffer=cantidadElementosArray(planNivelesPersonaje);
		printf("%d",tamanioBuffer);
		if(tamanioBuffer==0) return -1;
		tinfo *ptrAux;
		lista=list_create();
		tamanioArrayNivel=cantidadElementosArray(planNivelesPersonaje);
		for (i=0;i<tamanioArrayNivel;i++){ //Carga en nodos los niveles
				printf("Se carga el nivel: %s\n", *(planNivelesPersonaje+i));
				//Funca
				strcpy(infoNivel->nivel,*(planNivelesPersonaje+i));
				strcpy(miniPersonaje->nivel,infoNivel->nivel);
				recursosNivel=recursos(cfgPersonaje,infoNivel->nivel);
				if((cantidadElementosArray(recursosNivel))==0)return -1;
				listaRecursos=list_create();
				tamanioArrayRecursos=cantidadElementosArray(recursosNivel);
				for (j=0;j<tamanioArrayRecursos;j++){
				     trecurso*temp;
				     temp=(trecurso*)malloc(sizeof(trecurso));
				     temp->tipo=(char)*(recursosNivel[j]);
				     temp->posX=-1;
				     temp->posY=-1;
				     temp->checked=false;
				     list_add(listaRecursos,temp);
				     if (j==0) miniPersonaje->planDeRecursos=listaRecursos;
				     printf("Cargando recurso: %c\n",temp->tipo);
				    }
				contador=list_add(lista,(void*)infoNivel);
				if(i==0) personaje.planDeNiveles=lista;
				ptrAux=list_get(lista,contador);
				printf("Nivel cargado: %s\n",ptrAux->nivel);
				char *temp2;
				for (j=0;j<tamanioArrayRecursos;j++){
				  	temp2=(char*)list_get(miniPersonaje->planDeRecursos,j);
				   	printf("Recurso cargado: %c\n",*temp2);
				}
				sockfd=connectGRID(personaje.orquestadorPort,*personaje.orquestadorIP);
				socketEscucha=listenGRID(personaje.orquestadorPort);
				sendHandshake(1,personaje.nombre,miniPersonaje->simbolo,socketEscucha);
				recvAnswer(&conexionSaliente,socketEscucha);
				if(conexionSaliente.msg==-1){
					return -1;
				}
				miniPersonaje->orquestadorSocket=socketEscucha;
				hilo=hiloGRID(jugar,(void*)&miniPersonaje);
		}
	}else return -1;
	config_destroy(cfgPersonaje);
	return EXIT_SUCCESS;
}

int vidasPersonaje( t_config * cfgPersonaje)
{
	return config_get_int_value(cfgPersonaje,"vidas");
}

char identificadorPersonaje ( t_config * cfgPersonaje)
{
	return *config_get_string_value(cfgPersonaje,"simbolo");

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

int cantidadElementosArray(char **array){
	int tamanioArray,tamanioElemento;
	tamanioElemento=sizeof(array[0]);
	tamanioArray=strlen(array);
	return tamanioArray/tamanioElemento;
}

/*
 * Baja una vida en el padre
 *
 */
int estoyMuerto(){
	personaje.vidas-=1;
	return 0;
}

int actualizarRP(){

	return 0;
}

int gestionTurno(t_list * planDeRecursos,int sockfd,int posX,int posY,char simbolo){
	trecurso *recursoSiguiente;
	recursoSiguiente=(trecurso*)malloc(sizeof(trecurso));
	if(planDeRecursos->elements_count!=0){
		recursoSiguiente=(trecurso*)list_get(planDeRecursos,1);
		if(recursoSiguiente->posX==-1 || recursoSiguiente->posY==-1){ //Pedir pos Recurso
			sendAnswer(2,0,recursoSiguiente->tipo,simbolo, sockfd);
			return 0;
		}
		if(recursoSiguiente->posX==posX && recursoSiguiente->posY==posY){
			sendAnswer(2,1,recursoSiguiente->tipo,simbolo,sockfd);//Mensaje de instancia?
			return 1;
		}
		//if(recursoSiguiente->posX!=posX) Muevete baby
	}
return 0;
}

int instanciaConc(){
	return 0;
}

int movimientoConc(){
	return 0;
}
