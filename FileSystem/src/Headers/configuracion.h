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
#define PUERTODN "PUERTODN"
#define PUERTOYAMA "PUERTOYAMA"
#define PUERTOWORKER "PUERTOWORKER"

typedef struct Configuracion{
	int puerto_dn;
	int puerto_yama;
	int puerto_worker;
} Configuracion;

Configuracion* leerArchivoDeConfiguracion(char* ruta);

#endif /* HEADERS_CONFIGURACION_H_ */
