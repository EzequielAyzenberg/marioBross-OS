#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <commons/config.h>
#include <commons/string.h>
#include <commons/log.h>
#include <commons/collections/list.h>
#include <theGRID/general.h>
#include <theGRID/sockets.h>
#include <theGRID/cadenas.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <ncurses.h>

#define LOCAL_LOG "/home/utnso/GRIDLogs/LOGPersonaje/"
#define muestreo false

//Variables
typedef struct{
	t_log* debug;
	t_log* trace;
}logs;

typedef struct hilos{
	int nombre;
	char *nivel;
	pthread_t hilo;
}thilo;

typedef struct info{
	char *nivel;
}tinfo;

typedef struct personaje {
	char nombre[32];
	char simbolo;
	t_list *planDeNiveles;
	int vidas;
	int orquestadorPort;
	char *orquestadorIP;
	t_list *miniPersonajes;
	t_list *miniHilos;
}tpersonaje;

typedef struct miniPersonaje{
	char *nivel;
	t_list *planDeRecursos;
	int orquestadorSocket;
	int posX;
	int posY;
	logs *loggeo;
}tminipersonaje;

typedef struct recurso{
	char tipo;
	int posX;
	int posY;
	bool checked;
}trecurso;



int sockfd,socketEscucha,nuevo,ganado=0,ch,
	finalizados=0,repetir=1,repeticiones=0,
	limboOK=0,hilosMuertos=0,modificar=1,interrupcion=0,
	superMuerte=0,cerrado=0;
tpersonaje personaje;
char *recurso, *nivelAux;
t_list *lista, *listaRecursos;
pid_t pid;
pthread_t hilo;
t_config * cfgPersonaje;

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
int estoyMuerto(tminipersonaje*,int*);
int actualizarRP(t_list*,int,logs*);
int gestionTurno(t_list*,int,int*,int*,int*,int*,logs*);
int instanciaConc(t_list*,int*,logs*);
int movimientoConc(int,int*,int*,logs*);
void cierraHilos();
void aumentaVida(int);
void restaVida(int);
void cerrarBien();
void suicidarme(tminipersonaje);
logs crearLogs(tminipersonaje*);
void _loggearVictoria(tminipersonaje*);
void _loggearDerrota(tminipersonaje*);
void _loggearSenialAumenta(tminipersonaje*);
void _loggearSenialDecrementa(tminipersonaje*);

pthread_mutex_t mutexMuerte =PTHREAD_MUTEX_INITIALIZER;

