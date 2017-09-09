/*
 * handler-user.c
 *
 *  Created on: 26/4/2017
 *      Author: utnso
 */

/*
#ifndef HANDLER_USER_C_
#define HANDLER_USER_C_

#include <stdbool.h>
#include <pthread.h>
#include <stdlib.h>

#include "printing-messages.h"
#include "protocol-kernel-user.h"
#include "kernel.h"
#include <dc-commons/utils.h>
#include "PCB/ansisopProcess.h"
#include "gestionDelHeap.h"
#include "TerminacionDeProcesos.h"
#include "dc-commons/protocol-kernel-console.h"
#include "planificacion.h"


int PIDprocesoParaBusquedaEnConsolaKernel;
int PIDprocesoParaOtrasBusquedas;


void handleUserRequest(kernel_struct* kernelStruct);
void mostrarTablaArchivos(kernel_struct *kernelStruct);
void informacionProceso(kernel_struct *kernelStruct,char** commands);
void modificarGradoMultiprogramacion(kernel_struct *kernelStruct,char** commands);
void detenerProceso(kernel_struct *kernelStruct,char** commands);
void detenerPlanificacion(kernel_struct *kernelStruct);
void mostrarPIDDeLosPCBDelalista(pthread_mutex_t mutex,t_list *list,char* estado);
void mostrarEstadoProcesos(pthread_mutex_t mutex,char** commands);
bool buscarPCBenListaDePCB(PCB* pcbAux);
bool buscarPCBenListaDePCB2(PCB* pcbAux);
bool buscarPCBenListaDeProgramas(Programa* programaAux);
int paginasDeHeapSolicitadas(int pidProceso);
int cantidadDeLiberarMemoriaDelProceso(int pid);
int cantidadDeAlocarDelProceso(int pid);
void datosPedidosSobreElHeap(PCB* pcb);
void activarPlanificacion(kernel_struct *kernelStruct);





#endif /* HANDLER_USER_C_ */
