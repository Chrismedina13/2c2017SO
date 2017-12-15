#include "Headers/ComunicacionConWorker.h"

void comunicacionWorkers(ParametrosComunicacionWoker* parametros) {
	int FDServidorWORKER;
	FDServidorWORKER = lib_SocketCliente(parametros->ipWoker,
			parametros->puertoWoker);
	logInfo("FDServidorWORKER %i", FDServidorWORKER);

	//busco el script transformador
	char*contenido = obtenerPuntero(ruta_transformador);
	char * ruta = obtenerSoloNombreDelArchivo(ruta_transformador);
	script* transformador = malloc(strlen(contenido) + strlen(ruta));

	transformador->contenido = contenido;
	transformador->nombre = ruta;
	//logInfo("Transformador Nombre %s", transformador->nombre);
	//logInfo("Transformador Contenido %s", transformador->contenido);

	char * scriptSerializado = serializarScript(transformador);
	int tamanioTransf = (strlen(transformador->nombre)
			+ strlen(transformador->contenido) + sizeof(int) * 2);
	mensajesEnviadosAWorker(SCRIPT_TRANSFORMADOR, FDServidorWORKER,
			scriptSerializado, tamanioTransf);

	int i = 0;

	while (i < list_size(parametros->tareas)) {

		tareaTransformacion* tarea = list_get(parametros->tareas, i);

		infoTransformacionParaWorker* mensaje;
		int tamanio = strlen(tarea->archivoTemporal);
		mensaje = malloc(sizeof(int) * 4 + tamanio * sizeof(char));

		mensaje->nodo = parametros->nodo;
		mensaje->bloque = tarea->bloque;
		mensaje->bytesOcupados = tarea->bytesOcupados;
		mensaje->archivoTemporal = tarea->archivoTemporal;
		logInfo("archivoTemporal QUE QUIERO VER: %s", tarea->archivoTemporal);

		//logInfo("Antes de serializar info para worker");
		char* respuesta = serializarInfoParaWorker(mensaje->nodo,
				mensaje->bloque, mensaje->bytesOcupados,
				mensaje->archivoTemporal);
		logInfo("Despues de serializar info para worker");

		int tamanioRespuesta = sizeof(int) * 4
				+ strlen(mensaje->archivoTemporal);

		mensajesEnviadosAWorker(SOL_TRANSFORMACION, FDServidorWORKER, respuesta,
				tamanioRespuesta);

		while (1) {
			char buffer[4];

			// (recv(FDServidorWORKER, buffer, 4, 0) < 0) {

			//	logInfo("Se desconecto el nodo");

			//} else {

			recv(FDServidorWORKER, buffer, 4, 0);
			int codigo = deserializarINT(buffer);
			if (codigo > 0 && codigo < 50) {

				logInfo("Codigo que recibo  Master de Worker%i", codigo);
				mensajesRecibidosDeWorker(codigo, FDServidorWORKER);
				//}
//

			}
		}
	}
}
ParametrosComunicacionWoker* setParametrosComunicacionConWoker(int puerto,
		char* ip, int nodo, t_list* listaTareas) {

	int tamanioLista = tamanioDeListaDeTareas(listaTareas);
	ParametrosComunicacionWoker* parametros = malloc(
			sizeof(int) * 3 + tamanioLista);

	parametros->ipWoker = ip;
	parametros->puertoWoker = puerto;
	parametros->nodo = nodo;
	parametros->tareas = listaTareas;

	return parametros;
}

