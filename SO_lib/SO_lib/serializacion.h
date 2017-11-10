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

char* serializarBloque(SetBloque * setbloque);

SetBloque* deserilizarBloque(char* bloqueSerializado);


char * serializarUbicacionBloque(UbicacionBloque  ubicacionbloque);
UbicacionBloque deserializarUbicacionBloque(char* ubicacionbloqueserializado);


char* serializarUblicacionBloqueArchivo(UbicacionBloquesArchivo * estructura);
UbicacionBloquesArchivo * deserializarUbicacionArchivo(char* UbicacionSerializado);

RespuestaTransformacionYAMA  *deserializarRespuestaTransformacionYAMA(char* rtaSerializada);
char* serializarRespuestaTransformacionYAMA(RespuestaTransformacionYAMA rtaTransformacionYAMA);

#endif
