/*
 * configuracion.h
 *
 *  Created on: 4/9/2017
 *      Author: utnso
 */

#ifndef HEADERS_CONFIGURACIONW_H_
#define HEADERS_CONFIGURACIONW_H_

#include "commons/config.h"
#include <stddef.h>
#include <string.h>

//Keys para la configuracion
#define IP_FILESYSTEM "IP_FILESYSTEM"
#define PUERTO_FILESYSTEM1 "PUERTO_FILESYSTEM1"
#define PUERTO_FILESYSTEM2 "PUERTO_FILESYSTEM2"
#define NOMBRE_NODO "NOMBRE_NODO"
#define PUERTO_WORKER "PUERTO_WORKER"
#define PUERTO_DATANODE "PUERTO_DATANODE"
#define RUTA_DATABIN "RUTA_DATABIN"


typedef struct Configuracion {
	char* ipFileSystem;
	int puertoFileSystem1;
	int puertoFileSystem2;
	char* nombreNodo;
	int puertoWorker;
	int puertoDataNode;
	char* rutaDataBin;
} Configuracion;

Configuracion* leerArchivoDeConfiguracion(char* pathArchivoDeConfiguracion);

#endif /* HEADERS_CONFIGURACIONW_H_ */
