/*
 * logFileSystem.c
 *
 *  Created on: 10/9/2017
 *      Author: utnso
 */

#include "Headers/logFileSystem.h"


void crearLog(char* archivo,char* nombreDelPrograma,bool mostrarPorConsola,t_log_level nivelDeLog){

	logger = log_create(archivo,nombreDelPrograma,mostrarPorConsola,nivelDeLog);
}


void logInfo(const char* mensajeDeEntrada, ...){
	va_list arguments;
	va_start(arguments, mensajeDeEntrada);
	char* message = string_from_vformat(mensajeDeEntrada, arguments);
	log_info(logger,message);
	free(message);
	va_end(arguments);
}

void logDebug(const char* mensajeDeEntrada, ...){
	va_list arguments;
	va_start(arguments, mensajeDeEntrada);
	char* message = string_from_vformat(mensajeDeEntrada, arguments);
	log_debug(logger,message);
	free(message);
	va_end(arguments);
}

void logWarning(const char* mensajeDeEntrada, ...){
	va_list arguments;
	va_start(arguments, mensajeDeEntrada);
	char* message = string_from_vformat(mensajeDeEntrada, arguments);
	log_warning(logger,message);
	free(message);
	va_end(arguments);
}

void logError(const char* mensajeDeEntrada, ...){
	va_list arguments;
	va_start(arguments, mensajeDeEntrada);
	char* message = string_from_vformat(mensajeDeEntrada, arguments);
	log_error(logger,message);
	free(message);
	va_end(arguments);
}

void logTrace(const char* mensajeDeEntrada, ...){
	va_list arguments;
	va_start(arguments, mensajeDeEntrada);
	char* message = string_from_vformat(mensajeDeEntrada, arguments);
	log_trace(logger,message);
	free(message);
	va_end(arguments);
}

