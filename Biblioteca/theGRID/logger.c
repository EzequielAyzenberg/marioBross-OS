#include <stdio.h>
#include <stdlib.h>
#include "logger.h"
#include "commons/log.h"
#include "commons/string.h"




int main(void){
/*
 *   funciones que se utilizan para loguear todoooooooo el tp hay distintos tipos de log y son
 * los que la catedras piden de forma print y de forma archivo.
 *   En el archivo logger.h pueden cambiar la ruta donde se grabaran los log en forma txt
 */

// log que se utiliza para warning, primer warnin del programa mario
newArchLogWarning("super mario ");
//se loguea el erro que el tiene el ṕrograma mario
loguearWarning("luigi es maricón");
//se setea que programa quiero que tire el warning
newArchLogWarning("super luigi flacon");
// el programa luigi loguea dos warnings
loguearWarning("U.u");
loguearWarning("XD");

//log para trace que nose que es, se crea el encabezado del trace
newArchLogTrace("no sé que es Trace");
//info del trace
loguearTrace("honguito dice: mario y luigi par de toros");

//los de debugin, se crea el debug, se le pasa el programa
newArchLogDebug("nivel");
//info del debug
loguearDebug("tengo bug en el nivel");

//log de info, se utiliza por ejemplo cuando este habilitado recovery
//encabezado de la informacion
newArchLogInfo("Se deshabilito recovery");
//informacion despues de cada encabezado.
loguearInfo("se produjo un interbloqueo en entre luigui y mario. se procede a matar a luigi");

//Log de error, se le pasa el programa que tuvo el error
newArchLogError("TODO EL TP");
//se logue el error del programa
loguearError("Exploto todo al carajo");


//funciones que cierran el log, son necesaria porque los log allocan memoria
  finLogTrace();
  finLogDebug();
  finLogWarning();
  finLogInfo();
  finLogError();

return 0;
}



t_log* varLogTrace;
t_log* varLogDebug;
t_log* varLogInfo;
t_log* varLogWarning;
t_log* varLogError;

/*crea un archivo de log tipo TRACE*/

	void newArchLogTrace(char* programa){

	varLogTrace = log_create(DIRECTORIO_LOG_TRACE, programa, false, LOG_LEVEL_TRACE);
	}
	/*crea un archivo de log tipo DEBUG*/

	void newArchLogDebug(char* programa){
	varLogDebug = log_create(DIRECTORIO_LOG_DEBUG, programa, false, LOG_LEVEL_DEBUG);
	}
	/*crea un archivo de log tipo WARNING*/

	void newArchLogWarning(char* programa){
	varLogWarning = log_create(DIRECTORIO_LOG_WARNING, programa, false, LOG_LEVEL_WARNING);
	}
	/*crea un archivo de log tipo INFO*/

	void newArchLogInfo(char* programa){
	varLogInfo = log_create(DIRECTORIO_LOG_INFO, programa, false, LOG_LEVEL_INFO);
	}
	/*crea un archivo de log tipo ERROR*/

	void newArchLogError(char* programa){
	varLogError = log_create(DIRECTORIO_LOG_ERROR, programa, false, LOG_LEVEL_ERROR);

	}

//todas las formas de loguear salen con un formatito piola que codeo la catedra

//loguear de forma tracer,Debug, etc,etc

	void loguearTrace (char* mensaje){
		log_trace(varLogTrace,mensaje,"TRACE");

	}

	void loguearDebug (char* mensaje){
			log_debug(varLogDebug,mensaje,"DEBUG");

		}

	void loguearInfo (char* mensaje){
			log_info(varLogInfo,mensaje,"INFO");

		}

	void loguearWarning (char* mensaje){
			log_warning(varLogWarning,mensaje,"WARNING");

		}

	void loguearError (char* mensaje){
			log_error(varLogError,mensaje,"ERROR");

		}

	//NO OLVIDE CERRAR, funciones para cerrar los archivos log

	void finLogTrace (){
		log_destroy(varLogTrace);
	}
	void finLogDebug (){
		log_destroy(varLogDebug);
	}
	void finLogWarning (){
		log_destroy(varLogWarning);
	}
	void finLogError (){
		log_destroy(varLogError);
	}
	void finLogInfo (){
		log_destroy(varLogInfo);
	}

	void cerrarLogs(){
		  finLogTrace();
		  finLogDebug();
		  finLogWarning();
		  finLogInfo();
		  finLogError();
	}

