/*
 * manejoDataBin.h
 *
 *  Created on: 24/11/2017
 *      Author: utnso
 */

#ifndef HEADERS_MANEJODATABIN_H_
#define HEADERS_MANEJODATABIN_H_

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include "commons/collections/list.h"
#include "commons/string.h"


char * get_bloque(int nro_bloque);

char* obtenerPuntero(const char* rutaArchivo);

int set_bloque(char * contenido, int nro_bloque);

int tamanioArchivo(int fp);

#endif /* HEADERS_MANEJODATABIN_H_ */
