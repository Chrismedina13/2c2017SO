/*
 * DataNode.h
 *
 *  Created on: 7/9/2017
 *      Author: utnso
 */

#ifndef HEADERS_DATANODE_H_
#define HEADERS_DATANODE_H_

#include <stdio.h>
#include <stdlib.h>
#include "configuracionDN.h"
#include "logDataNode.h"
#include <SO_lib/sockets.h>
#include "SO_lib/estructuras.h"
#include "SO_lib/Protocolo.h"
#include "SO_lib/serializacion.h"

#define ARCHIVO_CONFIGURACION "/home/utnso/tp-2017-2c-s1st3m4s_0p3r4t1v0s/Nodo.conf"


void mensajesRecibidosDeFileSystem(int codigo, int FD_FileSystem);
void mensajesEnviadosAFileSystem(int codigo, int FD_FileSystem, char* mensaje, int tamanio);
#endif /* HEADERS_DATANODE_H_ */
