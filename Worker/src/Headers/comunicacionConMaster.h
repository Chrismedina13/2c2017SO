

#ifndef SOCKETSWORKER_H_
#define SOCKETSWORKER_H_

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "logWorker.h"
#include "commons/collections/list.h"
#include "SO_lib/sockets.h"
#include "SO_lib/Protocolo.h"
#include "SO_lib/estructuras.h"
#include "SO_lib/serializacion.h"

//#define  CONEXIONES_MAXIMAS 100 //PUDE MODIFICARSE SEGUN LO QUE PIDAN

struct sockaddr_in my_addr;
struct sockaddr_in their_addr;

typedef struct ParametrosComunicacionConMaster{
	int puertoWorker;
}ParametrosComunicacionConMaster;


void comunicacionConMaster(ParametrosComunicacionConMaster* parametros);

ParametrosComunicacionConMaster* setParametrosComunicacionConMaster(int puerto);

infoParaWorker* mensajesRecibidosDeMaster(int codigo, int FDMaster);

void mensajesEnviadosAMaster(int codigo, int FDMaster);

#endif /* SOCKETSWORKER_H_ */