int main(int argc, char *argv[]) {
	personaje.miniPersonajes=(t_list*)malloc(sizeof(t_list));
	personaje.miniHilos=(t_list*)malloc(sizeof(t_list));

	signal(SIGUSR1,aumentaVida);
	signal(SIGTERM,restaVida);
	signal(SIGINT,cerrarBien);

	while(repetir==1){
		superMuerte=0;
		puts("Listos para arrancar?");
		sleep(1);
		if( cargaPersonaje(argv) == -1){
			printf("Error al cargar configuracion del personaje.\n");
			list_destroy(personaje.miniPersonajes);
			list_destroy(personaje.miniHilos);
			return 0;
		}
		config_destroy(cfgPersonaje);

		//initscr();
		//keypad(stdscr, TRUE);   // for KEY_UP, KEY_DOWN

		while(personaje.miniHilos!=NULL && personaje.vidas>0){
			if(personaje.planDeNiveles->elements_count==ganado){
				int sockfdAux;
				printf("Ganamos! venga koopa, te hago frente\n");
				printf("Conectando con plataforma\n");
				sockfdAux=connectGRID(personaje.orquestadorPort,personaje.orquestadorIP);
				printf("Avisando a plataforma que soy un groso\n");
				sendHandshake(2,"",personaje.simbolo,(short)sockfdAux);
				printf("Ahora si, koopa, dame a la princesa\n");
				list_iterate(personaje.miniPersonajes,(void*)_loggearVictoria);
				//list_destroy(personaje.miniPersonajes);
				//QUIZAS DEBERIA ESPERAR A VER COMO SALIO KOOPA.
				exit(0);
			}
			if(limboOK){
				exit(1);
			}

			//if (ch == KEY_UP) aumentaVida(KEY_UP);
			//else if (ch == KEY_DOWN) restaVida(KEY_DOWN);
		}
		/*
		while(personaje.planDeNiveles->elements_count > hilosMuertos && limboOK==0){
			if(personaje.vidas<=0){
				superMuerte=1;
				cierraHilos();
				cerrado=1;
				break;
			}
		}
		 */
		if(limboOK==0){
			cierraHilos();
			char intentarlo[4];
			list_clean(personaje.miniPersonajes);
			list_clean(personaje.miniHilos);
			//sleep(1);
			bool esWeon=true;
			while(esWeon){
				printf("Se han agotado todas las vidas. Ha reintentado %d veces, desea reintentarlo?: S/N: ",repeticiones);
				leecad(intentarlo,1);
				switch(intentarlo[0]){
				case 'S': repeticiones++;
						  hilosMuertos = 0;
						  printf("\nEste es tu reintento numero: %d\n",repeticiones);
						  esWeon=false;
						  break;
				case 'N': repetir=0;
					  	  printf("\nPerdimos :(\n");
					  	  list_iterate(personaje.miniPersonajes,(void*)_loggearDerrota);
					  	  esWeon=false;
					  	  break;
				case 's': repeticiones++;
						  hilosMuertos = 0;
						  printf("\nEste es tu reintento numero: %d\n",repeticiones);
						  esWeon=false;
						  break;
				case 'n': repetir=0;
					  	  printf("\nPerdimos :(\n");
					  	  list_iterate(personaje.miniPersonajes,(void*)_loggearDerrota);
					  	  esWeon=false;
					  	  break;
				default:  puts("\nOpcion incorrecta.\n");
						  break;
				}
			}
		}else puts("El personaje se perdió en el limbo");
	}
	
	//list_destroy(personaje.miniPersonajes);
	//log_destroy(loggeo.trace);
	//endwin();
	return 0;
};
void _loggearVictoria(tminipersonaje* info){
	char mensaje [128],auxiliarMensaje[6];
	itoa(repeticiones,auxiliarMensaje,10);
	strcpy(mensaje,"El personaje gano con ");
	strcat(mensaje,auxiliarMensaje);
	strcat(mensaje," reintentos.");
	log_trace(info->loggeo->trace,mensaje,"TRACE");
}

void _loggearDerrota(tminipersonaje*info){
	log_trace(info->loggeo->trace,"--El personaje pierde--","TRACE");
}

/* Avisa que termino el nivel al orquestador
 * Cierra el socket
 */
void finDeNivel(int sockfd){
	//Antes tenia mas onda.
	close(sockfd);
}

/* Suma una vida al personaje
 */
void aumentaVida(int senial){
	int i;
	tminipersonaje *aux;
	printf("Se recibio la señal: %d\n",senial);
	interrupcion=1;
	personaje.vidas++;
	for(i=0;i<list_size(personaje.miniPersonajes);i++){
		aux=list_get(personaje.miniPersonajes,i);
		_loggearSenialAumenta(aux);
	}
	//list_iterate(personaje.miniPersonajes,(void*)_loggearSenialAumenta);
}

void _loggearSenialAumenta(tminipersonaje* info){
	log_trace(info->loggeo->trace,"--El personaje recibio una vida por senial--","TRACE");
}
void cerrarBien(){
	puts("Personaje cerrado correctamente");
	exit(1);
}

/* Resta una vida al personaje
 */
void restaVida(int senial){
	printf("Se recibio la señal: %d\n",senial);
	interrupcion=1;
	personaje.vidas--;
	list_iterate(personaje.miniPersonajes,(void*)_loggearSenialDecrementa);
}

void _loggearSenialDecrementa(tminipersonaje* info){
	log_trace(info->loggeo->trace,"--El personaje muere por: Senial--","TRACE");
}

/* Retorna true si el recurso fue agarrado
 */
