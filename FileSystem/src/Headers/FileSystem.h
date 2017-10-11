/*
 * FileSystem.h
 *
 *  Created on: 7/9/2017
 *      Author: utnso
 */

#ifndef HEADERS_FILESYSTEM_H_
#define HEADERS_FILESYSTEM_H_
#include "configuracion.h"
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include "socketsFileSystem.h"
#include "logFileSystem.h"
#include "pthread.h"
#include "comunicacionConDN.h"
#include "comunicacionConYama.h"
#include <commons/collections/list.h>

#define ARCHIVO_CONFIGURACION "/home/utnso/tp-2017-2c-s1st3m4s_0p3r4t1v0s/FileSystem/filesystem.conf"


struct tabla_directorios {
  int index;
  char nombre[255];
  int padre;
}tabla_directorios; // registros dentro de tabla_de_directorios

struct tabla_archivos {
	int ruta; //nombre
	int tamanio;
	int tipo;
	void* bloque;
}tabla_archivos;



#endif /* HEADERS_FILESYSTEM_H_ */

