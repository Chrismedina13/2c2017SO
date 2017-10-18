/*
 * pruebas.h
 *
 *  Created on: 11/10/2017
 *      Author: utnso
 */

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include "commons/collections/list.h"
#include "commons/string.h"

int pasarAMemoria (const char*,char* );
