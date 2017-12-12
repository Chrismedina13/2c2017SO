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
	cantNodosConectados = 0;
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

void mensajesEnviadosAMaster(int codigo, int FDMaster, char* mensaje,
		int tamanio) {

	Paquete* paqueteSolicitudTransf;
	Paquete* paqueteEnvioDeReduccionLocal;
	Paquete* paqueteEnvioReduccionGlobal;
	Paquete* paqueteEnvioAlmacenadoFinal;

	switch (codigo) {

	case NUMERO_DE_JOB:
		logInfo(
				"YAMA se prepara para enviar a Master el identificador de job.");

		paqueteSolicitudTransf = crearPaquete(codigo, tamanio, mensaje);

		if (enviarPaquete(FDMaster, paqueteSolicitudTransf) == -1) {
			logInfo("Error en envio de solicitud de transformación a MASTER");
		}

		destruirPaquete(paqueteSolicitudTransf);

		logInfo("YAMA envia a Master el identificador de job.");

		break;
	case REDUCCION_LOCAL:
		logInfo("YAMA envia a Master la respuesta de reduccion Local");

		paqueteEnvioDeReduccionLocal = crearPaquete(codigo, tamanio, mensaje);

		if (enviarPaquete(FDMaster, paqueteEnvioDeReduccionLocal) == -1) {
			logInfo("Error en envio de reduccion local a MASTER");
		} else {
			logInfo(
					"se envio exitosamente la solicitud de reduccion local a MASTER");

		}

		destruirPaquete(paqueteEnvioDeReduccionLocal);

		break;
	case REDUCCION_GLOBAL:
		logInfo("YAMA envia a Master la respuesta de Reduccion global");

		paqueteEnvioReduccionGlobal = crearPaquete(codigo, tamanio, mensaje);

		if (enviarPaquete(FDMaster, paqueteEnvioReduccionGlobal) == -1) {
			logInfo("Error en envio de reduccion local a MASTER");
		} else {
			logInfo(
					"se envio exitosamente la solicitud de reduccion local a MASTER");

		}

		destruirPaquete(paqueteEnvioReduccionGlobal);

		break;
	case SOL_ALMACENADO_FINAL:
		logInfo("YAMA envia a Master solicitud de Almacenado Final");
		paqueteEnvioAlmacenadoFinal = crearPaquete(codigo, tamanio, mensaje);

		if (enviarPaquete(FDMaster, paqueteEnvioAlmacenadoFinal) == -1) {
			logInfo("Error en envio de reduccion local a MASTER");
		} else {
			logInfo(
					"se envio exitosamente la solicitud de reduccion local a MASTER");

		}

		destruirPaquete(paqueteEnvioAlmacenadoFinal);

		break;

	}
}
void mensajesRecibidosDeMaster(int codigo, int FDMaster) {

	//Reduccion local
	char* mensajeReduccionLocalSerializado;
	int tamanioDelMensajeDeReduccionLocal;
	RespuestaReduccionLocal* RRL;
	char pesoARecibir[4];
	int tamanioDeLafinalizacionDeTransformacion;
	char* mensajeSerializadoFinTransformacion;
	finTransformacion* finTrans;

	//Reduccion Global
	char pesoARecibirRG[4];
	int tamanioDeLoQueReciboDelMensaje;
	char* numeroDeJobQueTerminoRL;
	t_list* RRG;
	int numeroDeJobFinalReduccionLocal;
	char* respuesReduccionGlobalSerializado;
	int tamanioDeLaRespuestaDeReduccionGlobal;

	//Almacenado Final
	respuestaAlmacenadoFinal* RAF;
	char pesoMensajeFRG;
	int tamanioDeFRG;
	char * mensajeFinReduccionGlobal;
	finTransformacion* finRG;
	char* almacenadoFinalSerializado;
	int tamanioDeLarespuestaAlmacenadoFinal;

	//Fin de job

	char pesoFJ[4];
	int tamanioDeLaFinalizacionDeJob;
	char* mesnajeFinalizacionDeJob;
	int jobQueFinalizo;

	char pesoMensaje[4];
	int tamanio;
	char* mensaje;
	Replanificacion* parametrosReplanif;
	t_list* nuevaPlanificacion;

	switch (codigo) {
	case REPLANIFICACION:
		recv(FDMaster, pesoMensaje, 4, 0);
		tamanio = deserializarINT(pesoMensaje);
		logInfo("tamanio de lo que recibo %i", tamanio);
		mensaje = malloc(tamanio + 1);
		mensaje[tamanio] = '\0';
		recv(FDMaster, mensaje, tamanio, 0);

		parametrosReplanif = deserializarReplanificacion(mensaje);

		//le saca 1 de carga por cada transformacion que hacia
		actualizarCargaDeWorkersPorReplanificacion(
				parametrosReplanif->nodoCaido, parametrosReplanif->numeroDeJOb,
				FDMaster);

		darDeBajaUnNodoCaido(parametrosReplanif);

		//Planificar nuevamente
		nuevaPlanificacion = replanificacionNueva(parametrosReplanif, FDMaster);

		actualizarNodosCaidosReplanificacion(parametrosReplanif, FDMaster);

		//serializarRespuestaTransformacionYAMA(nuevaPlanificacion);

		//mensajesEnviadosAmaster();

		insertarNodosNuevosPlanificados(nuevaPlanificacion, FDMaster,
				parametrosReplanif->numeroDeJOb);

		break;
	case NOMBRE_ARCHIVO:
		recv(FDMaster, pesoMensaje, 4, 0);
		tamanio = deserializarINT(pesoMensaje);
		logInfo("tamanio de lo que recibo %i", tamanio);
		mensaje = malloc(tamanio + 1);
		mensaje[tamanio] = '\0';
		recv(FDMaster, mensaje, tamanio, 0);
		logInfo("Se recibio el nombre del archivo: %s de tamanio %i", mensaje,
				tamanio);

		Job* job = crearJOB(FDMaster, mensaje);

		char* numeroDeJob;

		numeroDeJob = serializeInt(job->identificadorJob);
		mensajesEnviadosAMaster(NUMERO_DE_JOB, FDMaster, numeroDeJob, 4);

		logInfo("Envio De JOB a FS");

		int tamanioJOB = strlen(job->nombreDelArchivo);
		Paquete* paqueteDeEnvioDeJOB = crearPaquete(NOMBRE_ARCHIVO, tamanioJOB,
				job->nombreDelArchivo);

		if (enviarPaquete(FDsocketClienteFileSystem, paqueteDeEnvioDeJOB)
				== -1) {
			logInfo("Error en envio de job");
		} else {
			logInfo("Envio correcto de nombre de job a FS");
		}

		destruirPaquete(paqueteDeEnvioDeJOB);

		logInfo("Esperando Respuesta de las ubicaciones de las partes del job");

		agregarJObACola(job);

		atenderJOB();
		free(mensaje);
		free(numeroDeJob);

		break;

	case FIN_TRANSFORMACION:

		logInfo("Se recibe la finalizacion de la transformacion de un nodo");
		recv(FDMaster, pesoARecibir, 4, 0);
		tamanioDeLafinalizacionDeTransformacion = deserializarINT(pesoARecibir);
		logInfo("tamanio de lo que recibo %i",
				tamanioDeLafinalizacionDeTransformacion);
		mensajeSerializadoFinTransformacion = malloc(
				tamanioDeLafinalizacionDeTransformacion + 1);
		mensajeSerializadoFinTransformacion[tamanioDeLafinalizacionDeTransformacion] =
				'\0';

		recv(FDMaster, mensajeSerializadoFinTransformacion,
				tamanioDeLafinalizacionDeTransformacion, 0);

		finTrans = deserializarFinTransformacion(mensaje);
		logInfo("en nodo %i termino la reduccion del job %i", finTrans->nodo,
				finTrans->numeroDeJob);

		logInfo(
				"Actualizo la tabla de estados con la transformacion correcta del job");
		actualizarTablaDeEstados(finTrans->numeroDeJob, FDMaster,
				finTrans->nodo, 2, "OK");

		logInfo("Creo Respuesta Reduccion Local ");
		RRL = respuestaReduccionLocal(finTrans, FDMaster);

		tamanioDelMensajeDeReduccionLocal = strlen(RRL->ipWorker)
				+ list_size(RRL->archivosDeTransformacion)
				+ strlen(RRL->archivoReduccionLocal)
				+ tamanioListaDeArchivos(RRL->archivosDeTransformacion)
				+ sizeof(int) * 6;

		char *respuesReduccionLocalSerializada =
				serializarRespuestaReduccionLocal(RRL);
		mensajesEnviadosAMaster(REDUCCION_LOCAL, FDMaster,
				respuesReduccionLocalSerializada,
				tamanioDelMensajeDeReduccionLocal);

		logInfo("Actualizo la tabla De Estados");
		agregarEntradasReduccionLocal(finTrans, RRL, FDMaster);

		free(mensaje);
		free(finTrans);

		break;

	case FIN_REDUCCION_LOCAL:

		logInfo("YAMA recibe señal de finalización de Reducción Local");
		recv(FDMaster, pesoARecibirRG, 4, 0);
		tamanioDeLoQueReciboDelMensaje = deserializarINT(pesoARecibirRG);
		logInfo("tamanio de lo que recibo %i", tamanioDeLoQueReciboDelMensaje);
		numeroDeJobQueTerminoRL = malloc(tamanioDeLoQueReciboDelMensaje + 1);
		numeroDeJobQueTerminoRL[tamanioDeLoQueReciboDelMensaje] = '\0';
		recv(FDMaster, numeroDeJobQueTerminoRL, tamanioDeLoQueReciboDelMensaje,
				0);

		logInfo("Recibo el numero de job que termina la Reduccion Local");
		numeroDeJobFinalReduccionLocal = deserializarINT(mensaje);

		logInfo(
				"Actualizo la tabla de estados por finalizacion de reduccion local");
		actualizarTablaDeEstadosFinReduccionLocal(FDMaster,
				numeroDeJobFinalReduccionLocal);

		logInfo("creo la lista de respuesta de reduccion Global");
		RRG = respuestaReduccionGlobal(numeroDeJobFinalReduccionLocal,
				FDMaster);

		logInfo("Serializo la lista de respues de reduccion global");
		tamanioDeLaRespuestaDeReduccionGlobal = tamanioListareduccionGlobal(RRG)
				+ sizeof(int) + (sizeof(int) * list_size(RRG) * 4);
		respuesReduccionGlobalSerializado =
				serializarListaRespuestaReduccionGlobal(RRG);

		mensajesEnviadosAMaster(REDUCCION_GLOBAL, FDMaster,
				respuesReduccionGlobalSerializado,
				tamanioDeLaRespuestaDeReduccionGlobal);

		logInfo(
				"actualizar la tabla de estados por la respuesta de reduccion global");
		crearEntradasReduccionGlobal(RRG, FDMaster,
				numeroDeJobFinalReduccionLocal);

		logInfo("Asignar nodo encargado a Job Completo");
		asignarNodoEncargadoAJobCompleto(RRG, FDMaster,
				numeroDeJobFinalReduccionLocal);

		logInfo("Actualizar carga de worker por la reduccion global");
		actualizarCargaWorkerReduccionGlobal(RRG);

		break;

	case FIN_REDUCCION_GLOBAL:
		logInfo("YAMA recibe señal de finalización de Reducción Global");

		recv(FDMaster, pesoMensajeFRG, 4, 0);
		tamanioDeFRG = deserializarINT(pesoMensajeFRG);
		logInfo("tamanio de lo que recibo %i", tamanioDeFRG);
		mensajeFinReduccionGlobal = malloc(tamanioDeFRG + 1);
		mensajeFinReduccionGlobal[tamanioDeFRG] = '\0';
		recv(FDMaster, mensajeFinReduccionGlobal, tamanioDeFRG, 0);

		finRG = deserializarFinTransformacion(mensajeFinReduccionGlobal);

		logInfo(
				"Actualizo la tabla de estados por finalizacion de reduccion global");
		actualizarTablaDeEstadosFinReduccionGlobal(finRG->numeroDeJob,
				FDMaster);

		logInfo("Armo respuesta almacenado final");
		RAF = RespuestaAlmacenadoFinal(finRG, FDMaster);

		logInfo("Serializo respuesta almacenado final");
		tamanioDeLarespuestaAlmacenadoFinal = sizeof(int) * 5
				+ strlen(RAF->archivoDeReduccionGlobal) + strlen(RAF->ipWorker);
		almacenadoFinalSerializado = serializarRespuestaAlmacenadoFinal(RAF);

		logInfo("Envio respuesta almacenado final a master");
		mensajesEnviadosAMaster(SOL_ALMACENADO_FINAL, FDMaster,
				almacenadoFinalSerializado,
				tamanioDeLarespuestaAlmacenadoFinal);
		;

		logInfo("Actualizo la tabla de estados por el almacenado final");
		crearEntradasAlmacenamientoFinal(RAF, finRG, FDMaster);

		break;
	case FINALIZACION_DE_JOB:
		//aca entra por la finalizacion normal o un error
		logInfo("Yama recibe señal de finalización de JOB ");
		recv(FDMaster, pesoFJ, 4, 0);
		tamanioDeLaFinalizacionDeJob = deserializarINT(pesoFJ);
		logInfo("tamanio de lo que recibo %i", tamanioDeLaFinalizacionDeJob);
		mesnajeFinalizacionDeJob = malloc(tamanioDeLaFinalizacionDeJob + 1);
		mesnajeFinalizacionDeJob[tamanioDeLaFinalizacionDeJob] = '\0';
		recv(FDMaster, mesnajeFinalizacionDeJob, tamanioDeLaFinalizacionDeJob,
				0);

		jobQueFinalizo = deserializarINT(mesnajeFinalizacionDeJob);

		logInfo("Actualizar estructuras por finalizacion de job ");
		actualizarEstructurasFinalizacionDeJOB(jobQueFinalizo, FDMaster);

		break;
	case ABORTO_JOB:
		//aca entra por la finalizacion normal o un error
		logInfo("Yama recibe señal de finalización de JOB ");
		recv(FDMaster, pesoFJ, 4, 0);
		tamanioDeLaFinalizacionDeJob = deserializarINT(pesoFJ);
		logInfo("tamanio de lo que recibo %i", tamanioDeLaFinalizacionDeJob);
		mesnajeFinalizacionDeJob = malloc(tamanioDeLaFinalizacionDeJob + 1);
		mesnajeFinalizacionDeJob[tamanioDeLaFinalizacionDeJob] = '\0';
		recv(FDMaster, mesnajeFinalizacionDeJob, tamanioDeLaFinalizacionDeJob,
				0);

		jobQueFinalizo = deserializarINT(mesnajeFinalizacionDeJob);

		logInfo("ERROR: SE ABORTO EL JOB %i", jobQueFinalizo);

		//actualizarEstructuras

		break;

	default:
		logInfo("YAMA recibe un señal que no sabe traducir.");
		break;
	}
}

