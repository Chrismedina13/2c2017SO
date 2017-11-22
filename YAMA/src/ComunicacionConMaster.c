/*
 * ComunicacionConMaster.c
 *
 *  Created on: 16/9/2017
 *      Author: utnso
 */
#include "Headers/ComunicacionConMaster.h"
#include "Headers/logYAMA.h"

void comunicacionConMasters(ParametrosComunicacionConMaster* parametros) {

	//Creo Servidor Principal
	int socketYAMAServidor;
	socketYAMAServidor = lib_socketServidor(parametros->puerto);

	fd_set master;
	fd_set read_fds;
	int fd_max;
	int i;
	int FD_Cliente;
	int bytesRecibidos;
	char buffer[4];

	FD_SET(socketYAMAServidor, &master);
	fd_max = socketYAMAServidor;

	for (;;) {

		read_fds = master;
		if (select(fd_max + 1, &read_fds, NULL, NULL, NULL) == -1) {

			logInfo("Error en el select");
		}
		for (i = 0; i <= fd_max; i++) {

			if (FD_ISSET(i, &read_fds)) {

				if (i == socketYAMAServidor) {

					if ((FD_Cliente = lib_aceptarYRegistrarSocket(
							socketYAMAServidor)) == -1) {

						logInfo("Error en el aceept despues del select");

					} else {
						FD_SET(FD_Cliente, &master);
						if (FD_Cliente > fd_max) {
							fd_max = FD_Cliente;
						}
						logInfo(
								"Nueva conexion del socket cliente Master de FD: %i",
								FD_Cliente);
					}
				} else {

					//Recibo datos de algun cliente
					if ((bytesRecibidos = recv(i, buffer, 4, 0)) <= 0) {
						if (bytesRecibidos == 0) {
							logInfo("Conexion cerrada del FD : %i", i);

						}
						close(i);
						FD_CLR(i, &master);

					} else {

						int codigo = deserializarINT(buffer);
						logInfo("Recibi de Master: %i", codigo);
						mensajesRecibidosDeMaster(codigo, i);
					}
				}

			}
		}

	}
}

ParametrosComunicacionConMaster* setParametrosComunicacionConMaster(int puerto) {
	ParametrosComunicacionConMaster* parametros = malloc(
			sizeof(ParametrosComunicacionConMaster));
	parametros->puerto = puerto;
	return parametros;
}