void mensajesEnviadosAWorker(int codigo, int FDServidorWORKER, char* mensaje,
		int tamanio) {
	Paquete* paqueteDeEnvio;
	Paquete* paqueteDeEnvioRedu;
	Paquete* paqueteDeEnvioTransf;
	Paquete* paqueteDeEnvioTI;
	Paquete* paqueteDeEnvioTA;

	switch (codigo) {
	case SOL_TRANSFORMACION:
		paqueteDeEnvio = crearPaquete(SOL_TRANSFORMACION, tamanio, mensaje);

		if (enviarPaquete(FDServidorWORKER, paqueteDeEnvio) == -1) {
			logInfo("Error en envio de  INFOPARAWORKERS");
		} else {
			logInfo("Se envio INFOPARAWORKERS exitosamente.");
		}

		destruirPaquete(paqueteDeEnvio);
		break;
	case SCRIPT_TRANSFORMADOR:
		paqueteDeEnvioTransf = crearPaquete(SCRIPT_TRANSFORMADOR, tamanio,
				mensaje);

		if (enviarPaquete(FDServidorWORKER, paqueteDeEnvioTransf) == -1) {
			logInfo("Error en envio de  SCRIPT_TRANSFORMADOR");
		} else {
			logInfo("Se envio SCRIPT_TRANSFORMADOR exitosamente.");
		}

		destruirPaquete(paqueteDeEnvioTransf);
		break;
	case SCRIPT_REDUCCION:
		paqueteDeEnvioRedu = crearPaquete(SCRIPT_REDUCCION, tamanio, mensaje);

		if (enviarPaquete(FDServidorWORKER, paqueteDeEnvioRedu) == -1) {
			logInfo("Error en envio de  SCRIPT_REDUCCION");
		}

		destruirPaquete(paqueteDeEnvioRedu);
		break;

	case SCRIPT_TRANSFORMADOR_INICIAL:
		paqueteDeEnvioTI = crearPaquete(SCRIPT_TRANSFORMADOR_INICIAL, tamanio,
				mensaje);

		if (enviarPaquete(FDServidorWORKER, paqueteDeEnvioTI) == -1) {
			logInfo("Error en envio de  SCRIPT_TRANSFORMADOR_INICIAL");
		}

		destruirPaquete(paqueteDeEnvioTI);
		break;
	case SCRIPT_TRANSFORMADOR_ANUAL:
		paqueteDeEnvioTA = crearPaquete(SCRIPT_TRANSFORMADOR_ANUAL, tamanio,
				mensaje);

		if (enviarPaquete(FDServidorWORKER, paqueteDeEnvioTA) == -1) {
			logInfo("Error en envio de  SCRIPT_TRANSFORMADOR_ANUAL");
		}

		destruirPaquete(paqueteDeEnvioTA);
		break;

	}
}

