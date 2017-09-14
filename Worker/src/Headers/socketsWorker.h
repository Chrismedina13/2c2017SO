

#ifndef SOCKETSWORKER_H_
#define SOCKETSWORKER_H_

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "logWorker.h"
#include "commons/collections/list.h"
#define  CONEXIONES_MAXIMAS 30 //PUDE MODIFICARSE SEGUN LO QUE PIDAN

struct sockaddr_in my_addr;
struct sockaddr_in their_addr;

int socketServidor(int puerto);
void comunicacionCoMaster(int puertoWorker,t_list* mastersConectados);
int aceptarYRegistrarSocket(int socketServidor,t_list* conectados);

#endif /* SOCKETSWORKER_H_ */

