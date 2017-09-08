/*
 * logMaster.c
 *
 *  Created on: 7/9/2017
 *      Author: utnso
 */

#include "Headers/logMaster.h"


void crearLog(char* archivo,char* nombreDelPrograma,bool mostrarPorConsola,t_log_level nivelDeLog){

	logger = log_create(archivo,nombreDelPrograma,mostrarPorConsola,nivelDeLog);
}


void logInfo(const char* mensaje, ...){

	log_info(logger,mensaje);
}

void logDebug(const char* mensaje, ...){

	log_debug(logger,mensaje);
}

void logWarnig(const char* mensaje, ...){

	log_warning(logger,mensaje);
}

void logError(const char* mensaje, ...){

	log_error(logger,mensaje);
}

void logTrace(const char* mensaje, ...){

	log_trace(logger,mensaje);
}
