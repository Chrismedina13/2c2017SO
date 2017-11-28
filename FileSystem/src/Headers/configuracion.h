/*
 * configuracion.h
 *
 *  Created on: 7/9/2017
 *      Author: utnso
 */

#ifndef HEADERS_CONFIGURACION_H_
#define HEADERS_CONFIGURACION_H_
#include "FileSystem.h"
#include "commons/config.h"
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#define PUERTODN "PUERTODN"
#define PUERTOYAMA "PUERTOYAMA"
#define PUERTOWORKER "PUERTOWORKER"
#define ESTADORECUPERACION "ESTADORECUPERACION"
#define CANT_NODOS "CANT_NODOS"
#define CANT_ARCHIVOS "CANT_ARCHIVOS"

typedef struct Configuracion{
	int puerto_dn;
	int puerto_yama;
	int puerto_worker;
	int estado_recuperacion;
	int cant_archivos;
	int cant_nodos;
} Configuracion;

Configuracion* leerArchivoDeConfiguracion(char* ruta);

#endif /* HEADERS_CONFIGURACION_H_ */
