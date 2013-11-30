#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <commons/config.h>
#include <commons/string.h>
#include <commons/collections/list.h>
#include <theGRID/general.h>
#include <theGRID/sockets.h>
#include <theGRID/cadenas.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>


//Variables
typedef struct hilos{
	int nombre;
	char *nivel;
}thilo;

typedef struct info{
	char *nivel;
}tinfo;

typedef struct personaje {
	char *nombre;
	char simbolo;
	t_list *planDeNiveles;
	int vidas;
	int orquestadorPort;
	char *orquestadorIP;
	t_list *miniPersonajes;
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


int sockfd,socketEscucha,personajeCargado,nuevo,ganado=0,finalizados=0,repetir=1,repeticiones=0,limboOK=0;
tpersonaje personaje;
char *recurso;
t_list *lista,*listaRecursos;
pid_t pid;
pthread_t hilo;
char * path;
char *nivelAux;



//Prototipos
int vidasPersonaje( t_config * cfgPersonaje);
char identificadorPersonaje ( t_config * cfgPersonaje);
char * nombrePersonaje ( t_config * cfgPersonaje);
char * orquestador( t_config * cfgPersonaje);
char ** niveles( t_config * cfgPersonaje);
char ** recursos( t_config * cfgPersonaje, char *nivel);

int cargaPersonaje(char *argv[]);
void * jugar(void*);
int cantidadElementosArray(char **);
int estoyMuerto(tminipersonaje*);
int actualizarRP(t_list*,int);
int gestionTurno(t_list*,int,int*,int*,char,int*,int*);
int instanciaConc(t_list*,int*);
int movimientoConc(int,int*,int*);
void cierraHilos();
void aumentaVida(int);
void restaVida(int);
void finDeNivel(int,char);



int main(int argc, char *argv[]) {
	personaje.miniPersonajes=(t_list*)malloc(sizeof(t_list));
	char* intentarlo;

	signal(SIGUSR1,aumentaVida);
	signal(SIGTERM,restaVida);

	path="/home/utnso/GITHUB/tp-2013-2c-the-grid/Personaje/mario.cfg"; //De prueba

	while(repetir==1){
		puts("Listos para arrancar?");
		sleep(1);
		personajeCargado=cargaPersonaje(argv);//ACA CAMBIE LO QUE LE MANDA
		if(personajeCargado==-1){
			printf("Error al cargar configuracion del personaje.\n");
			list_destroy(personaje.miniPersonajes);
			return 0;
		}
		while(personaje.miniPersonajes!=NULL && personaje.vidas>0){
			if(personaje.planDeNiveles->elements_count==ganado){
				int sockfdAux;
				printf("Ganamos! venga koopa, te hago frente\n");
				printf("Conectando con plataforma\n");
				sockfdAux=connectGRID(personaje.orquestadorPort,personaje.orquestadorIP);
				printf("Avisando a plataforma que soy un groso\n");
				sendHandshake(2,"",personaje.simbolo,(short)sockfdAux);
				printf("Ahora si, koopa, dame a la princesa\n");
				//list_destroy(personaje.miniPersonajes);
				//QUIZAS DEBERIA ESPERAR A VER COMO SALIO KOOPA.
				exit(0);
			}
			if(personaje.planDeNiveles->elements_count==finalizados){
				exit(1);//TAMBIEN DEBERIA DESTRUIR TODOS LOS NODOS!!!
			}
		}
			printf("Se han agotado todas las vidas, desea reintentarlo?: S/N: ");
			list_clean(personaje.miniPersonajes);
			leecad(intentarlo,1);
			if(!strcmp(intentarlo,"S")){ //hay que ponerle el admiracion xq el strcmp te lo devuelve al revez
				repeticiones++;
				printf("\nEste es tu reintento numero: %d\n",repeticiones);
			}else repetir=0;
	}
	cierraHilos();
	printf("Perdimos :(\n");
	list_destroy(personaje.miniPersonajes);
	return 0;
};

/*
 * Avisa que termino el nivel al orquestador
 * Cierra el socket
 *
 */

void finDeNivel(int sockfd,char simbolo){
	//sendAnswer(8,0,0,simbolo, sockfd); //Mensaje de fin de nivel--QUE CARAJO MANDANDOME UN 8!!!!
	close(sockfd);
}

/*
 * Suma una vida al personaje
 *
 */

void aumentaVida(int senial){
	printf("Se recibio la señal: %d",senial);
	personaje.vidas++;
}

/*
 * Resta una vida al personaje
 */
void restaVida(int senial){
	printf("Se recibio la señal: %d",senial);
	personaje.vidas--;
}

/*
 * Retorna true si el recurso fue agarrado
 *
 */

bool _recursoAgarrado(trecurso *recurso){
	return recurso->checked;
}

/*
 * Retorna true si el recurso no fue agarrado
 *
 */

bool _recursoNoAgarrado(trecurso *recurso){
	return !(recurso->checked);
}

/*
 * Gestiona las ordenes del planificador y delega a otras funciones
 * cada opcion por orden
 *
 */

void *jugar (void *minipersonaje){
	answer ordenPlanificador;
	int *esInstancia,*moverEnX,posicionNueva;
	moverEnX=(int*)malloc(sizeof(int));
	esInstancia=(int*)malloc(sizeof(int));
	*esInstancia=0;
	posicionNueva=0;
	*moverEnX=1;
	//Recastea el parametro al tipo original
	tminipersonaje *infoBis=(tminipersonaje*)minipersonaje;
	tminipersonaje info= *infoBis;
	info.posX=0;
	info.posY=0;
	printf("Soy la preOrden\n");
	while((list_any_satisfy(info.planDeRecursos,(void*)_recursoNoAgarrado))==true){
		recvAnswer(&ordenPlanificador,info.orquestadorSocket);
		printf("Orden del planificador: %d\n",ordenPlanificador.msg);
		printf("Vidas: %d\n",personaje.vidas);
		switch(ordenPlanificador.msg){
			case 8: //Estoy muerto
				printf("Estoy muerto\n");
				estoyMuerto(&info);
				if(personaje.vidas<=0) return 0;
				break;
			case 1: //Instancia o movimiento concedido
				printf("Es instancia?: %d\n",*esInstancia);
				if(*esInstancia==1){
					instanciaConc(info.planDeRecursos,esInstancia);
				}
				else movimientoConc(posicionNueva,&(info.posX),&(info.posY));
				break;
			case 2: //Actualizar RP
				printf("Actualiza RP\n");
				actualizarRP(info.planDeRecursos,ordenPlanificador.cont);
				break;
			case 7: //Gestion turno
				printf("Gestionemos el turno\n");
				printf("Mover en X: %d\n",*moverEnX);
				posicionNueva=gestionTurno(info.planDeRecursos,info.orquestadorSocket,&(info.posX),&(info.posY),info.simbolo,moverEnX,esInstancia);
				break;
			case 0: //Limbo
				printf("Se fue la plataforma, panico panico!\n");
				//cierraHilos();
				finDeNivel(info.orquestadorSocket,info.simbolo);
				finalizados++;
				return 0;
				break;
		}
	}
	printf("Ganamos bitches\n");
	ganado++;
	finDeNivel(info.orquestadorSocket,info.simbolo);
	return NULL;
};


/*
 * Carga el personaje en el
 * struct personaje para su uso
 * y crea los hilos para los miniPersonajes
 *
 */
int cargaPersonaje(char *argv[]){
	thilo *miniHilo;
	miniHilo=(thilo*)malloc(sizeof(thilo));
	miniHilo->nivel=(char*)malloc(16);
	answer conexionSaliente;
	tminipersonaje *miniPersonaje;
	t_config * cfgPersonaje;
	cfgPersonaje=config_create(argv[1]);//ACA CAMBIE LO QUE LE MANDA AL CONFIG
	tinfo *infoNivel;
	infoNivel=(tinfo*)malloc(sizeof(tinfo));
	miniPersonaje=(tminipersonaje*)malloc(sizeof(tminipersonaje));
	miniPersonaje->nivel=(char*)malloc(16);
	personaje.orquestadorIP=(char*)malloc(16);

	if (config_has_property(cfgPersonaje,"vidas")){ //Si tiene cargadas las vidas las mete en la variable vidas
		personaje.vidas=vidasPersonaje(cfgPersonaje);
		printf("Vidas de personaje: %d.\n", personaje.vidas);
	} else {
		printf("Archivo de configuracion incompleto, falta campo: Vidas\n");
		return -1;
	}

	if (config_has_property(cfgPersonaje,"nombre")){ //Si tiene cargado el nombre lo mete en la variable nombre
		personaje.nombre=nombrePersonaje(cfgPersonaje);
		printf("Nombre de personaje: %s.\n", personaje.nombre);
	} else {
		printf("Archivo de configuracion incompleto, falta campo: Nombre\n");
		return -1;
	}

	if (config_has_property(cfgPersonaje,"simbolo")){ //Si tiene cargado el simbolo lo mete en la variable simbolo
		personaje.simbolo=identificadorPersonaje(cfgPersonaje);
		miniPersonaje->simbolo=personaje.simbolo;
		printf("Identificador de personaje: %c.\n", personaje.simbolo);
	} else {
		printf("Archivo de configuracion incompleto, falta campo: Simbolo\n");
		return -1;
	}

	if (config_has_property(cfgPersonaje,"orquestador")){//Si tiene cargado el orquestador lo mete en la variable orquestador
		char *orquestadorAux,*aux1,*aux2;
		orquestadorAux=orquestador(cfgPersonaje);
		aux1=(strchr(orquestadorAux,':'))+1;
		int monto=strlen(orquestadorAux)-strlen(aux1)-1;
		aux2=(char*)string_substring_until((char*)orquestadorAux,monto);
		personaje.orquestadorPort=atoi(aux1);
		strcpy(personaje.orquestadorIP,aux2);
		printf("El orquestador esta en la IP: %s y el puerto: %i.\n", personaje.orquestadorIP,personaje.orquestadorPort);
	} else{
		printf("Archivo de configuracion incompleto, falta campo: Orquestador\n");
		return -1;
	}

	if (config_has_property(cfgPersonaje,"planDeNiveles")){ //Si tiene el plan de niveles cargado lo mete en los nodos de niveles en la lista
		char ** planNivelesPersonaje=niveles(cfgPersonaje);
		printf("El primer nivel es %s\n",planNivelesPersonaje[0]);
		char ** recursosNivel;
		int i,j,tamanioArrayNivel,contador,tamanioArrayRecursos,tamanioBuffer;
		personaje.planDeNiveles=(t_list*)malloc(sizeof(t_list));
		tamanioBuffer=cantidadElementosArray(planNivelesPersonaje);
		if(tamanioBuffer==0) {
			printf("Archivo de configuracion incompleto, falta campo: Niveles\n");
			return -1;
		}
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
				     trecurso*temp2;
				     temp=(trecurso*)malloc(sizeof(trecurso));
				     temp2=(trecurso*)malloc(sizeof(trecurso));
				     temp->tipo=(char)*(recursosNivel[j]);
				     if(j!=0)temp2->tipo=(char)*(recursosNivel[j-1]);
				     if(temp->tipo==temp2->tipo){
				    	 printf("Archivo de configuracion erroneo: Recursos\n");
				    	 return -1;
				     }
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
				printf("La Ip es: %s\n",personaje.orquestadorIP);
				printf("El puerto es: %d\n",personaje.orquestadorPort);
				sockfd=connectGRID(personaje.orquestadorPort,personaje.orquestadorIP);
				sendHandshake(1,ptrAux->nivel,miniPersonaje->simbolo,(short)sockfd);
				recvAnswer(&conexionSaliente,sockfd);
				printf("Coneccion establecida\n");
				if(conexionSaliente.msg==-1){
					printf("Conexion imposible de realizar\n");
					return -1;
				}
				miniPersonaje->orquestadorSocket=sockfd;
				miniHilo->nombre=sockfd;
				printf("Buffer de hilo cargado\n");
				list_add(personaje.miniPersonajes,(void*)miniHilo);
				printf("Funciona?\n");
				hilo=hiloGRID(jugar,(void*)miniPersonaje);
				strcpy(miniHilo->nivel,infoNivel->nivel);

		}
	}else {
		printf("Archivo de configuracion incompleto, falta campo: Plan de Niveles\n");
		return -1;
	}
	config_destroy(cfgPersonaje);
	return EXIT_SUCCESS;
}

