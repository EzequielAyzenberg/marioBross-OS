//SOY EL PADRE DE TODO LO QUE PASA AQUI!!!

//#include"Orquestador.h"
#include "Plataforma.h"

#define MAX 64
#define PROGRAMA "PLATAFORMA"

int defaultRD;
bool mpantalla=false;
bool mtexto=false;
char * CFG_PATH;

int main(int argc, char *argv[]){
	if(argc==1){
		puts("Uso: pasar por argumento el path del .cfg");
		exit (0);
	};
	if(argc==3){
		char parametro[5];
		strcpy(parametro,argv[2]);
		if((parametro[0]=='-')&&(strlen(parametro)==2)){
			if(parametro[1]=='p'){
				puts("MODO PANTALLA--ON");
				mpantalla=true;
			}
			if(parametro[1]=='t'){
				puts("MODO DEBUG--ON");
				mtexto=true;
			}
		}
	}


	signal(SIGINT,cerrarTodo);
	finalizar=false;

	listaNiveles = list_create();

	newArchLogWarning(PROGRAMA);
	newArchLogTrace(PROGRAMA);
	newArchLogDebug(PROGRAMA);
	newArchLogInfo(PROGRAMA);
	newArchLogError(PROGRAMA);

	CFG_PATH = argv[1];
	printf("CFG_PATH: %s\n", CFG_PATH);
	defaultRD = cargarRemainingDistance(argv[1]);
	if(mpantalla)introduction();		//	LLAVE A LA FELICIDAD!!!!!
	pthread_t id_orquest = hiloGRID(orquestador,NULL);
	if(id_orquest == -1){
		puts("Hubo un error en la carga");
		return -1;
	};
	if(mpantalla) hiloGRID(pantalla,NULL);
	pthread_join(id_orquest,NULL);

	loguearInfo("--PLATAFORMA--Proceso Orquestador finalizado");
	if(!mpantalla)puts("--PLATAFORMA--Proceso Orquestador finalizado");
	loguearInfo("Cerrando logs...");
	cerrarLogs();
	return 0;
}

int cargarRemainingDistance(char * CFG_PATH){
	t_config * cfgPlataforma = config_create(CFG_PATH);
	int RD;

	if (config_has_property(cfgPlataforma,"remainingDistance")){
		RD = config_get_int_value(cfgPlataforma,"remainingDistance");
		if(!mpantalla)printf("Remaining Distance Default: %d.\n", RD);
	}else{
		if(!mpantalla)printf("Archivo de configuracion incompleto, falta campo: remainingDistance\n");
		exit(0);
	}
	config_destroy(cfgPlataforma);
	return RD;
}
