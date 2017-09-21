#ifndef SERIALIZACION_H_
#define SERIALIZACION_H_

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

//defincion de funciones
void serializarDato(char* buffer, void* datoASerializar, int tamanio,
		int* offset);

void deserializarDato(void* datoSerializado, char* buffer, int tamanio,
		int* offset);

#endif
