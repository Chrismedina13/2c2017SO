/*
 * FuncionesAuxiliares.h
 *
 *  Created on: 3/11/2017
 *      Author: utnso
 */

#ifndef SO_LIB_FUNCIONESAUXILIARES_H_
#define SO_LIB_FUNCIONESAUXILIARES_H_


#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <commons/collections/list.h>
#include "estructuras.h"

int generarNumeroAleatorioNoRepetido();
char* generarNombreArchivoReduccionGlobal(int variableRG);
char* generarNombreArchivoReduccionLocal(int variableRL);
char* generarNombreArchivoTransformacion(int variableTransformacion);
char* obtenerPuntero(const char* rutaArchivo);
int tamanioEstructuraInfoWorker(Info_Workers* info);
int tamanioEstructurasListaWorkers(t_list* listaWorkers);
Info_Workers* setInfoWorker(int puerto,char* ipWorker);



#endif /* SO_LIB_FUNCIONESAUXILIARES_H_ */
