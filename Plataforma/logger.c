#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "logger.h"
#include <commons/log.h>
#include <commons/string.h>

t_log* varLogTrace;
t_log* varLogDebug;
t_log* varLogInfo;
t_log* varLogWarning;
t_log* varLogError;

void cerrarLogs(void){
	  finLogTrace();
	  finLogDebug();
	  finLogWarning();
	  finLogInfo();
	  finLogError();
}


/*crea un archivo de log tipo TRACE*/

	void newArchLogTrace(char* programa, bool mostrarPorPantalla){

	varLogTrace = log_create(DIRECTORIO_LOG_TRACE, programa, mostrarPorPantalla, LOG_LEVEL_TRACE);
	}
	/*crea un archivo de log tipo DEBUG*/

	void newArchLogDebug(char* programa, bool mostrarPorPantalla){
	varLogDebug = log_create(DIRECTORIO_LOG_DEBUG, programa, mostrarPorPantalla, LOG_LEVEL_DEBUG);
	}
	/*crea un archivo de log tipo WARNING*/

	void newArchLogWarning(char* programa, bool mostrarPorPantalla){
	varLogWarning = log_create(DIRECTORIO_LOG_WARNING, programa, mostrarPorPantalla, LOG_LEVEL_WARNING);
	}
	/*crea un archivo de log tipo INFO*/

	void newArchLogInfo(char* programa, bool mostrarPorPantalla){
	varLogInfo = log_create(DIRECTORIO_LOG_INFO, programa, mostrarPorPantalla, LOG_LEVEL_INFO);
	}
	/*crea un archivo de log tipo ERROR*/

	void newArchLogError(char* programa, bool mostrarPorPantalla){
	varLogError = log_create(DIRECTORIO_LOG_ERROR, programa, mostrarPorPantalla, LOG_LEVEL_ERROR);

	}

//todas las formas de loguear salen con un formatito piola que codeo la catedra

//loguear de forma tracer,Debug, etc,etc

	void loguearTrace(char* mensaje){
		log_trace(varLogTrace,mensaje,"TRACE");

	}

	void loguearDebug(char* mensaje){
			log_debug(varLogDebug,mensaje,"DEBUG");

		}

	void loguearInfo(char* mensaje){
			log_info(varLogInfo,mensaje,"INFO");

		}

	void loguearWarning(char* mensaje){
			log_warning(varLogWarning,mensaje,"WARNING");

		}

	void loguearError(char* mensaje){
			log_error(varLogError,mensaje,"ERROR");

		}

	//NO OLVIDE CERRAR, funciones para cerrar los archivos log

	void finLogTrace(){
		log_destroy(varLogTrace);
	}
	void finLogDebug(){
		log_destroy(varLogDebug);
	}
	void finLogWarning(){
		log_destroy(varLogWarning);
	}
	void finLogError(){
		log_destroy(varLogError);
	}
	void finLogInfo(){
		log_destroy(varLogInfo);
	}



