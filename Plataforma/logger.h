/*
 * logger.h
 *
 *  Created on: 02/05/2013
 *      Author: utnso
 */

#ifndef LOGGER_H_
#define LOGGER_H_

#include <stdbool.h>

#define DIRECTORIO_LOG_TRACE "/home/utnso/logsPlataforma/errorTrace.txt"
#define DIRECTORIO_LOG_DEBUG "/home/utnso/logsPlataforma/errorDebug.txt"
#define DIRECTORIO_LOG_WARNING "/home/utnso/logsPlataforma/errorWarning.txt"
#define DIRECTORIO_LOG_INFO "/home/utnso/logsPlataforma/errorInfo.txt"
#define DIRECTORIO_LOG_ERROR "/home/utnso/logsPlataforma/errorFatalityError.txt"

void newArchLogTrace(char* programa, bool mostrarPorPantalla);
void newArchLogDebug(char* programa, bool mostrarPorPantalla);
void newArchLogWarning(char* programa, bool mostrarPorPantalla);
void newArchLogInfo(char* programa, bool mostrarPorPantalla);
void newArchLogError(char* programa, bool mostrarPorPantalla);
void loguearTrace(char* mensaje);
void loguearDebug(char* mensaje);
void loguearInfo(char* mensaje);
void loguearWarning(char* mensaje);
void loguearError(char* mensaje);
void finLogTrace();
void finLogDebug();
void finLogWarning();
void finLogError();
void finLogInfo();
void cerrarLogs(void);

#endif /* LOGGER_H_ */