bool _recursoAgarrado(trecurso *recurso){
	return recurso->checked;
}

/* Retorna true si el recurso no fue agarrado
 */
bool _recursoNoAgarrado(trecurso *recurso){
	return !(recurso->checked);
}


/* Gestiona las ordenes del planificador y delega a otras funciones
 * cada opcion por orden
 */
void *jugar (void *minipersonaje){
	//logs loggeo;
	answer ordenPlanificador;
	char mensaje[256],valor[8];
	int *esInstancia,*moverEnX,posicionNueva,desbloquear=0;
	moverEnX=(int*)malloc(sizeof(int));
	esInstancia=(int*)malloc(sizeof(int));
	*esInstancia=0;
	posicionNueva=0;
	*moverEnX=1;

	//Recastea el parametro al tipo original
	tminipersonaje *infoBis=(tminipersonaje*)minipersonaje;
	tminipersonaje info= *infoBis;
	info.nivel = (char*)malloc(sizeof(char[16]));
	strcpy(info.nivel,infoBis->nivel);

	printf("\n\n  Hola, soy el minepersonaje\n  Nivel: %s - Simbolo: %c \n\n",info.nivel,personaje.simbolo);
	modificar = 1;

	//printf("Cantidad de recursos cargados: %d\n",list_size(info.planDeRecursos));
	//printf("Nivel: %s\n",info.nivel);
	//printf("PosX e Y: %d--%d\n",info.posX,info.posY);
	printf("Socket: %d\n",info.orquestadorSocket);
	//printf("Simbolo: %c\n",personaje.simbolo);
	info.posX=0;
	info.posY=0;
	//int j,tamanioArrayRecursos=list_size(infoBis->planDeRecursos);
	/*char *temp2;
		for (j=0;j<tamanioArrayRecursos;j++){
	  	temp2=(char*)list_get(info.planDeRecursos,j);
	   	printf("Recurso en hilo: %c\n",*temp2);
	}*/
	extern tpersonaje personaje;
	printf("Creamos el log correspondiente al miniPersonaje\n\n");
	log_trace(info.loggeo->trace,"\t\t\t---------------------------\t\t\t","TRACE");
	strcpy(mensaje,"--Socket:(Env.)-Config:-(Nivel/Sym/[-Recursos-])--(");
	itoa(info.orquestadorSocket,valor,10);
	strcat(mensaje,valor);
	strcat(mensaje,")-(");
	strcat(mensaje,info.nivel);
	strcat(mensaje,"/");
	valor[0]=personaje.simbolo;
	valor[1]='\0';
	strcat(mensaje,valor);
	strcat(mensaje,"/[-");
	void _loggearRecursos(trecurso*recurso){
		valor[0]=recurso->tipo;
		valor[1]='\0';
		strcat(mensaje,valor);
		strcat(mensaje,"-");
	}
	if(list_is_empty(info.planDeRecursos))puts("ESTABA VACIA!!!");
	list_iterate(info.planDeRecursos,(void*)_loggearRecursos);
	strcat(mensaje,"])");
	log_trace(info.loggeo->trace,mensaje,"TRACE");
	while((list_any_satisfy(info.planDeRecursos,(void*)_recursoNoAgarrado))==true){
		if(desbloquear){
			desbloquear=0;
			pthread_mutex_unlock( &mutexMuerte); 
		}
		if(personaje.vidas<=0){
			strcpy(mensaje,"--No quedan mas vidas--");
			log_trace(info.loggeo->trace,mensaje,"TRACE");
			hilosMuertos ++;
			close(info.orquestadorSocket);
			pthread_exit(NULL);
			return NULL;
			//suicidarme(infoBis);
		}

		/*
		if(cerrado){
			hilosMuertos ++;
			close(info.orquestadorSocket);
			pthread_exit(NULL);
			return NULL;
		}

		if(superMuerte){
			puts("Estoy super muerto");
			hilosMuertos ++;
			close(info.orquestadorSocket);
			pthread_exit(NULL);
			return NULL;
		}
		*/
pthread_mutex_lock( &mutexMuerte);
		recvAnswer(&ordenPlanificador,info.orquestadorSocket);
		if(personaje.vidas<=0){
			hilosMuertos ++;
			close(info.orquestadorSocket);
			pthread_mutex_unlock(&mutexMuerte);
			pthread_exit(NULL);
			return NULL;
			//suicidarme(infoBis);
		}
if(ordenPlanificador.msg!=8) pthread_mutex_unlock( &mutexMuerte);
		printf("Orden del planificador: %d\n",ordenPlanificador.msg);
		printf("Vidas: %d\n",personaje.vidas);


		switch(ordenPlanificador.msg){
			case 8: //Estoy muerto
				if(ordenPlanificador.cont==0){
					strcpy(mensaje,"--Personaje muere por: Goomba--");
					printf("Personaje muere por: Goomba");
				} else {
					strcpy(mensaje,"--Personaje muere por: Interbloqueo--");
					printf("Personaje muere por: Interbloqueo");
				}
				log_trace(info.loggeo->trace,mensaje,"TRACE");
				estoyMuerto(&info,esInstancia);
				desbloquear=1;
				break;
			case 1: //Instancia o movimiento concedido
				//printf("Es instancia?: %d\n",*esInstancia);
				if(*esInstancia==1){
					instanciaConc(info.planDeRecursos,esInstancia,info.loggeo);
				}
				else movimientoConc(posicionNueva,&(info.posX),&(info.posY),info.loggeo);
				interrupcion=0;
				break;
			case 2: //Actualizar RP
				printf("Actualiza RP\n");
				actualizarRP(info.planDeRecursos,ordenPlanificador.cont,info.loggeo);
				interrupcion=0;
				break;
			case 7: //Gestion turno
				//printf("Gestionemos el turno\n");
				//printf("Mover en X: %d\n",*moverEnX);
				posicionNueva=gestionTurno(info.planDeRecursos,info.orquestadorSocket,&(info.posX),&(info.posY),moverEnX,esInstancia,info.loggeo);
				break;
			case 0: //Limbo
				printf("Se fue la plataforma, panico panico!\n");
				strcpy(mensaje,"--Plataforma caida--");
				log_trace(info.loggeo->trace,mensaje,"TRACE");
				limboOK=1;
				//suicidarme(infoBis);
				break;
		}
		printf("\n");
	}
	printf("Ganamos bitches\n");
	strcpy(mensaje,"--Personaje completa nivel--");
	log_trace(info.loggeo->trace,mensaje,"TRACE");
	ganado++;
	suicidarme(info);
	return NULL;
};

