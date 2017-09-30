/*
 * estructuras.c
 *
 *  Created on: 22/9/2017
 *      Author: utnso
 */

#include "estructuras.h"

RespuestaTransformacionYAMA* setRespuestaTransformacionYAMA(char* nodo,
		int puertoWorker, char* ipWorker, int bloque, int bytesOcupados,
		char* archivoTemporal) {

	RespuestaTransformacionYAMA* respuesta = malloc(
			sizeof(RespuestaTransformacionYAMA));
	respuesta->archivoTemporal = archivoTemporal;
	respuesta->nodo = nodo;
	respuesta->puertoWorker = puertoWorker;
	respuesta->ipWorkwer = ipWorker;
	respuesta->bloque = bloque;
	respuesta->bytesOcupados = bytesOcupados;
	return respuesta;
}

//void destruirTransformacionYAMA(RespuestaTransformacionYAMA* respuesta) {

	//Aca hace un free del malloc de la estructura
//}



Paquete* crearPaquete(uint32_t codigo, uint32_t tamanio, char* mensaje) {
	Paquete *Paquete = calloc(1, sizeof(Paquete));
	size_t sizeBuffer = (sizeof(char) * tamanio);
	Paquete->mensaje = calloc(1, sizeBuffer);
	memcpy(Paquete->mensaje, mensaje, sizeBuffer);
	Paquete->tamanio = tamanio;
	Paquete->codigo = codigo;
	return Paquete;
}
void destruirPaquete(Paquete* package) {
	if (package != NULL) {
		if (package->mensaje != NULL)
			free(package->mensaje);
		free(package);
	}
}

size_t sizePackage(Paquete *package) {
	size_t total;
	total = sizeof(uint32_t) + sizeof(uint32_t);
	if (package->mensaje != NULL)
		total += sizeof(char) * package->tamanio;
	return total;
}

char* serializarPaquete(Paquete *package) {

	char *serializarPaquete = calloc(1, sizePackage(package));

	int offset = 0;
	size_t size_to_send;

	size_to_send = sizeof(uint32_t);
	memcpy(serializarPaquete + offset, &(package->mensaje), size_to_send);
	offset += size_to_send;

	size_to_send = sizeof(uint32_t);
	memcpy(serializarPaquete + offset, &(package->tamanio), size_to_send);
	offset += size_to_send;

	if (package->tamanio > 0) {
		size_to_send = sizeof(char) * package->tamanio;
		memcpy(serializarPaquete + offset, package->mensaje, size_to_send);
	}

	return serializarPaquete;
}
int enviarPaquete(int fileDescriptor, Paquete *package) {
	int result;
	char* serializedPkg = serializarPaquete(package);
	result = send(fileDescriptor, serializedPkg, sizePackage(package), 0);
	free(serializedPkg);
	if (result == -1)
		return -1; //SI DA ERROR

	return 1; //SI ESTA OK
}