/*
 * Retorna las vidas del personaje
 *
 */

int vidasPersonaje( t_config * cfgPersonaje)
{
	return config_get_int_value(cfgPersonaje,"vidas");
}

/*
 * Retorna el identificador del personaje
 *
 */

char identificadorPersonaje ( t_config * cfgPersonaje)
{
	return *config_get_string_value(cfgPersonaje,"simbolo");

}

/*
 * Retorna el nombre del personaje
 *
 */
char * nombrePersonaje ( t_config * cfgPersonaje)
{
	return config_get_string_value(cfgPersonaje,"nombre");
}

/*
 * Retorna el orquestador en formato IP-Puerto
 *
 */

char * orquestador( t_config * cfgPersonaje)
{
	return config_get_string_value(cfgPersonaje,"orquestador");
}

/*
 * Retorna los niveles en forma de array de char*
 *
 */

char ** niveles( t_config * cfgPersonaje)
{
	return config_get_array_value(cfgPersonaje,"planDeNiveles");
}

/*
 * Retorna los recursos en forma de array de char*
 *
 */

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

/*
 * Devuelve la cantidad de elementos en un array de char*
 *
 */

int cantidadElementosArray(char **array){
	int tamanioArray,tamanioElemento;
	tamanioElemento=sizeof(array[0]);
	tamanioArray=strlen((char*)array);
	return tamanioArray/tamanioElemento;
}

