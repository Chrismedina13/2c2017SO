/*
 * handler-user.c
 *
 *  Created on: 26/4/2017
 *      Author: utnso
 */

#include "handler-user.h"
#include <commons/collections/queue.h>
#include <stdbool.h>


void handleUserRequest(kernel_struct* kernelStruct){

	bool exit = false;
	bool shouldCompareCommand;
	while (!exit) {
		printNewLine(kernelStruct->stdoutMutex);
		char** commands = get_user_commands();
		shouldCompareCommand = true;

		if (*commands != NULL) {
			if (shouldCompareCommand && equal_user_command(commands[0], COD_CONSOLE_HELP)) {
				shouldCompareCommand = false;
				printCommandsHelp(kernelStruct->stdoutMutex);
			}
		}

		if (*commands != NULL) {
			if (shouldCompareCommand && equal_user_command(commands[0], COD_ACTIVAR_PLANIFICACION)) {
				shouldCompareCommand = false;
				activarPlanificacion(kernelStruct);
			}
		}

		if (*commands != NULL) {
			if (shouldCompareCommand && equal_user_command(commands[0], COD_DETENER_PLANIFICACION)) {
				shouldCompareCommand = false;
				logInfo("Entra aca");
				detenerPlanificacion(kernelStruct);
			}
		}

		if (*commands != NULL) {
			if (shouldCompareCommand && equal_user_command(commands[0], COD_DETENER_PROCESO)) {
				shouldCompareCommand = false;
				detenerProceso(kernelStruct,commands);
			}
		}

		if (*commands != NULL) {
			if (shouldCompareCommand && equal_user_command(commands[0], COD_ESTADOS_PROCESOS)) {
				shouldCompareCommand = false;
				mostrarEstadoProcesos(kernelStruct->stdoutMutex,commands);
			}
		}

		if (*commands != NULL) {
			if (shouldCompareCommand && equal_user_command(commands[0], COD_GRADO_MULTIPROGRAMACION)) {
				shouldCompareCommand = false;
				modificarGradoMultiprogramacion(kernelStruct,commands);
			}
		}

		if (*commands != NULL) {
			if (shouldCompareCommand
					&& equal_user_command(commands[0], COD_INFORMACION_PROCESO)) {
				shouldCompareCommand = false;
				informacionProceso(kernelStruct,commands);
			}
		}

		if (*commands != NULL) {
			if (shouldCompareCommand && equal_user_command(commands[0], COD_TABLA_ARCHIVOS)) {
				shouldCompareCommand = false;
				mostrarTablaArchivos(kernelStruct);
			}
		}

		if (shouldCompareCommand) {
			pthread_mutex_lock(&(kernelStruct->stdoutMutex));
			printf("«%s» no es un comando válido. Si necesita ayuda use: «%s»",
					commands[0], COD_CONSOLE_HELP);
			pthread_mutex_unlock(&(kernelStruct->stdoutMutex));
		}

		free_user_commands(commands);
	}
}

void detenerPlanificacion(kernel_struct *kernelStruct){

	logInfo("Entra aca 2");

	//Bloqueo todos los semanforos de las colas de estados para que no ingrese ningun programa
	puts("Deteniendo Planificacion ....");
	pthread_mutex_lock(&mutexNew);
	pthread_mutex_lock(&mutexReady);
	pthread_mutex_lock(&mutexExec);
	pthread_mutex_lock(&mutexBloq);
	pthread_mutex_lock(&mutexExit);
	puts("Planificacion Detenida");

}

void activarPlanificacion(kernel_struct *kernelStruct){

	//}Desbloqueo todos los semanforos de las colas de estados para que no ingrese ningun programa
	puts("Activando Planificacion ....");
	pthread_mutex_unlock(&mutexNew);
	pthread_mutex_unlock(&mutexReady);
	pthread_mutex_unlock(&mutexExec);
	pthread_mutex_unlock(&mutexBloq);
	pthread_mutex_unlock(&mutexExit);
	puts("Planificacion Activada ..");


}



