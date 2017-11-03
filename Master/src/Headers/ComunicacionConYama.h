#ifndef HEADERS_COMUNICACIONCONYAMA_H_
#define HEADERS_COMUNICACIONCONYAMA_H_

#include <stdio.h>
#include <stdlib.h>
#include "configuracion.h"
#include "logMaster.h"
#include "SO_lib/sockets.h"
#include "SO_lib/estructuras.h"
#include "SO_lib/Protocolo.h"

typedef struct ParametrosComunicacionYAMA{

	int puertoYAMA;
	char* ipYAMA;

}ParametrosComunicacionYAMA;

void comunicacionYama(ParametrosComunicacionYAMA* parametros);
ParametrosComunicacionYAMA* setParametrosComunicacionYAMA(int puerto,char* ip);
void mensajesEnviadosAYama(int codigo, int FDsocketClienteYAMA, char* mensaje,int tamanio);
void mensajesRecibidosDeYama(int codigo, int FDsocketClienteYAMA);


#endif /* HEADERS_MASTER_H_ */
