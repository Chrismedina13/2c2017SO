/*
 * estructuras.h
 *
 *  Created on: 22/9/2017
 *      Author: utnso
 */

#ifndef SO_LIB_ESTRUCTURAS_H_
#define SO_LIB_ESTRUCTURAS_H_
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "commons/collections/list.h"
#include "sockets.h"
//YAMA armara una lista con esta estructura , la serializara y mandara a Master que deserealizara

typedef struct RespuestaTransformacionYAMA {
	char* nodo;
	int puertoWorker;
	char* ipWorkwer;
	int bloque;
	int bytesOcupados;
	char* archivoTemporal;

} RespuestaTransformacionYAMA;

//paquete para mandar y recibir mensajes
typedef struct Paquete {
	uint32_t codigo;
	uint32_t tamanio;
	char* mensaje;
} Paquete;
//nodoParaPlanificar es un nodo de la lista de listaDeWorkerTotales
typedef struct nodo {

	int nodo;
	int disponibilidad;
	int carga;
	t_list* partesDelArchivo;
	t_list* partesAplanificar;
} nodoParaPlanificar;

typedef struct ubicacionBloque {

	int nodo;  //Nodo donde se encuentra la copia
	int bloqueDelNodoDeLaCopia; // Bloque adentro del nodo donde se encuentra
} UbicacionBloque;

typedef struct ubiacionBloquesArchivo {

	int parteDelArchivo; //numero del bloque del archivo
	UbicacionBloque* ubicacionCopia1; //ubicacion copia 1
	UbicacionBloque* ubicacionCopia2; //ubicacion copia 2
	int bytesOcupados; //bytesOCupados por ese bloque
} UbicacionBloquesArchivo;

//funciones
nodoParaPlanificar* crearNodoParaPlanificar(int nodo, int disponibilidad,
		int carga,int bloque);
void DestruirNodoParaPlanificar(nodoParaPlanificar* nodo);
RespuestaTransformacionYAMA* setRespuestaTransformacionYAMA(char* nodo,
		int puertoWorker, char* ipWorker, int bloque, int bytesOcupados,
		char* archivoTemporal);
Paquete* crearPaquete(uint32_t codigo, uint32_t tamanio, char* mensaje);
void destruirPaquete(Paquete* package);
size_t sizePackage(Paquete *package);
char* serializarPaquete(Paquete *package);
int enviarPaquete(int fileDescriptor, Paquete *package);
#endif /* SO_LIB_ESTRUCTURAS_H_ */