void detenerProceso(kernel_struct *kernelStruct,char** commands){

	int PID = atoi(commands[1]);
	printf("Detener Proceso %i\n",PID);

	PIDprocesoParaBusquedaEnConsolaKernel = PID;

	if(list_any_satisfy(estados->new,buscarPCBenListaDeProgramas)){

		logInfo("El programa esta en la cola de NEW");
		Programa* programa = quitarDeNewPorPID(PID);
		programa->pcb->exitCod = FINALIZACION_POR_CONSOLA_KERNEL;
		logInfo("Avisando a consola la finalizacion del proceso PID : %i", PID);
		createAndSendPackage(programa->pcb->FDConsola,COD_KC_END_PROGRAM,strlen("Fin por consola kernel"),"Fin por consola kernel");
		moverAExit(programa->pcb);
		free(programa->codigo);
	}else{
		if(list_any_satisfy(estados->ready,buscarPCBenListaDePCB)){

			logInfo("El programa esta en la cola de READY");
			PCB* pcb = quitarDeReadyPorPID(PID);
			pcb->exitCod = FINALIZACION_POR_CONSOLA_KERNEL;
			logInfo("Avisando a consola la finalizacion del proceso PID : %i", PID);
			createAndSendPackage(pcb->FDConsola,COD_KC_END_PROGRAM,strlen("Fin por consola kernel"),"Fin por consola kernel");
			logInfo("Avisando a memoria la finalizacion de proceso PID : %i", PID);
			char* pidSerializado = serialize_int(PID);
			createAndSendPackage(kernelStruct->socketClientMemoria,COD_FINALIZAR_PROGRAMA,sizeof(int),pidSerializado);
			free(pidSerializado);
			moverAExit(pcb);
		}else{

			if(list_any_satisfy(estados->bloq,buscarPCBenListaDePCB)){

				logInfo("El programa esta en la cola de Bloquqados ");
				PCB* pcb = quitarDeBLoqPorPID(PID);
				pcb->exitCod = FINALIZACION_POR_CONSOLA_KERNEL;
				logInfo("Avisando a consola la finalizacion del proceso PID : %i", PID);
				createAndSendPackage(pcb->FDConsola,COD_KC_END_PROGRAM,strlen("Fin por consola kernel"),"Fin por consola kernel");
				logInfo("Avisando a memoria la finalizacion de proceso PID : %i", PID);
				char* pidSerializado = serialize_int(PID);
				createAndSendPackage(kernelStruct->socketClientMemoria,COD_FINALIZAR_PROGRAMA,sizeof(int),pidSerializado);
				free(pidSerializado);
				moverAExit(pcb);
			}else{
				if(list_any_satisfy(estados->exec,buscarPCBenListaDePCB)){

					logInfo("El programa esta en la cola de Exec");
					//aca se me complico
				}else{

					puts("No se encuentra un programa con ese PID");
				}



			}
		}

	}


}

void mostrarEstadoProcesos(pthread_mutex_t mutex,char** commands){

	char* Ready = "Ready";
	char* Exec = "Exec";
	char* Exit = "Exit";
	char* Bloq = "Bloq";

	if(commands[1] != NULL){



	if(equal_user_command(commands[1],ALL_ESTADOS)){

		if((programasEnElSistema()+list_size(estados->new)+list_size(estados->exit))>0){
			pthread_mutex_lock(&mutex);

			int a;
			for(a=0; list_is_empty(estados->new) != 1 && a <= (list_size(estados->new)); a++){

				Programa *prog = list_get(estados->new,a);
				printf("En la cola de new esta el programa: %i \n",prog->pcb->PID);

			}
			pthread_mutex_unlock(&mutex);

			mostrarPIDDeLosPCBDelalista(mutex,estados->ready,Ready);
			mostrarPIDDeLosPCBDelalista(mutex,estados->exec,Exec);
			mostrarPIDDeLosPCBDelalista(mutex,estados->exit,Exit);
			mostrarPIDDeLosPCBDelalista(mutex,estados->bloq,Bloq);
		}
		else{

			puts("No hay programas en el sistema");
		}
	}

	if(equal_user_command(commands[1],NEW)){//Muestra solo los programas que estan en ready

		if(!list_is_empty(estados->new)){

			pthread_mutex_lock(&mutex);
			int b;
			for(b=0; list_is_empty(estados->new) != 1 && b <= (list_size(estados->new)); b++){

				Programa *prog = list_get(estados->new,b);
				printf("En la cola de new esta el programa: %i \n",prog->pcb->PID);

			}

			pthread_mutex_unlock(&mutex);
		}
		else{
			puts("No hay programas en la cola de new");
		}
	}

	if(equal_user_command(commands[1],READY)){

		if( !list_is_empty(estados->ready)){
			mostrarPIDDeLosPCBDelalista(mutex,estados->ready,Ready);
		}
		else{
			puts("No hay programas en la cola de ready");
		}
	}

	if(equal_user_command(commands[1],EXEC)){

		if( !list_is_empty(estados->exec)){
			mostrarPIDDeLosPCBDelalista(mutex,estados->exec,Exec);
		}
		else{
			puts("No hay programas en la cola de exec");
		}
	}

	if(equal_user_command(commands[1],BLOQ)){

		if( !list_is_empty(estados->bloq)){
			mostrarPIDDeLosPCBDelalista(mutex,estados->bloq,Bloq);
		}
		else{
			puts("No hay programas en la cola de bloq");
		}
	}

	if(equal_user_command(commands[1],EXIT)){

		if( !list_is_empty(estados->exit)){
			mostrarPIDDeLosPCBDelalista(mutex,estados->exit,exit);
		}
		else{
			puts("No hay programas en la cola de exit");
		}
	}

	}
	else{

		puts("No es un comando valido para estados");
	}

}