/*
 * Baja una vida en el padre
 * Retorna a la posicion 0,0
 * Reinicia sus recursos
 *
 */

int estoyMuerto(tminipersonaje *info){
	int i,sockfd;
	trecurso *aux;
	answer conexionSaliente;
	aux=(trecurso*)malloc(sizeof(trecurso));

	personaje.vidas-=1;
	printf("Vidas restantes: %d\n",personaje.vidas);
	info->posX=0;
	info->posY=0;
	printf("Reseteada la posicion del personaje\n");
	for(i=0;(info->planDeRecursos->elements_count)>i;i++){
		printf("Levantando el recurso %d\n",i);
		aux=(trecurso*)list_get(info->planDeRecursos,i);
		printf("Restaurando recurso\n");
		aux->checked=false;
		aux->posX=-1; //tenemos que resetear la posicion para evitar errores locos D:
		aux->posY=-1;
	}
	printf("Cerrando socket del hilo\n");
	close(info->orquestadorSocket);
	printf("Socket cerrado\n");
	if(personaje.vidas>0){
		printf("Reconectando personaje \n");
		sockfd=connectGRID(personaje.orquestadorPort,personaje.orquestadorIP);
		printf("Conectado. Avisando a los chamigos que volvi\n");
		sendHandshake(1,info->nivel,info->simbolo,(short)sockfd);
		printf("Esperando seniales de vida\n");
		recvAnswer(&conexionSaliente,sockfd);
		if(conexionSaliente.msg==-1){
			printf("Conexion imposible de realizar\n");
			return -1;
		}
		printf("Volvimos al juego, yeah\n");
		info->orquestadorSocket=sockfd;
	}else{
		printf("No hay mas vidas, limpiando lista de mini personajes\n");
		//list_clean(personaje.miniPersonajes);
		//repetir=1;
	}
	return 0;
}

