//SOY EL PADRE DE TODO LO QUE PASA AQUI!!!

#include"Orquestador.h"

#define MAX 64
#define PROGRAMA "PLATAFORMA"

int defaultRD;

int main(int argc, char *argv[]){
					//Carga la IP de destino y el puerto puestos a la hora de ejecutar el programa.
	int puerto;
	if(argc==1){
		puts("Uso: pasar por argumento el path del .cfg");
		return 0;
	};

	newArchLogWarning(PROGRAMA);
	newArchLogTrace(PROGRAMA);
	newArchLogDebug(PROGRAMA);
	newArchLogInfo(PROGRAMA);
	newArchLogError(PROGRAMA);

	infoOrquestador registroOrquestador;
	t_list *listaNiveles = list_create();

	registroOrquestador.cfg = argv[1];
	registroOrquestador.listaNiveles = listaNiveles;
	printf("CFG_PATH: %s\n", registroOrquestador.cfg);

	defaultRD = cargarRemainingDistance(argv[1]);
	pthread_t id_orquest = hiloGRID(orquestador,(void*)&registroOrquestador);

	if(id_orquest == -1){
		puts("Hubo un error en la carga");
		return -1;
	};
	pthread_join(id_orquest,NULL);

	loguearInfo("--PLATAFORMA--Proceso Orquestador finalizado");
	puts("--PLATAFORMA--Proceso Orquestador finalizado");
	loguearInfo("Cerrando logs...");
	cerrarLogs();
	return 0;
}

int cargarRemainingDistance(char * CFG_PATH){
	t_config * cfgPlataforma = config_create(CFG_PATH);
	int RD;

	if (config_has_property(cfgPlataforma,"remainingDistance")){
		RD = config_get_int_value(cfgPlataforma,"remainingDistance");
		printf("Remaining Distance Default: %d.\n", RD);
	}else{
		printf("Archivo de configuracion incompleto, falta campo: remainingDistance\n");
		exit(0);
	}
	config_destroy(cfgPlataforma);
	return RD;
}
