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

//YAMA armara una lista con esta estructura , la serializara y mandara a Master que deserealizara

typedef struct RespuestaTransformacionYAMA{
	char* nodo;
	int puertoWorker;
	char* ipWorkwer;
	int bloque;
	int bytesOcupados;
	char* archivoTemporal;

}RespuestaTransformacionYAMA;


#endif /* SO_LIB_ESTRUCTURAS_H_ */