/*
 * Pide la posicion del recurso siguiente
 *
 */

int actualizarRP(t_list*planDeRecursos,int posicion){
		trecurso *recursoSiguiente;
		int posX,posY;
		//recursoSiguiente=(trecurso*)malloc(sizeof(trecurso));  NO HACER MALLOC SI VAS A CAMBIARLO POR OTRA DIRECCION!!
		recursoSiguiente=(trecurso*)list_find(planDeRecursos,(void*)_recursoNoAgarrado);
		posX=posicion/100;
		printf("Posicion en X: %d\n",posX);
		posY=posicion-(posX*100);
		printf("Posicion en Y: %d\n",posY);
		recursoSiguiente->posX=posX;
		recursoSiguiente->posY=posY;
	return 0;
}

/*
 * Si no tiene la posicion del siguiente recurso la pide
 * Si tiene la posicion del siguiente recurso pide movimiento
 * Si esta en el recurso pide una instancia
 *
 */

int gestionTurno(t_list * planDeRecursos,int sockfd,int *posX,int *posY,char simbolo,int *moverEnX,int *esInstancia){
	trecurso *recursoSiguiente;
	int tempX=0,tempY=0;
	recursoSiguiente=(trecurso*)malloc(sizeof(trecurso));
	printf("Puntero: %p\n",(void*)recursoSiguiente);
	if((list_any_satisfy(planDeRecursos,(void*)_recursoNoAgarrado))==true){
		recursoSiguiente=(trecurso*)list_find(planDeRecursos,(void*)_recursoNoAgarrado);
		printf("Se crea el buffer de recurso\n");
		printf("Puntero: %p\n",(void*)recursoSiguiente);
		printf("Tipo: %c\n",recursoSiguiente->tipo);
		printf("Checked: %d\n",recursoSiguiente->checked);
		if(recursoSiguiente->posX==-1 || recursoSiguiente->posY==-1){ //Pedir pos Recurso
			printf("Pidiendo posicion recurso\n");
			sendAnswer(2,0,recursoSiguiente->tipo,simbolo, sockfd);
			return 0;
		}
		if(recursoSiguiente->posX==*posX && recursoSiguiente->posY==*posY){
			int auxiliar=1;
			printf("Pidiendo instancia\n");
			printf("Puntero: %p\n",(void*)recursoSiguiente);
			printf("Pidiendo instancia del recurso %c\n",recursoSiguiente->tipo);
			sendAnswer(2,1,recursoSiguiente->tipo,simbolo,sockfd); //Pedir instancia
			printf("Instancia pedida\n");
			*esInstancia=auxiliar;
			return 0;
		}
		if(recursoSiguiente->posX!=*posX && recursoSiguiente->posY!=*posY){ //Pedir movimiento
			printf("Pidiendo movimiento\n");
			if(*moverEnX==1){
				if((*posX-recursoSiguiente->posX)<0){
					tempX=1;
				}else tempX=-1;
				*moverEnX=0;
			}else{
				if((*posY-recursoSiguiente->posY)<0){
					tempY=1;
				}else tempY=-1;
				*moverEnX=1;
			}
		}
		if(recursoSiguiente->posX==*posX && recursoSiguiente->posY!=*posY){
			if((*posY-recursoSiguiente->posY)<0){
				tempY=1;
			}else tempY=-1;
		}
		if(recursoSiguiente->posY==*posY && recursoSiguiente->posX!=*posX){
			if((*posX-recursoSiguiente->posX)<0){
				tempX=1;
			}else tempX=-1;
		}
		tempX+=*posX;
		tempY+=*posY;
		printf("Nueva posicion X: %d\nNueva posicion Y: %d\n",tempX,tempY);
		printf("Posicion X: %d\nPosicion Y: %d\n",*posX,*posY);
		*posX=tempX;
		*posY=tempY;
		sendAnswer(3,(tempX*100+tempY),0,simbolo,sockfd);
		return tempX*100+tempY;
	}
return 0;
}

/*
 * Marca como levantado el recurso concedido
 *
 */

int instanciaConc(t_list * planDeRecursos,int* esInstancia){
	trecurso *aux;
	aux=(trecurso*)malloc(sizeof(trecurso));
	aux=(trecurso*)list_find(planDeRecursos,(void*)_recursoNoAgarrado);
	printf("El recurso agarrado fue: %c\n",aux->tipo);
	aux->checked=true;
	printf("Checked: %d\n",aux->checked);
	*esInstancia=0;
	return 0;
}

/*
 * Modifica su posicion por la concedida
 *
 */

int movimientoConc(int posicion,int*posX,int*posY){
	*posX=posicion/100;
	*posY=posicion-(*posX*100);
	return 0;
}

/*
 * Cierra los sockets de los hilos de miniPersonajes
 *
 */

void cierraHilos(){
	int j;
	thilo* aux;
	for (j = 0; j < personaje.miniPersonajes->elements_count; j++) {
		aux=(thilo*)list_get(personaje.miniPersonajes,j);
		close(aux->nombre);
	}
}