void suicidarme(tminipersonaje info){
	//list_destroy(infoBis->planDeRecursos);
	close(info.orquestadorSocket);
	//pthread_exit(NULL);
}

/* Carga el personaje en el
 * struct personaje para su uso
 * y crea los hilos para los
 */
int cargaPersonaje(char *argv[]){
	thilo *miniHilo;
	miniHilo=(thilo*)malloc(sizeof(thilo));
	miniHilo->nivel=(char*)malloc(sizeof(char[16]));
	answer conexionSaliente;
	tminipersonaje *miniPersonaje;
	tinfo *infoNivel;
	cfgPersonaje=config_create(argv[1]);//ACA CAMBIE LO QUE LE MANDA AL CONFIG
	infoNivel=(tinfo*)malloc(sizeof(tinfo));
	infoNivel->nivel=(char*)malloc(sizeof(char[16]));
	//miniPersonaje=(tminipersonaje*)malloc(sizeof(tminipersonaje));
	//miniPersonaje->nivel=(char*)malloc(sizeof(char[16]));
	personaje.orquestadorIP=(char*)malloc(sizeof(char[16]));

	//printf("Cargaremos el personaje\n");
	if (config_has_property(cfgPersonaje,"Vidas")){ //Si tiene cargadas las vidas las mete en la variable vidas
		personaje.vidas=vidasPersonaje(cfgPersonaje);
		printf("Vidas de personaje: %d.\n", personaje.vidas);
	} else {
		printf("Archivo de configuracion incompleto, falta campo: Vidas\n");
		return -1;
	}

	if (config_has_property(cfgPersonaje,"Nombre")){ //Si tiene cargado el nombre lo mete en la variable nombre
		char*temporal=nombrePersonaje(cfgPersonaje);
		strcpy(personaje.nombre,temporal);
		//free(temporal);
		printf("Nombre de personaje: %s /// ", personaje.nombre);
	} else {
		printf("Archivo de configuracion incompleto, falta campo: Nombre\n");
		return -1;
	}

	if (config_has_property(cfgPersonaje,"Simbolo")){ //Si tiene cargado el simbolo lo mete en la variable simbolo
		personaje.simbolo=identificadorPersonaje(cfgPersonaje);
		printf("Identificador: %c\n", personaje.simbolo);

	} else {
		printf("Archivo de configuracion incompleto, falta campo: Simbolo\n");
		return -1;
	}

	if (config_has_property(cfgPersonaje,"Orquestador")){//Si tiene cargado el orquestador lo mete en la variable orquestador
		char *orquestadorAux,*aux1,*aux2;
		orquestadorAux=orquestador(cfgPersonaje);
		aux1=(strchr(orquestadorAux,':'))+1;
		int monto=strlen(orquestadorAux)-strlen(aux1)-1;
		aux2=(char*)string_substring_until((char*)orquestadorAux,monto);
		personaje.orquestadorPort=atoi(aux1);
		strcpy(personaje.orquestadorIP,aux2);
		printf("OrquestadorIP: %s /// Puerto: %i\n", personaje.orquestadorIP,personaje.orquestadorPort);
	} else{
		printf("Archivo de configuracion incompleto, falta campo: Orquestador\n");
		return -1;
	}

	if (config_has_property(cfgPersonaje,"PlanDeNiveles")){ //Si tiene el plan de niveles cargado lo mete en los nodos de niveles en la lista
		char ** planNivelesPersonaje=niveles(cfgPersonaje);
		//printf("El primer nivel es %s\n",planNivelesPersonaje[0]);
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
			miniPersonaje=(tminipersonaje*)malloc(sizeof(tminipersonaje));
			miniPersonaje->nivel=(char*)malloc(sizeof(char[16]));
			while(modificar==0);
			printf("\nSe carga el nivel: %s /// Recursos: ", *(planNivelesPersonaje+i));
				strcpy(infoNivel->nivel,*(planNivelesPersonaje+i));
				strcpy(miniPersonaje->nivel,infoNivel->nivel);
				recursosNivel=recursos(cfgPersonaje,infoNivel->nivel);
				if((cantidadElementosArray(recursosNivel))==0){
					printf("Archivo de configuracion incompleto, falta campo: Recursos\n");
					return -1;
				}
				listaRecursos=list_create();
				tamanioArrayRecursos=cantidadElementosArray(recursosNivel);
				printf("\n");
				//puts("Lista de recursos creada, a cargarla");
				for (j=0;j<tamanioArrayRecursos;j++){		//CARGA LOS RECURSOS DE UN NIVEL
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
				     list_add(listaRecursos,(void*)temp);
				     if (j==0) miniPersonaje->planDeRecursos=listaRecursos;
				     //printf("Cargando recurso: %c\n",temp->tipo);
				    }
				contador=list_add(lista,(void*)infoNivel);
				if(i==0) personaje.planDeNiveles=lista;
				ptrAux=list_get(lista,contador);
				//printf("Nivel cargado: %s\n",ptrAux->nivel);
				/*char *temp2;
				for (j=0;j<tamanioArrayRecursos;j++){
				  	temp2=(char*)list_get(miniPersonaje->planDeRecursos,j);
				   	//printf("Recurso cargado: %c\n",*temp2);
				}*/
				printf("La Ip es: %s\n",personaje.orquestadorIP);
				printf("El puerto es: %d\n",personaje.orquestadorPort);
				sockfd=connectGRID(personaje.orquestadorPort,personaje.orquestadorIP);
				printf("El socket es %d\n",sockfd);
				strcpy(miniPersonaje->nivel,ptrAux->nivel);
				sendHandshake(1,miniPersonaje->nivel,personaje.simbolo,(short)sockfd);
				puts("Mande un handshake contandote quien soy");
				recvAnswer(&conexionSaliente,sockfd);
				printf("Conexion establecida\n");
				if(conexionSaliente.msg==-1){
					printf("Conexion imposible de realizar\n");
					return -1;
				}
				miniPersonaje->orquestadorSocket=sockfd;
				miniHilo->nombre=sockfd;
				//printf("Buffer de hilo cargado\n");
				printf("\n\n  Preparando para cargar el minepersonaje\n  Nivel: %s - Simbolo: %c \n\n",miniPersonaje->nivel,personaje.simbolo);
				logs *logAux;
				logAux=(logs*)malloc(sizeof(logs));
				*logAux=crearLogs(miniPersonaje);
				miniPersonaje->loggeo=logAux;
				hilo=hiloGRID(jugar,(void*)miniPersonaje);
				miniHilo->hilo=hilo;
				strcpy(miniHilo->nivel,infoNivel->nivel);
				modificar = 0;
				list_add(personaje.miniHilos,(void*)miniHilo);
				list_add(personaje.miniPersonajes,(void*)miniPersonaje);
		}

	}else {
		printf("Archivo de configuracion incompleto, falta campo: Plan de Niveles\n");
		return -1;
	}
	return EXIT_SUCCESS;
}