RespuestaReduccionLocal* respuestaReduccionLocal(finTransformacion* fin,
		int master) {

	int i = 0;
	RespuestaReduccionLocal* RRL;
	t_list* listaArchivosTemporalesTrans;
	char* ArchivoreduccionLocal;

	while (i < list_size(tabla_estados)) {
		t_reg* registro = list_get(tabla_estados, i);

		if (registro->job == fin->numeroDeJob && registro->nodo == fin->nodo
				&& registro->master == master) {

			listaArchivosTemporalesTrans =
					listaDeArchivosTemporalesTransformacion(registro->job,
							registro->master, registro->nodo);
			VariableReduccionLocal++;
			ArchivoreduccionLocal = generarNombreArchivoReduccionLocal(
					VariableReduccionLocal);
			Info_Workers* worker = list_get(list_info_workers,
					((fin->nodo) - 1));
			RRL = crearRespuestaReduccionLocal(registro->nodo, worker->puerto,
					worker->ipWorker, listaArchivosTemporalesTrans,
					ArchivoreduccionLocal);

			return RRL;
		} else {

			i++;
		}

	}

	return NULL;
}

t_list* listaDeArchivosTemporalesTransformacion(int job, int master, int nodo) {

	t_list* listaArchvosTemporales = list_create();
	int i = 0;
	char* transforamcion = "TRANSFORMACION";

	while (i < list_size(tabla_estados)) {

		t_reg* registro = list_get(tabla_estados, i);

		if (registro->job == job && registro->master == master
				&& registro->etapa == transforamcion
				&& registro->nodo == nodo) {

			list_add(listaArchvosTemporales, registro->arch_temp);

			i++;
		} else {

			i++;
		}
	}

	return listaArchvosTemporales;
}

