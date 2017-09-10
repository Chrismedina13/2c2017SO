/*
 * logFileSystem.h
 *
 *  Created on: 10/9/2017
 *      Author: utnso
 */

#ifndef HEADERS_LOGWORKER_H_
#define HEADERS_LOGWORKER_H_

#include "commons/log.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <commons/string.h>

t_log* logger; //Variable global loguer, lo que devuelve a crear el log

void crearLog(char* archivo, char* nombreDelPrograma, bool mostrarPorConsola,
		t_log_level nivelDeLog);
void logInfo(const char* mensaje, ...);
void logTrace(const char* mensaje, ...);
void logError(const char* mensaje, ...);
void logWarnig(const char* mensaje, ...);
void logDebug(const char* mensaje, ...);

#endif /* HEADERS_LOGWORKER_H_ */