/* Retorna las vidas del personaje
 */
int vidasPersonaje( t_config * cfgPersonaje){
	return config_get_int_value(cfgPersonaje,"Vidas");
}

/* Retorna el identificador del personaje
 */
char identificadorPersonaje ( t_config * cfgPersonaje){
	return *config_get_string_value(cfgPersonaje,"Simbolo");
}

/* Retorna el nombre del personaje
 */
char * nombrePersonaje ( t_config * cfgPersonaje){
	return config_get_string_value(cfgPersonaje,"Nombre");
}

/* Retorna el orquestador en formato IP-Puerto
 */
char * orquestador( t_config * cfgPersonaje){
	return config_get_string_value(cfgPersonaje,"Orquestador");
}

/* Retorna los niveles en forma de array de char*
 */
char ** niveles( t_config * cfgPersonaje){
	return config_get_array_value(cfgPersonaje,"PlanDeNiveles");
}

/* Retorna los recursos en forma de array de char*
 */
char ** recursos( t_config * cfgPersonaje, char *nivel){
	char cadenaAuxiliar[20];
	char cadenaFinal[20];
	strcpy(cadenaAuxiliar,"Obj[");
	strcat(cadenaAuxiliar,nivel);
	strcat(cadenaAuxiliar,"]");
	strcpy(cadenaFinal,cadenaAuxiliar);
	return config_get_array_value(cfgPersonaje,cadenaFinal);
}

