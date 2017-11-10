#ifndef SERIALIZACION_H_
#define SERIALIZACION_H_

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include "estructuras.h"

//defincion de funciones
void serializarDato(char* buffer, void* datoASerializar, int tamanio,
		int* offset);

void deserializarDato(void* datoSerializado, char* buffer, int tamanio,
		int* offset);

int deserializarINT(char* stream);

char* serializarBloque(int numeroBloque,char* contenidoBloque);

SetBloque* deserilizarBloque(char* bloqueSerializado);


char * serializarUbicacionBloque(UbicacionBloque  ubicacionbloque);
UbicacionBloque deserializarUbicacionBloque(char* ubicacionbloqueserializado);


char* serializarUblicacionBloqueArchivo(UbicacionBloquesArchivo * estructura);
UbicacionBloquesArchivo * deserializarUbicacionArchivo(char* UbicacionSerializado);

RespuestaTransformacionYAMA  *deserializarRespuestaTransformacionYAMA(char* rtaSerializada);
char* serializarRespuestaTransformacionYAMA(char* nodo, int puertoWorker,char* ipWorker,int bloque, int bytesOcupados, char* archivoTemporal);

char * serializarLista_info_workers(t_list * listaWorkers);
t_list * deserializarLista_info_workers(char * listaWorkersSerializada );

#endif