//devuelve una lista de respuestas de trnasformaciones globales
t_list* respuestaReduccionGlobal(int numeroDeJob, int master) {

	int i;
	t_list* listaRRG = list_create();
	int* nodoConMenorCarga;
	char* reduccionLocal = "REDUCCION LOCAL";

	while (i < list_size(tabla_estados)) {

		t_reg* registro = list_get(tabla_estados, i);

		if (numeroDeJob == registro->job && master == registro->master
				&& registro->etapa == reduccionLocal) {

			nodoConMenorCarga = nodoConMenorCargaDeTrabajoParaReduccionGlobal(
					numeroDeJob, master);

			if (registro->nodo == nodoConMenorCarga) {

				Info_Workers* info = list_get(ipYPuertoWoerkers,
						((registro->nodo) - 1));
				variableReduciionGlobal++;
				char* archivoRRG = generarNombreArchivoReduccionGlobal(
						variableReduciionGlobal);

				RespuestaReduccionGlobal* RRGEncargado =
						crearRespuestaReduccionGlobal(registro->nodo,
								info->puerto, info->ipWorker,
								registro->arch_temp, archivoRRG, true);

				list_add(listaRRG, RRGEncargado);
			} else {

				Info_Workers* info = list_get(list_info_workers,
						((registro->nodo) - 1));
				variableReduciionGlobal++;
				char* archivoRRGlobal = generarNombreArchivoReduccionGlobal(
						variableReduciionGlobal);

				RespuestaReduccionGlobal* RRGNOEncargado =
						crearRespuestaReduccionGlobal(registro->nodo,
								info->puerto, info->ipWorker,
								registro->arch_temp, archivoRRGlobal, false);

				list_add(listaRRG, RRGNOEncargado);

			}

			i++;
		} else {

			i++;
		}

	}

	return listaRRG;

}

