/*
 * configuracion.h
 *
 *  Created on: 7/9/2017
 *      Author: utnso
 */

#ifndef HEADERS_CONFIGURACION_H_
#define HEADERS_CONFIGURACION_H_

#include "commons/config.h"
#include <stddef.h>
#include <string.h>

//Keys para la configuracion
#define YAMA_IP "YAMA_IP"
#define YAMA_PUERTO "YAMA_PUERTO"
#define CANT_NODOS "CANT_NODOS"

typedef struct Configuracion {
	char* ipYama;
	int puertoYama;
	int cantNodos;

} Configuracion;

Configuracion* leerArchivoDeConfiguracion(char* pathArchivoDeConfiguracion);

#endif /* HEADERS_CONFIGURACION_H_ */
