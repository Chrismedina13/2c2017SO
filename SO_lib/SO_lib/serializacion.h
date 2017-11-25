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

char* serializarBloque(int numeroBloque, char* contenidoBloque);

//SetBloque* deserilizarBloque(char* bloqueSerializado);

char * serializarUbicacionBloque(UbicacionBloque ubicacionbloque);
UbicacionBloque deserializarUbicacionBloque(char* ubicacionbloqueserializado);

char* serializarUblicacionBloqueArchivo(UbicacionBloquesArchivo * estructura);
UbicacionBloquesArchivo * deserializarUbicacionArchivo(
		char* UbicacionSerializado);

char* serializarListaUbicacionBloquesArchivos(t_list * listaUbicaciones);

t_list * deserializarUbicacionBloquesArchivos(char* ListaUbicacionesSerializada);

RespuestaTransformacionYAMA *deserializarRespuestaTransformacionYAMA(
		char* rtaSerializada);

char* serializarRespuestaTransformacionYAMA(int nodo, int puertoWorker,
		char* ipWorker, int bloque, int bytesOcupados, char* archivoTemporal);

char * serializarLista_info_workers(t_list * listaWorkers);

t_list * deserializarLista_info_workers(char * listaWorkersSerializada);

char* serializarInfoWorker(int puerto, char* ipWorker);

Info_Workers *deserializarInfoWorker(char * infoWorkerSerializado);

char * serializarListaRespuestaTransf(t_list * lista);

t_list * deserializarListaRespuestaTransf(char * listaSerializada);

char* serializarFinTransformacion(finTransformacion* fin);

finTransformacion * deserializarFinTransformacion(char* FT);

char* serializarInfoParaWorker(int nodo, int bloque, int bytesOcupados,
		char* archivoTemporal,script* scriptTransformacion);

infoParaWorker *deserializarInfoParaWorker(char* rtaSerializada);


char * serializarListaTemp(t_list * lista);

//char* serializarScript(char* nombre, int tamanio, char* contenido);
/*HAY ALGUNA RAZON PARA PASARLE LOS PARAMETROS POR SEPARADO EN VEZ DE LA ESTRUCTURA DIRECTAMENTE?*/
char* serializarScript(script* script);


char* deserializarScript(char* rtaSerializada);


saludo_datanode *deserializar_saludo_datanode(char* saludoSerializado);

char * serializar_saludo(int nombre_nodo, int capacidad_nodo, char* ipWorker );

#endif