void mensajesEnviadosAMaster(int codigo, int FDMaster,char* mensaje,int tamanio) {
	switch (codigo) {
	case SOL_TRANSFORMACION:
		logInfo("YAMA envia a Master solicitud de transformación.");
		// FALTA serializar paquete

		Paquete* paqueteSolicitudTransf = crearPaquete(codigo,
				tamanio, mensaje);

		if (enviarPaquete(FDMaster, paqueteSolicitudTransf) == -1) {
			logInfo("Error en envio de solicitud de transformación a MASTER");
		}

		destruirPaquete(paqueteSolicitudTransf);

		// Yama envia solicitud de transformacion YAMA , serializando esa lista

		break;
	case SOL_REDUCCION_LOCAL:
		logInfo("YAMA envia a Master solicitud de Reducción Local.");

		break;
	case SOL_REDUCCION_GLOBAL:
		logInfo("YAMA envia a Master solicitud de Reducción Global");


		break;
	case SOL_ALMACENADO_FINAL:
		logInfo("YAMA envia a Master solicitud de Almacenado Final");
		break;

	}
}
void mensajesRecibidosDeMaster(int codigo, int FDMaster) {
//Yama recibe de master
	char pesoMensaje[4];
	int tamanio;
	char* mensaje;
    RespuestaReduccionLocal* RRL;
    int numeroDeJobFinalReduccionLocal;
    t_list* RRG;
    int jobDeFinalizacionReduccionGlobal;
    respuestaAlmacenadoFinal* RAF;

	switch (codigo) {
	case NOMBRE_ARCHIVO:
		recv(FDMaster, pesoMensaje, 4, 0);
		tamanio = deserializarINT(pesoMensaje);
		logInfo("tamanio de lo que recibo %i", tamanio);
		mensaje = malloc(tamanio + 1);
		mensaje[tamanio] = '\0';
		recv(FDMaster, mensaje, tamanio, 0);
		logInfo("Se recibio el nombre del archivo: %s de tamanio %i", mensaje,
				tamanio);

		Job* job = crearJOB(FDMaster,mensaje);

		//enviarAMasterElnumeroDejob

		agregarJObACola(job);
		sem_post(&semaforoYAMA);
		break;

	case FIN_TRANSFORMACION:
		logInfo("YAMA recibe señal de finalización de Transformación.");
		recv(FDMaster, pesoMensaje, 4, 0);
		tamanio = deserializarINT(pesoMensaje);
		logInfo("tamanio de lo que recibo %i", tamanio);
		mensaje = malloc(tamanio + 1);
		mensaje[tamanio] = '\0';

		recv(FDMaster, mensaje, tamanio, 0);

		finTransformacion* finTrans = deserializarFinTransformacion(mensaje);

		actualizarTablaDeEstados(finTrans->numeroDeJob,FDMaster,finTrans->nodo,2,"OK");

		RRL = respuestaReduccionLocal(finTrans,FDMaster);

		//serializarRespuestaReduccionLocal(RRL);
		//mensajesEnviadosAMaster();

		agregarEntradasReduccionLocal(finTrans,RRL,FDMaster);

		break;

	case FIN_REDUCCION_LOCAL:

		//recibe numero de job
		logInfo("YAMA recibe señal de finalización de Reducción Local");
		recv(FDMaster, pesoMensaje, 4, 0);
		tamanio = deserializarINT(pesoMensaje);
		logInfo("tamanio de lo que recibo %i", tamanio);
		mensaje = malloc(tamanio + 1);
		mensaje[tamanio] = '\0';
		recv(FDMaster, mensaje, tamanio, 0);
		numeroDeJobFinalReduccionLocal = deserializarINT(mensaje);

		actualizarTablaDeEstadosFinReduccionLocal(FDMaster,numeroDeJobFinalReduccionLocal);

		RRG = respuestaReduccionGlobal(numeroDeJobFinalReduccionLocal,FDMaster);

		//serializarRespuestaReduccionLocal(RRL);

		//mensajesEnviadosAMaster();

		crearEntradasReduccionGlobal(RRG,FDMaster,numeroDeJobFinalReduccionLocal);

		break;

	case FIN_REDUCCION_GLOBAL:

		logInfo("YAMA recibe señal de finalización de Reducción Global");
		recv(FDMaster, pesoMensaje, 4, 0);
		tamanio = deserializarINT(pesoMensaje);
		logInfo("tamanio de lo que recibo %i", tamanio);
		mensaje = malloc(tamanio + 1);
		mensaje[tamanio] = '\0';
		recv(FDMaster, mensaje, tamanio, 0);

		finTransformacion* finRG = deserializarFinTransformacion(mensaje);

		actualizarTablaDeEstadosFinReduccionGlobal(finRG->numeroDeJob,FDMaster);

		RAF = RespuestaAlmacenadoFinal(finRG,FDMaster);

		//serializarRespuestaAlmacenadoFinal(RAF);
		//mensajesEnviadosAMaster
		crearEntradasAlmacenamientoFinal(RAF,finRG,FDMaster);




		break;
	case ALMACENADO_FINAL:
		logInfo("Yama recibe señal de finalización de Almacenamiento Final.");
		recv(FDMaster, pesoMensaje, 4, 0);
		tamanio = deserializarINT(pesoMensaje);
		logInfo("tamanio de lo que recibo %i", tamanio);
		mensaje = malloc(tamanio + 1);
		mensaje[tamanio] = '\0';
		recv(FDMaster, mensaje, tamanio, 0);
		break;

	default:
		logInfo("YAMA recibe un señal que no sabe traducir.");
		break;
	}
}


RespuestaReduccionLocal* respuestaReduccionLocal(finTransformacion* fin,int master){

	int i = 0;
	int j = 0;
	t_list* archivosTransformacion = list_create();
	int puerto;
	char* ip;
	char* ArchivoreduccionLocal;
	RespuestaReduccionLocal* RRL;
	while(i < list_size(listaDeJobs)){

		JOBCompleto* jobAAnalizar = list_get(listaDeJobs,i);
		if(fin->numeroDeJob == jobAAnalizar->job->identificadorJob && master == jobAAnalizar->job->master){

			JOBCompleto* jobAModificar = list_remove(listaDeJobs,i);

			while(j < list_size(jobAModificar->respuestaDePlanificacion)){
				RespuestaTransformacionYAMA * respuestaTransformacion = list_get(jobAAnalizar->respuestaDePlanificacion,j);

				if(respuestaTransformacion->nodo == fin->nodo){

					ip = respuestaTransformacion->ipWorkwer;
					puerto = respuestaTransformacion->puertoWorker;
					list_add(archivosTransformacion,respuestaTransformacion->archivoTemporal);
				}
				j++;
			}

			VariableReduccionLocal++;
			ArchivoreduccionLocal = generarNombreArchivoReduccionLocal(VariableReduccionLocal);
			RRL = crearRespuestaReduccionLocal(fin->nodo,puerto,ip,archivosTransformacion,ArchivoreduccionLocal);
			list_add(jobAModificar->respuestaReduccionLocal,RRL);
			list_add_in_index(listaDeJobs,i,jobAModificar);
			return RRL;
		}

		i++;
	}
	return NULL;
}

