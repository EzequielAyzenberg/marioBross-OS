#include"Mini.h"

void chequearVidas(tminipersonaje);
bool _recursoNoAgarrado(trecurso*);
int estoyMuerto(tminipersonaje*,int*);
int instanciaConc(t_list*,int*,logs*);
int movimientoConc(int,int*,int*,logs*,char*);
int actualizarRP(t_list*,int,logs*);
int gestionTurno(t_list*,int,int*,int*,int*,int*,logs*);
void suicidarme(tminipersonaje);

extern pthread_mutex_t mutexMuerte;


/* Gestiona las ordenes del planificador y delega a otras funciones
 * cada opcion por orden
 */
void *jugar (tminipersonaje*infoBis){
	answer ordenPlanificador;
	char mensaje[256],valor[8];
	int *esInstancia,*moverEnX,posicionNueva,desbloquear=0;
	moverEnX=(int*)malloc(sizeof(int));
	esInstancia=(int*)malloc(sizeof(int));
	*esInstancia=0;
	posicionNueva=0;
	*moverEnX=1;

	//Recastea el parametro al tipo original
	tminipersonaje info= *infoBis;
	info.nivel = infoBis->nivel;
	//strcpy(info.nivel,infoBis->nivel);

	printf("\n\n  Hola, soy el minepersonaje\n  Nivel: %s - Simbolo: %c \n\n",info.nivel,personaje.simbolo);
	printf("Socket: %d\n",info.orquestadorSocket);
	info.posX=0;
	info.posY=0;
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
	else list_iterate(info.planDeRecursos,(void*)_loggearRecursos);
	strcat(mensaje,"])");
	log_trace(info.loggeo->trace,mensaje,"TRACE");
	while((list_any_satisfy(info.planDeRecursos,(void*)_recursoNoAgarrado))){
		if(desbloquear){
			desbloquear=0;
		}
		chequearVidas(info);
		recvAnswer(&ordenPlanificador,info.orquestadorSocket);
		chequearVidas(info);

		printf("Orden del planificador: %d\n",ordenPlanificador.msg);
		printf("Vidas: %d\n",personaje.vidas);


		switch(ordenPlanificador.msg){
			case 8: //Estoy muerto
				if(ordenPlanificador.cont==0){
					strcpy(mensaje,"--Personaje muere por: Goomba--");
					printf("Personaje muere por: Goomba\n");
				} else {
					strcpy(mensaje,"--Personaje muere por: Interbloqueo--");
					printf("Personaje muere por: Interbloqueo\n");
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
				else movimientoConc(posicionNueva,&(info.posX),&(info.posY),info.loggeo,info.nivel);
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
				printf("La Plataforma nos echo, panico panico!\n");
				strcpy(mensaje,"--Plataforma caida--");
				log_trace(info.loggeo->trace,mensaje,"TRACE");
				limboOK=1;
				suicidarme(info);
				break;
		}
		printf("\n");
	}//FIN DEL WHILE!!



	printf("Ganamos bitches\n");
	strcpy(mensaje,"--Personaje completa nivel--");
	log_trace(info.loggeo->trace,mensaje,"TRACE");
	ganado++;
	suicidarme(info);
	return NULL;
}
//-----------------------------------------------------------------------------------------------------------------


void chequearVidas(tminipersonaje info){
	if(personaje.vidas<=0){
		printf("No hay mas vidas\n");
		//sendAnswer(0,0,' ',' ',info.orquestadorSocket);
		close(info.orquestadorSocket);
	//	list_remove(personaje.miniHilos)   ELIMINAR EL NODO???
		pthread_exit(NULL);
	}
}

/* Retorna true si el recurso no fue agarrado
 */

bool _recursoNoAgarrado(trecurso *recurso){
	return !(recurso->checked);
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
	chequearVidas(*info);
	pthread_mutex_lock( &mutexMuerte);
	personaje.vidas-=1;
	pthread_mutex_unlock( &mutexMuerte);
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
	chequearVidas(*info);
	printf("Cerrando socket del hilo\n");
	sendAnswer(0,0,' ',' ',info->orquestadorSocket);
	close(info->orquestadorSocket);
	printf("Socket cerrado\n");
	//if(personaje.vidas>0){
	chequearVidas(*info);
		printf("Reconectando personaje \n");
		//pthread_mutex_lock( &mutexMuerte);
		sockfd=connectGRID(personaje.orquestadorPort,personaje.orquestadorIP);
		printf("Conectado. Avisando a los chamigos que volvi\n");
		printf("\n\n  Reconectando como:\n  Nivel: %s - Simbolo: %c \n\n",info->nivel,personaje.simbolo);
		sendHandshake(1,info->nivel,personaje.simbolo,(short)sockfd);
		printf("Esperando seniales de vida\n");
		recvAnswer(&conexionSaliente,sockfd);
		//pthread_mutex_unlock( &mutexMuerte);
		if(conexionSaliente.msg==-1){
			printf("Conexion imposible de realizar\n");
			return -1;
		}
		printf("Volvimos al juego, yeah\n");
		info->orquestadorSocket=sockfd;
	/*}else{
		printf("No hay mas vidas\n");
		superMuerte=1;
		//list_clean(personaje.miniPersonajes);
		repetir=1;

		//suicidarme(info);
	}*/
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
int movimientoConc(int posicion,int*posX,int*posY,logs *loggeo,char* nivel){
	if(interrupcion==0){
		char mensaje[256],aux[8];
		strcpy(mensaje,"--Accion - Moverse a posicion(X,Y) -- (");
		*posX=posicion/100;
		*posY=posicion-(*posX*100);
		printf("Soy %c jugando en el %s\n",personaje.simbolo,nivel);
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

void suicidarme(tminipersonaje info){
	//sendAnswer(0,0,' ',' ',info.orquestadorSocket);
	close(info.orquestadorSocket);
	pthread_exit(NULL);
}

