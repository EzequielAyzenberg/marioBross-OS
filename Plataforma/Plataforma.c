//SOY EL PADRE DE TODO LO QUE PASA AQUI!!!

#include"Orquestador.h"

#define MYIP "127.0.0.1"
#define MAX 64
#define PROGRAMA "PLATAFORMA"


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

	pthread_t id_orquest = cargadorOrquestador(argv[1]);

	if(id_orquest == -1){
		puts("Hubo un error en la carga");
		return -1;
	};
	pthread_join(id_orquest,NULL);

	loguearInfo("--PLTAFORMA--Proceso Orquestador finalizado");
	puts("--PLTAFORMA--Proceso Orquestador finalizado");
	loguearInfo("Cerrando logs...");
	cerrarLogs();
	return 0;
}

pthread_t cargarOrquestador(char *path){
		t_config * cfgPlataforma;
		infoOrquestador registroOrquestador;
		cfgPlataforma=config_create(path);

		if (config_has_property(cfgPlataforma,"puerto")){
			registroOrquestador.puerto=puertoPlataforma(cfgPlataforma);
			printf("Puerto: %d.\n", registroOrquestador.puerto);
		}else{
			printf("Archivo de configuracion incompleto, falta campo: puerto\n");
			return -1;
		}

		if (config_has_property(cfgPlataforma,"koopa")){
			registroOrquestador.koopa=pathKoopaPlataforma(cfgPlataforma);
			printf("Path de koopa: %s.\n", registroOrquestador.koopa);
		}else{
			printf("Archivo de configuracion incompleto, falta campo: koopa\n");
			return -1;
		}

		if (config_has_property(cfgPlataforma,"script")){
			registroOrquestador.script=pathScriptPlataforma(cfgPlataforma);
			printf("Path del script: %s.\n", registroOrquestador.script);
		}else{
			printf("Archivo de configuracion incompleto, falta campo: script\n");
			return -1;
		}

		t_list *listaNiveles = list_create();
		registroOrquestador.listaNiveles = listaNiveles;
		pthread_t id_orquest = id_orquest=hiloGRID(orquestador,(void*)&registroOrquestador);
		return id_orquest;
};

int puertoPlataforma( t_config * cfgPlataforma){
	return config_get_int_value(cfgPlataforma,"puerto");
}

char * pathKoopaPlataforma( t_config * cfgPlataforma){
	return config_get_string_value(cfgPlataforma,"koopa");
}

char * pathScriptPlataforma( t_config * cfgPlataforma){
	return config_get_string_value(cfgPlataforma,"script");
}
