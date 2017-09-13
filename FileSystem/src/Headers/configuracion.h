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
#include <stdlib.h>
#include <stdio.h>
#define PUERTO1 "PUERTO1"
#define PUERTO2 "PUERTO2"

typedef struct Configuracion{
	int puerto1;
	int puerto2;
} Configuracion;

Configuracion* leerArchivoDeConfiguracion(char* ruta);

#endif /* HEADERS_CONFIGURACION_H_ */
