/*
 * bloques.h
 *
 *  Created on: 2/12/2017
 *      Author: utnso
 */

#ifndef SO_LIB_BLOQUES_H_
#define SO_LIB_BLOQUES_H_

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include "commons/collections/list.h"
#include "commons/string.h"


char * get_bloque(int nro_bloque);

int set_bloque(char * contenido, int nro_bloque);


int tamanioArchivo(int fp);


#endif /* SO_LIB_BLOQUES_H_ */