int* nodoConMenorCargaDeTrabajoParaReduccionGlobal(int master, int job) {

	t_list* nodosIntervinientes = list_create();
	int i = 0;
	int a = 0;
	int* nodoConMenorCarga;
	int carga1;
	int carga2;
	int* nodo1;
	int* nodo2;
	char* reduccionLocal = "REDUCCION LOCAL";

	while (i < list_size(tabla_estados)) {

		t_reg* registroNodo = list_get(tabla_estados, i);
		if (registroNodo->job == job && registroNodo->master == master
				&& registroNodo->etapa == reduccionLocal) {

			list_add(nodosIntervinientes, &registroNodo->nodo);
			i++;

		} else {

			i++;
		}
	}

	while (a < (list_size(nodosIntervinientes) - 1)) {

		nodo1 = list_get(nodosIntervinientes, a);
		nodo2 = list_get(nodosIntervinientes, (a + 1));

		carga1 = cargaDeTrabajoDelNodo(nodo1);
		carga2 = cargaDeTrabajoDelNodo(nodo2);

		if (carga1 <= carga2) {

			nodoConMenorCarga = list_get(nodosIntervinientes, a);
			a++;
		} else {

			nodoConMenorCarga = list_get(nodosIntervinientes, (a + 1));
			a++;
		}

	}

	list_destroy_and_destroy_elements(nodosIntervinientes, free);

	return nodoConMenorCarga;
}