/* Devuelve la cantidad de elementos en un array de char*
 */
int cantidadElementosArray(char **array){
	/*int tamanioArray,tamanioElemento,i=0;
	tamanioElemento=sizeof(array);
	printf("tamanio elementosPOSTA: %d\n",tamanioElemento);
	tamanioElemento=sizeof(array[0]);
	printf("tamanio elemento: %d\n",tamanioElemento);
	tamanioArray=strlen(*array);
	printf("tamanio arrayPOSTA: %d\n",tamanioArray);
	tamanioArray=strlen((char*)array);
	printf("tamanio array: %d\n",tamanioArray);
	return tamanioArray/tamanioElemento;*/
	int i=0;
	while(array[i]!=NULL){
		printf("%s\n",array[i]);
		i++;
	}
	return i;
}

/* Baja una vida en el padre
 * Retorna a la posicion 0,0
 * Reinicia sus recursos
 */
int estoyMuerto(tminipersonaje *info,int *esInstancia){
	int i,sockfd,auxiliar=0;
	trecurso *aux;
	answer conexionSaliente;
	aux=(trecurso*)malloc(sizeof(trecurso));

	personaje.vidas-=1;
	printf("Vidas restantes: %d\n",personaje.vidas);
	info->posX=0;
	info->posY=0;
	*esInstancia=auxiliar;
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

		printf("\n\n  Reconectando como:\n  Nivel: %s - Simbolo: %c \n\n",info->nivel,personaje.simbolo);

		sendHandshake(1,info->nivel,personaje.simbolo,(short)sockfd);
		printf("Esperando seniales de vida\n");
		recvAnswer(&conexionSaliente,sockfd);
		if(conexionSaliente.msg==-1){
			printf("Conexion imposible de realizar\n");
			return -1;
		}
		printf("Volvimos al juego, yeah\n");
		info->orquestadorSocket=sockfd;
	}else{
		printf("No hay mas vidas\n");
		superMuerte=1;
		//list_clean(personaje.miniPersonajes);
		repetir=1;

		//suicidarme(info);
	}
	return 0;
}