void modificarGradoMultiprogramacion(kernel_struct *kernelStruct,char** commands){

	int grado = atoi(commands[1]);
	printf("Modificar grado multiprogramacion por %i \n",grado);
	kernelStruct->config->grado_multiprog = grado;

}
//todo: falta probar informacion proceso cuando se carguen programas en el sistema

void informacionProceso(kernel_struct *kernelStruct,char** commands){

	PCB *pcb;
	Programa* programa;

	PIDprocesoParaBusquedaEnConsolaKernel = atoi(commands[1]);
	printf("Informacion de proceso %i \n",PIDprocesoParaBusquedaEnConsolaKernel);


	if( list_find(estados->new,buscarPCBenListaDeProgramas) != NULL ){

		programa =  list_find(estados->new,buscarPCBenListaDeProgramas);
		datosPedidosSobreElHeap(programa->pcb);
	}
	else
	{
		if(list_find(estados->ready,buscarPCBenListaDePCB) != NULL){

			pcb =  list_find(estados->ready,buscarPCBenListaDePCB);
			datosPedidosSobreElHeap(pcb);
		}
		else
		{
			if( list_find(estados->exec,buscarPCBenListaDePCB) != NULL ){

				pcb =  list_find(estados->exec,buscarPCBenListaDePCB);
				datosPedidosSobreElHeap(pcb);
			}
			else{

				if(list_find(estados->bloq,buscarPCBenListaDePCB) != NULL){

					pcb =  list_find(estados->bloq,buscarPCBenListaDePCB);
					datosPedidosSobreElHeap(pcb);
				}
				else{

					if(list_find(estados->exit,buscarPCBenListaDePCB) != NULL){

						pcb =  list_find(estados->exit,buscarPCBenListaDePCB);
						datosPedidosSobreElHeap(pcb);
					}
					else{

						puts("NO HAY PROCESOS CON ESE PID");
					}
				}
			}
		}
	}

}


void mostrarTablaArchivos(kernel_struct *kernelStruct){
	//Por ahora no hace nada
	puts("Mostrar tabla de archivos del sistema");
}

void mostrarPIDDeLosPCBDelalista(pthread_mutex_t mutex,t_list *list,char* estado){

	pthread_mutex_lock(&mutex);

	int a;

	for(a=0; !list_is_empty(list) && a < (list_size(list)); a++){

		PCB *pcb = list_get(list,a);
		printf("En la cola de %s esta el programa: %i \n",estado,pcb->PID);
	}

	pthread_mutex_unlock(&mutex);

}

bool buscarPCBenListaDePCB(PCB* pcbAux){
    // posibilidad de semaforos
	return pcbAux->PID == PIDprocesoParaBusquedaEnConsolaKernel;
}

bool buscarPCBenListaDePCB2(PCB* pcbAux){
    // posibilidad de semaforos
	return pcbAux->PID == PIDprocesoParaOtrasBusquedas;
}

bool buscarPCBenListaDeProgramas(Programa* programaAux){

	return programaAux->pcb->PID == PIDprocesoParaBusquedaEnConsolaKernel;
}


int paginasDeHeapSolicitadas(int pidProceso){

	int a ;
	int b = 0 ;
	for(a=0;a<list_size(tablaDeAdminstradoresDeHeap);a++){

		Admin_Heap* admin = list_get(tablaDeAdminstradoresDeHeap,a);
		if(admin->pidProceso == pidProceso){

			b++;
		}
	}
	return b;
}

int cantidadDeAlocarDelProceso(int pid){

	int a ;
		int b = 0 ;
		for(a=0;a<list_size(tablaDeAdminstradoresDeHeap);a++){

			Admin_Heap* admin = list_get(tablaDeAdminstradoresDeHeap,a);
			if(admin->pidProceso == pid){

				b =+ admin->cantidadDeAlocarSobrePagina;
			}
		}
		return b;
}

int cantidadDeLiberarMemoriaDelProceso(int pid){

	int a ;
		int b = 0 ;
		for(a=0;a<list_size(tablaDeAdminstradoresDeHeap);a++){

			Admin_Heap* admin = list_get(tablaDeAdminstradoresDeHeap,a);
			if(admin->pidProceso == pid){

				b =+ admin->cantidadDeDeliberarSobrePagina;
			}
	}
		return b;

}

void datosPedidosSobreElHeap(PCB* pcb){

	printf("mostrar PROCESO PID = %i \n",pcb->PID);
	printf("Cantidad rafagas ejecutadas = %i \n",pcb->rafagasEjecutadas);
	printf("Cantidad SysCall = %i \n",pcb->numeroDeSCall);
	printf("Cantidad paginas De Heap = %i \n", paginasDeHeapSolicitadas(pcb->PID));
	printf("Cantidad Alocar memoria realizadas = %i \n", cantidadDeAlocarDelProceso(pcb->PID));
	printf("Cantidad Desalocar memoria realizadas = %i \n", cantidadDeLiberarMemoriaDelProceso(pcb->PID));
	//printf("Cantidad de memoria solicitada en Heap = %i \n",);
	//printf("Cantidad de liberada en Heap = %i \n",);
}