void asignarNodoEncargadoAJobCompleto(t_list* RRG, int master, int job) {

	int i = 0;
	while (i < list_size(RRG)) {

		RespuestaReduccionGlobal* rrg = list_get(RRG, i);
		if (rrg->encargado == true) {

			elNodoEncargadoEs(rrg->nodo, master, job);
			i++;
		} else {

			i++;
		}
	}
}

void elNodoEncargadoEs(int nodo, int master, int job) {

	int i = 0;
	while (i < list_size(listaDeJobs)) {

		JOBCompleto * jc = list_get(listaDeJobs, i);
		if (jc->job->identificadorJob == job && jc->job->master) {
			JOBCompleto * jcModificar = list_remove(listaDeJobs, i);
			jcModificar->nodoEncargado = nodo;
			list_add_in_index(listaDeJobs, i, jcModificar);
		}
		i++;
	}
}

int cargaDeTrabajoDelNodo(int* nodo) {

	int i = 0;
	while (i < list_size(listaDeWorkerTotales)) {
		nodoParaPlanificar* n = list_get(listaDeWorkerTotales, i);
		if (n->nodo == nodo) {

			return n->carga;
		} else {

			i++;
		}
	}
	return -1;
}

respuestaAlmacenadoFinal* RespuestaAlmacenadoFinal(finTransformacion* finRG,
		int master) {

	int i = 0;
	respuestaAlmacenadoFinal* respuestaAF;
	char* reduccionGlobal = "REDUCCION GLOBAL";

	while (i < list_size(tabla_estados)) {
		t_reg* registroAAnalizar = list_get(tabla_estados, i);

		if (registroAAnalizar->job == finRG->numeroDeJob
				&& registroAAnalizar->etapa == reduccionGlobal
				&& registroAAnalizar->master == master
				&& registroAAnalizar->nodo == finRG->nodo) {

			Info_Workers* info = list_get(list_info_workers, finRG->nodo);

			respuestaAF = crearRespuestaAlmacenadoFinal(registroAAnalizar->nodo,
					info->puerto, info->ipWorker, registroAAnalizar->arch_temp);

			return respuestaAF;
		}

		i++;
	}

	return NULL;
}