/*Pide la posicion del recurso siguiente
 */
int actualizarRP(t_list*planDeRecursos,int posicion,logs *loggeo){
	char mensaje[256],aux[8];
	if(interrupcion==0){
		trecurso *recursoSiguiente;
		int posX,posY;
		recursoSiguiente=(trecurso*)list_find(planDeRecursos,(void*)_recursoNoAgarrado);
		posX=posicion/100;
		printf("Posicion en X: %d\n",posX);
		posY=posicion-(posX*100);
		printf("Posicion en Y: %d\n",posY);
		recursoSiguiente->posX=posX;
		recursoSiguiente->posY=posY;
		strcpy(mensaje,"--Posicion recurso siguiente (Recurso,X,Y)-- (");
		aux[0]=recursoSiguiente->tipo;
		aux[1]='\0';
		strcat(mensaje,aux);
		strcat(mensaje,",");
		itoa(posX,aux,10);
		strcat(mensaje,aux);
		strcat(mensaje,",");
		itoa(posY,aux,10);
		strcat(mensaje,aux);
		strcat(mensaje,") --");
		log_trace(loggeo->trace,mensaje,"TRACE");
	}
	return 0;
}

/* Si no tiene la posicion del siguiente recurso la pide
 * Si tiene la posicion del siguiente recurso pide movimiento
 * Si esta en el recurso pide una instancia
 */
