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
#include "Protocolo.h"



int generarNumeroAleatorioNoRepetido();
char* generarNombreArchivoReduccionGlobal(int variableRG);
char* generarNombreArchivoReduccionLocal(int variableRL);
char* generarNombreArchivoTransformacion(int variableTransformacion);
char* obtenerPuntero(const char* rutaArchivo);
int tamanioEstructuraInfoWorker(Info_Workers* info);
int tamanioEstructurasListaWorkers(t_list* listaWorkers);
void setInfoWorker(Info_Workers* info,int puerto,char* ipWorker);
script* setScript(char* rutaScript);
void rearmar_script(script* script,int codigo);
int ejecutarScript(char* rutaScript,char* rutaArchivo)//Le paso la ruta del script y el archivo al que quiero aplicar el script;

#endif /* SO_LIB_FUNCIONESAUXILIARES_H_ */