//Replanificacion nueva
t_list* replanificacionNueva(Replanificacion* repla, int master) {

	JOBCompleto* jobQueReplanifica;
	JOBCompleto* jobAComparar;
	t_list* respuestaReplanificada;
	int i = 0;
	t_list* ReplanificacionVieja;

	while (i < list_size(listaDeJobs)) {
		JOBCompleto* jobAComparar = list_get(listaDeJobs, i);
		if (jobAComparar->job->identificadorJob == repla->numeroDeJOb
				&& jobAComparar->job->master == master) {
			jobQueReplanifica = list_remove(listaDeJobs, i);
			i = 50; //para que salga del whilw
		} else {

			i++;
		}
	}

	respuestaReplanificada = crearNuevaPlanificacionNueva(
			jobQueReplanifica->ubicacionDeLasPartesDelJOB, repla->nodoCaido);
	ReplanificacionVieja = jobQueReplanifica->respuestaTransformacion;
	//destruirRespuestaTransformacionYama(ReplanificacionVieja);
	jobQueReplanifica->respuestaTransformacion = respuestaReplanificada;
	list_add(listaDeJobs, jobQueReplanifica);

	return respuestaReplanificada;
}

t_list* crearNuevaPlanificacionNueva(t_list* ubicacionDeLasPartes,
		int nodoCaido) {

	int parte = 0;
	t_list* partesDelArchivo = list_create(); // me dice las partes que tiene el archivo
	int cantidadDePartesDelArchivo = list_size(ubicacionDeLasPartes);

	while (cantidadDePartesDelArchivo > 0) {

		list_add(partesDelArchivo, parte);
		cantidadDePartesDelArchivo--;
		parte++;
	}

	//Me actualiza la lista de workers totales con las partes que tiene para planificar pero no me crea el nodo caido
	actualizarListaDeWorkersTotalesParaReplanificacion(ubicacionDeLasPartes,
			nodoCaido, parametrosFileSystem->disponibilidadBase);

	if (string_equals_ignore_case(parametrosFileSystem->algoritmo, "W-CLOCK")) {

		asignarFuncionDeDisponibilidadW_ClockANodos(
				parametrosFileSystem->disponibilidadBase);

	} else {

		asignarFuncionDeDisponibilidadClockANodos(
				parametrosFileSystem->disponibilidadBase);

	}

	t_list* nodosFinalesAReplanificar = dev_nodos_a_planificar();

	algoritmoPrincipal(nodosFinalesAReplanificar, partesDelArchivo,
			parametrosFileSystem->disponibilidadBase);

	actualizarCargaDeTrabajoDeWorkersPLanificados(nodosFinalesAReplanificar);

	t_list* respuestaReplanificacion = armarRespuestaTransformacionYAMA(
			nodosFinalesAReplanificar, ubicacionDeLasPartes);

	logInfo(
			"devolver nodos planificados a la estructura de listaDeWorkersTotales y limpiar las listas internas");

	int j = 0;
	while (j < list_size(nodosFinalesAReplanificar)) {
		nodoParaPlanificar* nodofin = list_remove(nodosFinalesAReplanificar, j);
		list_clean(nodofin->partesAplanificar);
		list_clean(nodofin->partesDelArchivo);
		list_add_in_index(nodosFinalesAReplanificar, j, nodofin);
		j++;
	}

	list_add_all(listaDeWorkerTotales, nodosFinalesAReplanificar);

	return respuestaReplanificacion;
}

void actualizarListaDeWorkersTotalesParaReplanificacion(
		t_list* ubicacionDeLasPartes, int nodoCaido, int disponibilidadBase) {

	int nodo1;
	int nodo2;
	int a = 0;
	while (a < list_size(ubicacionDeLasPartes)) {

		UbicacionBloquesArchivo2* bloque = list_get(ubicacionDeLasPartes, a);
		nodo1 = bloque->nodo1;
		nodo2 = bloque->nodo2;
		if (!estaNodorEnLaListaDeTotales(nodo1)) {

			if (nodo1 != nodoCaido) {

				nodoParaPlanificar* nodoA = crearNodoParaPlanificar(
						bloque->nodo1, disponibilidadBase, 0,
						bloque->parteDelArchivo);
				logInfo("Se va agregar el nodo %i", nodo1);
				logInfo("Tamanio Lista de workersTotales antes de add %i",
						list_size(listaDeWorkerTotales));

				list_add(listaDeWorkerTotales, nodoA);
				logInfo("Tamanio Lista de workersTotales %i",
						list_size(listaDeWorkerTotales));
			}
		} else {
			//si ya esta el nodo en la lista, agre ga una parte de archivo
			agregarPartedeArchivoANodo(nodo1, bloque->parteDelArchivo);
		}
		if (!estaNodorEnLaListaDeTotales(nodo2)) {
			if (nodo2 != nodoCaido) {
				nodoParaPlanificar* nodoB = crearNodoParaPlanificar(
						bloque->nodo2, disponibilidadBase, 0,
						bloque->parteDelArchivo);
				logInfo("Se va agregar el nodo %i", nodo2);

				logInfo("Tamanio Lista de workersTotales antes de add %i",
						list_size(listaDeWorkerTotales));
				list_add(listaDeWorkerTotales, nodoB);
				logInfo("Tamanio Lista de workersTotales %i",
						list_size(listaDeWorkerTotales));
			}
		} else {
			//si ya esta el nodo en la lista, agrega una parte de archivo
			agregarPartedeArchivoANodo(nodo2, bloque->parteDelArchivo);
		}

		a++;
	}

}