void mensajesRecibidosDeWorker(int codigo, int FDServidorWORKER) {

//Data node recibe de File System
	char pesoMensaje[8];
	int tamanio;
	char* mensaje;
	int intRecibido;
	int jobQueFinalizo;
	char* mensajeNuevo;
	char* mensajeNuevo2;

	finTransformacion* finRG;
	resultadoJob * resultado_job;

	switch (codigo) {
	case FIN_TRANSFORMACION: //master recibe de worker el nro del nodo y el resultado
		//si result==1, manda a YAMA replanificacion
		//si result==0, manda a YAMA fin transformacion

		logInfo("MASTER RECIBE EL FIN DE LA TRANSFORMACION");

		recv(FDServidorWORKER, pesoMensaje, 4, 0);
		tamanio = deserializarINT(pesoMensaje);
		logInfo("Tamanio de lo que recibo %i", tamanio);
		mensaje = malloc(tamanio + 1);
		mensaje[tamanio] = '\0';
		recv(FDServidorWORKER, mensaje, tamanio, 0);
		resultado_job = deserializarResultado(mensaje);
		logInfo("Resultado %i,Nodo %i", resultado_job->resultado,
				resultado_job->nodo);

		if (resultado_job->resultado == 0) {

			logInfo("Envio a YAMA el fin transformacion");
			finRG = malloc(sizeof(int) * 2);
			finRG->nodo = resultado_job->nodo;
			finRG->numeroDeJob = nro_job;
			mensajeNuevo = serializarFinTransformacion(finRG);
			mensajesEnviadosAYama(FIN_TRANSFORMACION, FD_YAMA, mensajeNuevo,
					sizeof(int) * 2);
		} else {
			logInfo(
					"Fin de la transformacion salio mal, Envio a YAMA la replanificacion");

			mensajeNuevo2 = serializarReplanificacion(nro_job,
					resultado_job->nodo);

			mensajesEnviadosAYama(REPLANIFICACION, FD_YAMA, mensajeNuevo2,
					sizeof(int) * 2);
		}
		break;

	case FIN_REDUCCION_LOCAL:// master recibe de worker y el nro de nodo y resultado,
		//si result ==0 , manda fin reduccion local --- FALTA SEMAFORO QUE NO LO MANDA HASTA QUE NO ESTEN TODOS LOS WORKERS TERMINADOS
		//si result ==1, manda FINALIZACION DE JOB

		logInfo("MASTER RECIBE EL FIN DE LA REDUCCION LOCAL");

		recv(FDServidorWORKER, pesoMensaje, 4, 0);
		tamanio = deserializarINT(pesoMensaje);
		logInfo("tamanio de lo que recibo %i", tamanio);
		mensaje = malloc(tamanio + 1);
		mensaje[tamanio] = '\0';
		recv(FDServidorWORKER, mensaje, tamanio, 0);
		resultado_job = deserializarResultado(mensaje);
		if (resultado_job->resultado == 0) {
			logInfo("Recibi de Worker el fin de la reduccion local ");

			//if(cantN)

			logInfo("Envio a YAMA el fin reduccion Local");
			finRG = malloc(sizeof(int) * 2);
			finRG->nodo = resultado_job->nodo;
			finRG->numeroDeJob = nro_job;
			mensajeNuevo = serializarFinTransformacion(finRG);
			mensajesEnviadosAYama(FIN_REDUCCION_LOCAL, FD_YAMA, mensajeNuevo,
					sizeof(int) * 2);

		} else {
			logInfo(
					"Fin de la reduccion local salio mal, Envio a YAMA la finalizacion del job");

			jobQueFinalizo = nro_job;
			mensajeNuevo2 = serializeInt(jobQueFinalizo);

			mensajesEnviadosAYama(ABORTO_JOB, FD_YAMA, mensajeNuevo2,
					sizeof(int));
		}

		break;

	case FIN_REDUCCION_GLOBAL:
		logInfo("MASTER RECIBE EL FIN DE LA REDUCCION GLOBAL");

		recv(FDServidorWORKER, pesoMensaje, 4, 0);
		tamanio = deserializarINT(pesoMensaje);
		logInfo("tamanio de lo que recibo %i", tamanio);
		mensaje = malloc(tamanio + 1);
		mensaje[tamanio] = '\0';
		recv(FDServidorWORKER, mensaje, tamanio, 0);
		resultado_job = deserializarResultado(mensaje);
		if (resultado_job->resultado == 0) {

			logInfo("Envio a YAMA el fin reduccion Global");
			finRG = malloc(sizeof(int) * 2);
			finRG->nodo = resultado_job->nodo;
			finRG->numeroDeJob = nro_job;
			mensajeNuevo = serializarFinTransformacion(finRG);
			mensajesEnviadosAYama(FIN_REDUCCION_GLOBAL, FD_YAMA, mensajeNuevo,
					sizeof(int) * 2);
		} else {
			logInfo(
					"Fin de la reduccion local salio mal, Envio a YAMA la finalizacion del job");

			jobQueFinalizo = nro_job;
			mensajeNuevo2 = serializeInt(jobQueFinalizo);

			mensajesEnviadosAYama(ABORTO_JOB, FD_YAMA, mensajeNuevo2,
					sizeof(int));
		}

		break;

	case ALMACENADO_FINAL: //el estado (int). 0 si esta bien, 1 si esta mal
		logInfo("MASTER RECIBE EL FIN DEL ALMACENADO FINAL");

		recv(FDServidorWORKER, pesoMensaje, 4, 0);
		tamanio = deserializarINT(pesoMensaje);
		logInfo("tamanio de lo que recibo %i", tamanio);
		mensaje = malloc(tamanio + 1);
		mensaje[tamanio] = '\0';
		recv(FDServidorWORKER, mensaje, tamanio, 0);
		intRecibido = deserializarINT(mensaje);
		logInfo("Recivi de Worker el fin deL almacenado final %i", intRecibido);
		mensajeNuevo = malloc(sizeof(int));
		mensajeNuevo = serializeInt(intRecibido);

		mensajesEnviadosAYama(FINALIZACION_DE_JOB, FD_YAMA, mensajeNuevo,
				sizeof(int));

		break;

	default:
		logInfo("MASTER RECIBE UNA SENIAL QUE NO SABE TRADUCIR");
		break;
	}
}
