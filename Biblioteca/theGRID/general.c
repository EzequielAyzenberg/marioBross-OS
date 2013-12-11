#include"general.h"

void flush_in(void){
	//No le pasas nada, solo lo ejecutas, y tecnicamente limpia el buffer de entrada.
int ch;
while( (ch = fgetc(stdin)) != EOF && ch != '\n' && ch!='\0' ){}
}

pthread_t hiloGRID(void*(*funcion)(void*),void*parametro){

	/*Le pasas el nombre de la funcion (debe recibir un puntero void y devolver
		un puntero void) junto con su argumento casteado como (void*).
		Te devuelve un la id del hilo que creaste.
	*/
	int i;
	pthread_t idHilo;
	if(pthread_create(&idHilo,NULL,funcion,parametro)==0)i++/*puts("Hilo creado correctamente.")*/;
	else puts("Hubo un problema en la creacion del hilo.");
	return idHilo;
}