//Esto va en serializacion.c

Replanificacion* deserializarReplanificacion(char* replanifSerializado) {

	Replanificacion* replanif = malloc(sizeof(int) * 2);
	int desplazamiento = 0;
	deserializarDato(&(replanif->numeroDeJOb), replanifSerializado, sizeof(int),
			&desplazamiento);
	deserializarDato(&(replanif->nodoCaido), replanifSerializado, sizeof(int),
			&desplazamiento);

	return replanif;

}

void actualizarCargaWorkerReduccionGlobal(t_list* RRG) {

	int nodoEncargado = nodoEncargadoDeLaReduccionGlobal(RRG);
	if (nodoEncargado != -1) {
		int cantidadDeArchivosAEfectuarRG = list_size(RRG);
		int cantidadDeTrabajoAAumentar = (cantidadDeArchivosAEfectuarRG / 2);

		actualizarCargaANodo(nodoEncargado, cantidadDeTrabajoAAumentar);
	} else {

		logInfo("Error de encontrar el nodo para actualizar carga");
	}
}

int nodoEncargadoDeLaReduccionGlobal(t_list* RRG) {

	int i = 0;
	while (i < list_size(RRG)) {
		RespuestaReduccionGlobal* respuestaRG = list_get(RRG, i);
		if (respuestaRG->encargado == true) {

			return respuestaRG->nodo;
		} else {

			i++;
		}
	}

	return -1;
}

int actualizarCargaANodo(int nodo, int cantidadDeCarga) {

	int i = 0;

	while (i < list_size(listaDeWorkerTotales)) {

		nodoParaPlanificar* nodo = list_get(listaDeWorkerTotales, i);
		if (nodo->nodo == nodo) {

			nodoParaPlanificar* nodo = list_remove(listaDeWorkerTotales, i);
			nodo->carga += cantidadDeCarga;
			list_add_in_index(listaDeWorkerTotales, i, nodo);
			return 0;
		} else {
			i++;
		}
	}

	return -1;
}

void actualizarEstructurasFinalizacionDeJOB(int numeroDeJob, int master) {

	actualizarAlmacenadoFinalOK(numeroDeJob, master);

	actualizarCargaDeTrabajoFinalizacionDeJOBYEliminarJOB(numeroDeJob, master);
}

void actualizarCargaDeTrabajoFinalizacionDeJOBYEliminarJOB(int numeroDeJob,
		int master) {

	int i = 0;

	while (i < list_size(listaDeJobs)) {
		JOBCompleto* jc = list_get(listaDeJobs, i);
		if (jc->job->identificadorJob == numeroDeJob
				&& jc->job->master == master) {

			actualizarCargaDeTrabajoWorkerPorFinalizacion(jc);
			JOBCompleto* jc = list_remove(listaDeJobs, i);

			//destruirJobCompleto(jc);
		} else {

			i++;
		}
	}
}

void actualizarCargaDeTrabajoWorkerPorFinalizacion(JOBCompleto* jc) {

	int i = 0;
	while (i < list_size(listaDeWorkerTotales)) {

		nodoParaPlanificar* nodo = list_get(listaDeWorkerTotales, i);
		if (estaNodoEnLaRespuestaDeTransformacion(nodo->nodo,
				jc->respuestaTransformacion)) {

			if (nodo->nodo == jc->nodoEncargado) {

				int cantidadAReducirPorSerEncargado =
						cantidadDeNodosEnLaRespuestaTransformacion(
								jc->respuestaTransformacion);
				int cantidadAReducir =
						cantidadDeTrasformacionesYReduccionesQueHace(nodo->nodo,
								jc->respuestaTransformacion);
				reducirCargaANodo(nodo->nodo,
						(cantidadAReducir + cantidadAReducirPorSerEncargado));

			} else {

				int cantidadAReducir =
						cantidadDeTrasformacionesYReduccionesQueHace(nodo->nodo,
								jc->respuestaTransformacion);
				reducirCargaANodo(nodo->nodo, cantidadAReducir);

			}
			i++;
		} else {
			i++;
		}
	}
}

