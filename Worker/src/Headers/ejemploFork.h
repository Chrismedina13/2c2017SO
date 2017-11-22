/*
 * ejemploFork.h
 *
 *  Created on: 20/11/2017
 *      Author: utnso
 */

#ifndef HEADERS_EJEMPLOFORK_H_
#define HEADERS_EJEMPLOFORK_H_

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>



char* obtenerPuntero(const char* rutaArchivo);


#endif /* HEADERS_EJEMPLOFORK_H_ */
