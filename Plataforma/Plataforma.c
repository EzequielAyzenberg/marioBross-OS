//SOY EL PADRE DE TODO LO QUE PASA AQUI!!!

#include"Orquestador.h"


#define MYPORT 2345
#define MYIP "192.168.0.11"
#define MAX 64
#define PROGRAMA "Plataforma"


int main(int argc, char *argv[]){
					//Carga la IP de destino y el puerto puestos a la hora de ejecutar el programa.
	int puerto;
	switch (argc){
	case 2:
		puerto=atoi(argv[1]);
		printf("Se usara el siguiente puerto:\t%d\n", puerto);
		break;
	default:
		puerto=MYPORT;
		puts("Se usara el puerto estandar.");
		break;
	}

	newArchLogWarning(PROGRAMA,true);
	newArchLogTrace(PROGRAMA,true);
	newArchLogDebug(PROGRAMA,true);
	newArchLogInfo(PROGRAMA,true);
	newArchLogError(PROGRAMA,true);

	t_list *listaNiveles = list_create();
	pthread_t id_orquest;
	infoOrquestador registroOrquestador;
	registroOrquestador.puerto = puerto;
	registroOrquestador.listaNiveles = listaNiveles;

	id_orquest= hiloGRID(orquestador,(void*)&registroOrquestador);
	pthread_join(id_orquest,NULL);

	loguearInfo("Proceso Orquestador finalizado");
	loguearInfo("Cerrando logs...");
	cerrarLogs();
	return 0;
}
