//#include"Personaje.h"
#include"Mini.h"


int cargaPersonaje(char *argv[]);
void aumentaVida(int);
void restaVida(int);
void cerrarBien(int);
void _loggearVictoria(tminipersonaje*);
void _loggearDerrota(tminipersonaje*);
logs crearLogs(tminipersonaje*);
void finDeNivel(int);
void _loggearSenialAumenta(tminipersonaje*);
void _loggearSenialDecrementa(tminipersonaje*);
bool _recursoAgarrado(trecurso*);
int vidasPersonaje( t_config * cfgPersonaje);
char identificadorPersonaje ( t_config * cfgPersonaje);
char * nombrePersonaje ( t_config * cfgPersonaje);
char * orquestador( t_config * cfgPersonaje);
char ** niveles( t_config * cfgPersonaje);
char ** recursos( t_config * cfgPersonaje, char *nivel);
int cantidadElementosArray(char **);
void unirse(thilo*);

extern int repetir,ganado,limboOK,repeticiones,interrupcion,sockfd;

pthread_mutex_t mutexMuerte=PTHREAD_MUTEX_INITIALIZER;

//extern pthread_mutex_t mutexMuerte;

int main(int argc, char *argv[]) {
	repetir=1;
	personaje.miniPersonajes=(t_list*)malloc(sizeof(t_list));
	personaje.miniHilos=(t_list*)malloc(sizeof(t_list));

	signal(SIGUSR1,aumentaVida);
	signal(SIGTERM,restaVida);
	signal(SIGINT,cerrarBien);

	while(repetir==1){
		//superMuerte=0;
		puts("Listos para arrancar?");
		if( cargaPersonaje(argv) == -1){
			printf("Error al cargar el personaje.\n");
			list_destroy(personaje.miniPersonajes);
			list_destroy(personaje.miniHilos);
			exit(1);
		}
		config_destroy(cfgPersonaje);

		while(!list_is_empty(personaje.miniPersonajes) && personaje.vidas>0){
			if(personaje.planDeNiveles->elements_count==ganado){
				int sockfdAux;
				printf("Ganamos! venga koopa, te hago frente\n");
				printf("Conectando con plataforma\n");
				sockfdAux=connectGRID(personaje.orquestadorPort,personaje.orquestadorIP);
				printf("Avisando a plataforma que soy un groso\n");
				sendHandshake(2,"",personaje.simbolo,(short)sockfdAux);
				printf("Ahora si, koopa, dame a la princesa\n");
				list_iterate(personaje.miniPersonajes,(void*)_loggearVictoria);
				exit(0);
			}
			if(limboOK){
				break;
				exit(1);
			}

		}
		list_iterate(personaje.miniHilos,(void*)unirse);
		//list_iterate(personaje.miniHilos,(void*)matar);
		if(!limboOK){
			char intentarlo[4];
			list_clean(personaje.miniPersonajes);
			list_clean(personaje.miniHilos);
			bool esWeon=true;
			while(esWeon){
				printf("\nSe han agotado todas las vidas. Ha reintentado %d veces\nDesea reintentarlo?: S/N: ",repeticiones);
				leecad(intentarlo,1);
				switch(intentarlo[0]){
				case 'S': repeticiones++;
						  printf("\nEste es tu reintento numero: %d\n",repeticiones);
						  esWeon=false;
						  break;
				case 'N': repetir=0;
					  	  printf("\nPerdimos :(\n");
					  	  list_iterate(personaje.miniPersonajes,(void*)_loggearDerrota);
					  	  esWeon=false;
					  	  break;
				case 's': repeticiones++;
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
		}else {
			puts("El personaje se perdió en el limbo");
			exit(1);
		}
	}

	return 0;
};

/* Suma una vida al personaje
 */
void aumentaVida(int senial){
	int i;
	tminipersonaje *aux;
	printf("Se recibio la señal: %d\n",senial);
	interrupcion=1;
	pthread_mutex_lock( &mutexMuerte);
	personaje.vidas++;
	pthread_mutex_unlock( &mutexMuerte);
	for(i=0;i<list_size(personaje.miniPersonajes);i++){
		aux=list_get(personaje.miniPersonajes,i);
		_loggearSenialAumenta(aux);
	}
	//list_iterate(personaje.miniPersonajes,(void*)_loggearSenialAumenta);
}

/* Resta una vida al personaje
 */
void restaVida(int senial){
	printf("Se recibio la señal: %d\n",senial);
	interrupcion=1;
	pthread_mutex_lock(&mutexMuerte);
	personaje.vidas--;
	pthread_mutex_unlock(&mutexMuerte);
	list_iterate(personaje.miniPersonajes,(void*)_loggearSenialDecrementa);
}

void cerrarBien(int senial){
	puts("\nPersonaje cerrado correctamente");
	exit(1);
}

/* Carga el personaje en el
 * struct personaje para su uso
 * y crea los hilos para los
 */
int cargaPersonaje(char *argv[]){
	thilo *miniHilo;
	answer conexionSaliente;
	tminipersonaje *miniPersonaje;
	tinfo *infoNivel;
	cfgPersonaje=config_create(argv[1]);//ACA CAMBIE LO QUE LE MANDA AL CONFIG
	infoNivel=(tinfo*)malloc(sizeof(tinfo));
	infoNivel->nivel=(char*)malloc(sizeof(char[16]));
	miniPersonaje=(tminipersonaje*)malloc(sizeof(tminipersonaje));
	miniPersonaje->nivel=(char*)malloc(sizeof(char[16]));
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
			miniHilo=(thilo*)malloc(sizeof(thilo));
			miniHilo->nivel=(char*)malloc(sizeof(char[16]));
			miniPersonaje=(tminipersonaje*)malloc(sizeof(tminipersonaje));
			miniPersonaje->nivel=(char*)malloc(sizeof(char[16]));
			//while(modificar==0);
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
				if(conexionSaliente.msg==-1){
					printf("Falta algun Nivel. Abortado\n");
					return -1;
				}
				printf("Conexion establecida\n");
				miniPersonaje->orquestadorSocket=sockfd;
				miniHilo->nombre=sockfd;
				//printf("Buffer de hilo cargado\n");
				printf("\n\n  Preparando para cargar el minepersonaje\n  Nivel: %s - Simbolo: %c \n\n",miniPersonaje->nivel,personaje.simbolo);
				logs*logAux;
				logAux=(logs*)malloc(sizeof(logs));
				*logAux=crearLogs(miniPersonaje);
				miniPersonaje->loggeo=logAux;
				hilo=hiloGRID((void*)jugar,(void*)miniPersonaje);
				miniHilo->hilo=hilo;
				strcpy(miniHilo->nivel,infoNivel->nivel);
				list_add(personaje.miniHilos,(void*)miniHilo);
				//printf("\t\t\tEL HILO ES:%d\n",(int)hilo);
				//sleep(10);
				list_add(personaje.miniPersonajes,(void*)miniPersonaje);
		}

	}else {
		printf("Archivo de configuracion incompleto, falta campo: Plan de Niveles\n");
		return -1;
	}
	return EXIT_SUCCESS;
}

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

/* Avisa que termino el nivel al orquestador
 * Cierra el socket
 */
void finDeNivel(int sockfd){
	//Antes tenia mas onda.
	close(sockfd);
}

void _loggearSenialAumenta(tminipersonaje* info){
	log_trace(info->loggeo->trace,"--El personaje recibio una vida por senial--","TRACE");
}

void _loggearSenialDecrementa(tminipersonaje* info){
	log_trace(info->loggeo->trace,"--El personaje muere por: Senial--","TRACE");
}

/* Retorna true si el recurso fue agarrado
 */
bool _recursoAgarrado(trecurso *recurso){
	return recurso->checked;
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
	int i=0;
	while(array[i]!=NULL){
		printf("%s\n",array[i]);
		i++;
	}
	return i;
}

void unirse(thilo*hilo){
	pthread_join(hilo->hilo,NULL);
	pthread_cancel(hilo->hilo);

}
/*void unirse(thilo*hilo){
		pthread_join(hilo->hilo,NULL);
}*/