bool estaNodoEnLaRespuestaDeTransformacion(int nodo,
		t_list* respuestaTransformacion) {
	int i = 0;
	while (i < list_size(respuestaTransformacion)) {
		RespuestaTransformacionYAMA* rty = list_get(respuestaTransformacion, i);
		if (rty->nodo == nodo) {

			return true;
		} else {
			i++;
		}
	}
	return false;
}

int cantidadDeTrasformacionesYReduccionesQueHace(int nodo,
		t_list* respuestaTransformacionYama) {
	int i = 0;
	int cantidad = 0;
	while (i < list_size(respuestaTransformacionYama)) {
		RespuestaTransformacionYAMA* rty = list_get(respuestaTransformacionYama,
				i);
		if (rty->nodo == nodo) {

			cantidad += 1;
			i++;
		} else {
			i++;
		}
	}

	return cantidad;
}

int reducirCargaANodo(int nodo, int cantidadDeCarga) {

	int i = 0;

	while (i < list_size(listaDeWorkerTotales)) {

		nodoParaPlanificar* nodo = list_get(listaDeWorkerTotales, i);
		if (nodo->nodo == nodo) {

			nodoParaPlanificar* nodo = list_remove(listaDeWorkerTotales, i);
			nodo->carga -= cantidadDeCarga;
			list_add_in_index(listaDeWorkerTotales, i, nodo);
			return 0;
		} else {
			i++;
		}
	}

	return -1;
}

int cantidadDeNodosEnLaRespuestaTransformacion(
		t_list* respuestaTransformacionYama) {

	t_list* nodos = list_create();
	int i = 0;
	int cantidadDeNodos;
	while (i < list_size(respuestaTransformacionYama)) {
		RespuestaTransformacionYAMA* rty = list_get(respuestaTransformacionYama,
				i);

		if (estaNodoEnlista(nodos, rty->nodo)) {

			i++;
		} else {

			list_add(nodos, rty->nodo);
			i++;
		}
	}
	cantidadDeNodos = list_size(nodos);
	list_destroy_and_destroy_elements(nodos, free);
	return cantidadDeNodos;

}

bool estaNodoEnlista(t_list* lista, int nodo) {

	if (list_is_empty(lista)) {

		return false;
	}
	int i = 0;
	while (i < list_size(lista)) {
		int a = list_get(lista, i);

		if (a == nodo) {

			return true;
		} else {

			i++;
		}
	}
	return false;

}

void darDeBajaUnNodoCaido(int nodoCaido) {

	int i;
	while (i < list_size(listaDeWorkerTotales)) {

		nodoParaPlanificar* nodo = list_get(listaDeWorkerTotales, i);

		if (nodo->nodo == nodoCaido) {

			nodoParaPlanificar* nodoASacar = list_remove(listaDeWorkerTotales,
					i);
			list_add(nodosCaidos, nodoASacar);
			i++;
		} else {
			i++;
		}
	}

}

//Replanificacion

void actualizarCargaDeWorkersPorReplanificacion(int numeroDeNodo,
		int numeroDeJOb, int master) {

	int i = 0;
	while (i < list_size(listaDeJobs)) {
		JOBCompleto* jobAComparar = list_get(listaDeJobs, i);
		if (jobAComparar->job->identificadorJob == numeroDeJOb, jobAComparar->job->master =
				master) {

			JOBCompleto* jobElegido = list_remove(listaDeJobs, i);
			disminuirCargaDeWorkersPorReplanificacion(
					jobElegido->respuestaTransformacion);
			list_add_in_index(listaDeJobs, i, jobElegido);
			i++;
		} else {

			i++;
		}
	}
}

void disminuirCargaDeWorkersPorReplanificacion(t_list* listaDeRespuestas) {

	int i = 0;
	while (i < list_size(listaDeRespuestas)) {
		RespuestaTransformacionYAMA* respuesta = list_get(listaDeRespuestas, i);
		reducirCargaANodo(respuesta->nodo, 1);
		i++;
	}
}
