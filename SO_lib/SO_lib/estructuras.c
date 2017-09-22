/*
 * estructuras.c
 *
 *  Created on: 22/9/2017
 *      Author: utnso
 */

#include "estructuras.h"

RespuestaTransformacionYAMA* setRespuestaTransformacionYAMA(char* nodo,int puertoWorker,char* ipWorker,int bloque,int bytesOcupados,char* archivoTemporal){

	RespuestaTransformacionYAMA* respuesta = malloc(sizeof(RespuestaTransformacionYAMA));
	respuesta->archivoTemporal = archivoTemporal;
	respuesta->nodo = nodo;
	respuesta->puertoWorker = puertoWorker;
	respuesta->ipWorkwer = ipWorker;
	respuesta->bloque = bloque;
	respuesta->bytesOcupados = bytesOcupados;
	return respuesta;
}

void destruirTransformacionYAMA(RespuestaTransformacionYAMA* respuesta){

	//Aca hace un free del malloc de la estructura
}

