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

char* rutaScriptReduccion;
char* rutaScriptTransformador;
char* rutaScriptTransformadorInicial;
char* rutaScriptTransformadorAnual;

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
int ejecutarScriptTransformador(char* rutaScript,char* rutaArchivoAEjecutar,char* rutaArchivoAGuardar);
int ejecutarScriptReductor(char* rutaScript,char* rutaArchivoAEjecutar,char* rutaArchivoAGuardar);
void darPermisosAlLosScriptsPy(char* ruta);
void destruirArchivoOScript(char* rutaScript);
int tamanioListaDeArchivos(t_list* lista);
int tamanioScript(script script);
vectorConIndice* LApalabra(vectorConIndice* nodo1,vectorConIndice* nodo2);
void apareoDeArchivos(t_list* lista);
void apareoDeArchivosVectores(t_list* lista,char* rutaAGuardarApareo);
int indiceDelVectorQueQuedanLecturas(vectorConIndice vectorNodos[],int cantidadElementosLista);
int nodosConElementosSinLeer(vectorConIndice vectorNodos[], int cantidadElementosLista);
char* palabraMasChicaEntre(char* palabraAux1,char* palabraAux2);
void crearArchivo(char* contenido,char* rutaArchivoACrear);
char* obtenerSoloNombreDelArchivo(char* ruta);
int tamanioArchivoCerrado(const char* rutaArchivo);
int indiceDelVector(ParametroParaWorker vectorParam[], int cantidad ,RespuestaTransformacionYAMA* respuesta);
int cantidadDeNodos(t_list* listaDeRTYAMA);

int exiteEnLista(t_list* lista,int nodo);

#endif /* SO_LIB_FUNCIONESAUXILIARES_H_ */

