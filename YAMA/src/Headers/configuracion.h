/*
 * configuracion.h
 *
 *  Created on: 4/9/2017
 *      Author: utnso
 */

#ifndef HEADERS_CONFIGURACION_H_
#define HEADERS_CONFIGURACION_H_

#include "commons/config.h"
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

//Keys para la configuracion
#define FS_IP "FS_IP"
#define FS_PUERTO "FS_PUERTO"
#define RETARDO_PLANIFICACION "RETARDO_PLANIFICACION"
#define ALGORITMO_BALANCEO "ALGORITMO_BALANCEO"

typedef struct Configuracion {
	char* ipFileSystem;
	int puertoFileSystem;
	int retardo;
	char* algoritmo_bal;
} Configuracion;

Configuracion* leerArchivoDeConfiguracion(char* pathArchivoDeConfiguracion);

#endif /* HEADERS_CONFIGURACION_H_ */