int gestionTurno(t_list * planDeRecursos,int sockfd,int *posX,int *posY,int *moverEnX,int *esInstancia,logs *loggeo){
	trecurso *recursoSiguiente;
	char mensaje[256],aux[8];
	int tempX=0,tempY=0;
	recursoSiguiente=(trecurso*)malloc(sizeof(trecurso));
	//printf("Puntero: %p\n",(void*)recursoSiguiente);
	if((list_any_satisfy(planDeRecursos,(void*)_recursoNoAgarrado))==true){
		recursoSiguiente=(trecurso*)list_find(planDeRecursos,(void*)_recursoNoAgarrado);
		if(recursoSiguiente->posX==-1 || recursoSiguiente->posY==-1){ //Pedir pos Recurso
			printf("Pidiendo posicion recurso\n");
			strcpy(mensaje,"--Accion - Pidiendo posicion de recurso: ");
			aux[0]=recursoSiguiente->tipo;
			aux[1]='\0';
			strcat(mensaje,aux);
			strcat(mensaje," --");
			log_trace(loggeo->trace,mensaje,"TRACE");
			if(recursoSiguiente->tipo != '^')
			sendAnswer(2,0,recursoSiguiente->tipo,personaje.simbolo, sockfd);
			return 0;
		}
		if(recursoSiguiente->posX==*posX && recursoSiguiente->posY==*posY){
			int auxiliar=1;
			//printf("Pidiendo instancia\n");
			//printf("Puntero: %p\n",(void*)recursoSiguiente);
			printf("Pidiendo instancia del recurso %c\n",recursoSiguiente->tipo);
			strcpy(mensaje,"--Accion - Pidiendo instancia de recurso: ");
			aux[0]=recursoSiguiente->tipo;
			aux[1]='\0';
			strcat(mensaje,aux);
			strcat(mensaje," --");
			log_trace(loggeo->trace,mensaje,"TRACE");
			sendAnswer(2,1,recursoSiguiente->tipo,personaje.simbolo,sockfd); //Pedir instancia
			//printf("Instancia pedida\n");
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
		strcpy(mensaje,"--Accion - Pidiendo movimiento a posicion (X,Y)-- (");
		itoa(tempX,aux,10);
		strcat(mensaje,aux);
		strcat(mensaje,",");
		itoa(tempY,aux,10);
		strcat(mensaje,aux);
		strcat(mensaje,") --");
		log_trace(loggeo->trace,mensaje,"TRACE");
		//printf("Nueva posicion X: %d\nNueva posicion Y: %d\n",tempX,tempY);
		//printf("Posicion X: %d\nPosicion Y: %d\n",*posX,*posY);
		*posX=tempX;
		*posY=tempY;
		sendAnswer(3,(tempX*100+tempY),'.',personaje.simbolo,sockfd);
		return tempX*100+tempY;
	}
return 0;
}

/* Marca como levantado el recurso concedido
 */
int instanciaConc(t_list * planDeRecursos,int* esInstancia,logs *loggeo){
	if(interrupcion==0){
		trecurso *aux;
		char mensaje[256],valor[8];
		aux=(trecurso*)malloc(sizeof(trecurso));
		aux=(trecurso*)list_find(planDeRecursos,(void*)_recursoNoAgarrado);
		printf("El recurso agarrado fue: %c --CHECKED--\n",aux->tipo);
		aux->checked=true;
		strcpy(mensaje,"--Accion - Tomando instancia de recurso: ");
		valor[0]=aux->tipo;
		valor[1]='\0';
		strcat(mensaje,valor);
		strcat(mensaje," --");
		log_trace(loggeo->trace,mensaje,"TRACE");
		//printf("Checked: %d\n",aux->checked);
		*esInstancia=0;
	}
	return 0;
}

/* Modifica su posicion por la concedida
 */

int movimientoConc(int posicion,int*posX,int*posY,logs *loggeo){
	if(interrupcion==0){
		char mensaje[256],aux[8];
		strcpy(mensaje,"--Accion - Moverse a posicion(X,Y) -- (");
		*posX=posicion/100;
		*posY=posicion-(*posX*100);
		printf("Soy %c\n",personaje.simbolo);
		printf("Moviendonos hacia: (%d,%d)\n",*posX,*posY);
		itoa(*posX,aux,10);
		strcat(mensaje,aux);
		strcat(mensaje,",");
		itoa(*posY,aux,10);
		strcat(mensaje,aux);
		strcat(mensaje,") --");
		log_trace(loggeo->trace,mensaje,"TRACE");
	}
	return 0;
}

/* Cierra los sockets de los hilos de miniPersonajes
 * y los hilos
 */
void cierraHilos(){
//	int j;
//	thilo* aux;
	/*for (j = 0; j < personaje.miniPersonajes->elements_count; j++) {
		aux=(thilo*)list_get(personaje.miniPersonajes,j);
		close(aux->nombre);
	}*/
}

logs crearLogs(tminipersonaje*raiz){
	char file[64],program_name[32];
	logs paquete;
	extern tpersonaje personaje;
	strcpy(file,LOCAL_LOG);
	strcat(file,personaje.nombre);
	strcat(file,"-");
	strcat(file,raiz->nivel);
	strcat(file,"-Trace");
	strcat(file,".txt");
	strcpy(program_name,"PERSONAJE_");
	strcat(program_name,raiz->nivel);
	paquete.trace=log_create(file,program_name,muestreo,LOG_LEVEL_TRACE);
/*	strcpy(file,LOCAL_LOG);
	strcat(file,personaje.nombre);
	strcat(file,"-");
	strcat(file,raiz->nivel);
	strcat(file,"-Debug");
	strcat(file,".txt");
	paquete.debug=log_create(file,program_name,muestreo,LOG_LEVEL_DEBUG);*/
	return paquete;
}