//devuelve una lista de respuestas de trnasformaciones globales
t_list* respuestaReduccionGlobal(int numeroDeJob,int master){

	int i = 0;
	int j = 0;
	int nodoConMenorCarga;
	t_list* respuestaReduccionGlobal = list_create();
	char* nombreArchivoReduccionGlobal;

	while(i<list_size(listaDeJobs)){
		JOBCompleto* jobAAnalizar = list_get(listaDeJobs,i);

		if(jobAAnalizar->job->master == master && jobAAnalizar->job->identificadorJob == numeroDeJob){

			JOBCompleto* jobAModificar = list_remove(listaDeJobs,i);
			nodoConMenorCarga = 0;//nodoConMenorCargaDeTrabajo(jobAModificar);

			while(j<list_size(jobAModificar->respuestaReduccionLocal)){

				RespuestaReduccionLocal* reduccionLocal = list_get(jobAModificar->respuestaReduccionLocal,j);
				if(nodoConMenorCarga == reduccionLocal->nodo){

				variableReduciionGlobal++;
				nombreArchivoReduccionGlobal = generarNombreArchivoReduccionGlobal(variableReduciionGlobal);

				RespuestaReduccionGlobal* respuestaRGEncargado = crearRespuestaReduccionGlobal(reduccionLocal->nodo,
				reduccionLocal->puertoWorker,reduccionLocal->ipWorker,reduccionLocal->archivoReduccionLocal,nombreArchivoReduccionGlobal,true);

				list_add(respuestaReduccionGlobal,respuestaRGEncargado);

				}else{

				RespuestaReduccionGlobal* respuestaRGNoencargado = crearRespuestaReduccionGlobal(reduccionLocal->nodo,
				reduccionLocal->puertoWorker,reduccionLocal->ipWorker,reduccionLocal->archivoReduccionLocal,"No es encargado",false);

				list_add(respuestaReduccionGlobal,respuestaRGNoencargado);
			}
			jobAModificar->respuesReduciionGlobal = respuestaReduccionGlobal;
			list_add_in_index(listaDeJobs,i,jobAModificar);

			j++;
			}
			return respuestaReduccionGlobal;
		}
		i++;
	}
	return NULL;
}

int nodoConMenorCargaDeTrabajo(JOBCompleto* jobC){

	int i= 0;
	int nodoConMenorTrabajo = 0;

	while(i < (list_size(jobC->respuestaReduccionLocal) -1)){
		RespuestaReduccionLocal* RRL1 = list_get(jobC->respuestaReduccionLocal,i);
		RespuestaReduccionLocal* RRL2 = list_get(jobC->respuestaReduccionLocal,(i+1));
		if(cargaDeTrabajoDelNodo(RRL1->nodo) <= cargaDeTrabajoDelNodo(RRL2->nodo)){

			nodoConMenorTrabajo = RRL1->nodo;
		}else{
			nodoConMenorTrabajo = RRL2->nodo;
		}
	}
	return nodoConMenorTrabajo;
}

int cargaDeTrabajoDelNodo(int nodo){

	int i = 0;
	while(i < list_size(listaDeWorkerTotales)){
		nodoParaPlanificar* n = list_get(listaDeWorkerTotales,i);
		if(n->nodo == nodo){

			return n->carga;
		}else{

			i++;
		}
	}
	return NULL;
}

respuestaAlmacenadoFinal* RespuestaAlmacenadoFinal(finTransformacion* finRG,int master){

	int i = 0;
	respuestaAlmacenadoFinal* respuestaAF;

	while(i < list_size(tabla_estados)){
		t_reg* registroAAnalizar = list_get(tabla_estados,i);

		if(registroAAnalizar->job == finRG->numeroDeJob && registroAAnalizar->etapa == "REDUCCION GLOBAL" && registroAAnalizar->master == master
			&& registroAAnalizar->nodo == finRG->nodo){

			Info_Workers* info = list_get(list_info_workers,finRG->nodo);

			respuestaAF = crearRespuestaAlmacenadoFinal(registroAAnalizar->nodo,info->puerto,
					info->ipWorker,registroAAnalizar->arch_temp);

			return respuestaAF;
		}
	}

	return NULL;
}






