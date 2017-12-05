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

//DEFINICION DE FUNCIONES PROBADAS
char* serializeInt(int value);


void serializarDato(char* buffer, void* datoASerializar, int tamanio,
		int* offset);
void deserializarDato(void* datoSerializado, char* buffer, int tamanio,
		int* offset);

char* serializarRespuestaTransformacionYAMA(int nodo, int puertoWorker,
		char* ipWorker, int bloque, int bytesOcupados, char* archivoTemporal);
RespuestaTransformacionYAMA *deserializarRespuestaTransformacionYAMA(
		char* rtaSerializada);
int tamanioEstructuraRespuestaTransf(RespuestaTransformacionYAMA* nodo);

char* serializarListaYAMA(t_list* lista);
t_list* deserializarListaYAMA(char* stackSerializado);

int tamanioScript(script* script);
char* serializarInfoParaWorker(int nodo, int bloque, int bytesOcupados,
		char* archivoTemporal);
infoParaWorker *deserializarInfoParaWorker(char* rtaSerializada);

//char* serializarScript(char* nombre, int tamanio, char* contenido);
char* serializarScript(script* script);
script* deserilizarScript(char* bloqueSerializado);

char * serializarLista_info_workers(t_list * listaWorkers);

t_list * deserializarLista_info_workers(char * listaWorkersSerializada);

char* serializarInfoWorker(int puerto, char* ipWorker);

Info_Workers *deserializarInfoWorker(char * infoWorkerSerializado);

char* serializarUblicacionBloqueArchivo(UbicacionBloquesArchivo * estructura);

UbicacionBloquesArchivo * deserializarUbicacionArchivo(char* UbicacionSerializado);

char * serializarUbicacionBloque(UbicacionBloque ubicacionbloque);
UbicacionBloque deserializarUbicacionBloque(char* ubicacionbloqueserializado);

char* serializarListaUbicacionBloquesArchivos(t_list * listaUbicaciones);

t_list * deserializarUbicacionBloquesArchivos(char* ListaUbicacionesSerializada);

//////////////////////////////////////DEFINICION DE FUNCIONES  SIN PROBAR//////////////////////////////////////
int deserializarINT(char* stream);

char* serializarBloque(int numeroBloque, char* contenidoBloque);

//SetBloque* deserilizarBloque(char* bloqueSerializado);

char* serializarFinTransformacion(finTransformacion* fin);
finTransformacion * deserializarFinTransformacion(char* FT);

char * serializarListaTemp(t_list * lista);
saludo_datanode *deserializar_saludo_datanode(char* saludoSerializado);

char * serializar_saludo(int nombre_nodo, int capacidad_nodo, char* ipWorker );

char* serializarRespuestaAlmacenadoFinal(respuestaAlmacenadoFinal* RAF);

respuestaAlmacenadoFinal* deserializarRespuestaAlmacenadoFinal(char* almacenadoFinalSerializado);



#endif

