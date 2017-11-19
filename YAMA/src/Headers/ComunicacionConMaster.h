/*
 * ComunicacionConMaster.h
 *
 *  Created on: 16/9/2017
 *      Author: utnso
 */

#ifndef HEADERS_COMUNICACIONCONMASTER_H_
#define HEADERS_COMUNICACIONCONMASTER_H_

#include "SO_lib/sockets.h"
#include "commons/collections/list.h"
#include "SO_lib/Protocolo.h"
#include "yama.h"
#include "SO_lib/estructuras.h"
typedef struct ParametrosComunicacionConMaster{
	int puerto;
} ParametrosComunicacionConMaster;

ParametrosComunicacionConMaster* setParametrosComunicacionConMaster(int puerto);
void comunicacionConMasters(ParametrosComunicacionConMaster* parametro);
void mensajesEnviadosAMaster(int codigo, int FDMaster,char* mensaje,int tamanio);
void mensajesRecibidosDeMaster(int codigo, int FDMaster);
RespuestaReduccionLocal* repuestaTransformacionLocal(finTransformacion* fin,int master);

#endif /* HEADERS_COMUNICACIONCONMASTER_H_ */
