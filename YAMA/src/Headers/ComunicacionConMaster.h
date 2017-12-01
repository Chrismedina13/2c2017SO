/*
 * ComunicacionConMaster.h
 *
 *  Created on: 16/9/2017
 *      Author: utnso
 */

#ifndef HEADERS_COMUNICACIONCONMASTER_H_
#define HEADERS_COMUNICACIONCONMASTER_H_

#include "SO_lib/sockets.h"
#include "commons/collections/list.h"
#include "SO_lib/Protocolo.h"
#include "yama.h"
#include "SO_lib/estructuras.h"
#include "SO_lib/serializacion.h"

typedef struct ParametrosComunicacionConMaster{
	int puerto;
} ParametrosComunicacionConMaster;

ParametrosComunicacionConMaster* setParametrosComunicacionConMaster(int puerto);
void comunicacionConMasters(ParametrosComunicacionConMaster* parametro);
void mensajesEnviadosAMaster(int codigo, int FDMaster,char* mensaje,int tamanio);
void mensajesRecibidosDeMaster(int codigo, int FDMaster);
RespuestaReduccionLocal* respuestaReduccionLocal(finTransformacion* fin,int master);
t_list* respuestaReduccionGlobal(int numeroDeJob,int master);
int* nodoConMenorCargaDeTrabajoParaReduccionGlobal(int master,int job);
int cargaDeTrabajoDelNodo(int* nodo);
respuestaAlmacenadoFinal* RespuestaAlmacenadoFinal(finTransformacion* finRG,int master);
t_list* listaDeArchivosTemporalesTransformacion(int job,int master,int nodo);

//Replanificacion
t_list* replanificacion(Replanificacion* replani,int master);
t_list* crearNuevaPlanificacion(t_list* respuestaTransformacion,t_list* ubicacionDeLosBloques,int nodoCaido);
UbicacionBloque* otroNodoDondeEstaLaParte(t_list* ubicacionDeLosBloques,int nodo,int bloque);
Replanificacion* deserializarReplanificacion(char* replanifSerializado);
char* serializarReplanificacion(int numeroJob,int nodoCaido);

int actualizarCargaANodo(int nodo, int cantidadDeCarga);
int nodoEncargadoDeLaReduccionGlobal(t_list* RRG);
void actualizarCargaWorkerReduccionGlobal(t_list* RRG);

void actualizarEstructurasFinalizacionDeJOB(int numeroDeJob, int master);


void asignarNodoEncargadoAJobCompleto(t_list* RRG,int master, int job);

bool estaNodoEnLaRespuestaDeTransformacion(int nodo,t_list* respuestaTransformacion);

void actualizarCargaDeTrabajoFinalizacionDeJOBYEliminarJOB(int numeroDeJob,int master);
void actualizarCargaDeTrabajoWorkerPorFinalizacion(JOBCompleto* jc);


void elNodoEncargadoEs(int nodo,int master,int job);

bool estaNodoEnlista(t_list* lista, int nodo);
int cantidadDeNodosEnLaRespuestaTransformacion(t_list* respuestaTransformacionYama);
int reducirCargaANodo(int nodo, int cantidadDeCarga);
int cantidadDeTrasformacionesYReduccionesQueHace(int nodo,t_list* respuestaTransformacionYama);
void darDeBajaUnNodoCaido(int nodoCaido);




#endif /* HEADERS_COMUNICACIONCONMASTER_H_ */
