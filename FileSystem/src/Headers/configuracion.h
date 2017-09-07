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
#define PUERTO "PUERTO"

typedef struct Configuracion{
	int puerto;
} Configuracion;

Configuracion* leerArchivoDeConfiguracion(char* ruta);

#endif /* HEADERS_CONFIGURACION_H_ */
