#include "Headers/ComunicacionConWorker.h"


void comunicacionWorkers(ParametrosComunicacionWoker* parametros) {
	int FDServidorWORKER;
	FDServidorWORKER = lib_SocketCliente(parametros->ipWoker,parametros->puertoWoker);
	//logInfo("%i",FDServidorWORKER);

	/*script* scriptReductor = setScript("/home/utnso/tp-2017-2c-s1st3m4s_0p3r4t1v0s/Master/reductor.py");
	int tamanioScriptReductor = tamanioScript(scriptReductor);
		script* scriptTransformador = setScript("/home/utnso/tp-2017-2c-s1st3m4s_0p3r4t1v0s/Master/transformador.py");
	int tamanioScriptTransformador = tamanioScript(scriptTransformador);
	script* scriptTransformadorIniciales = setScript("/home/utnso/tp-2017-2c-s1st3m4s_0p3r4t1v0s/Master/transformador.py");
	int tamanioScriptTransformadorIniciales = tamanioScript(scriptTransformadorIniciales);
	script* scriptTransformadorAnuales = setScript("/home/utnso/tp-2017-2c-s1st3m4s_0p3r4t1v0s/Master/transformador.py");
	int tamanioScriptTransformadorAnuales = tamanioScript(scriptTransformadorAnuales);

	char* respuestaScriptReductor = serializarScript(scriptReductor);
	char* respuestaScriptTransformador = serializarScript(scriptTransformador);
	char* respuestaScriptIniciales = serializarScript(scriptTransformadorIniciales);
	char* respuestaScriptAnuales = serializarScript(scriptTransformadorAnuales);


	mensajesEnviadosAWorker(SCRIPT_REDUCCION,FDServidorWORKER,respuestaScriptReductor,tamanioScriptReductor);
	logInfo("Se envia script reductor");

	mensajesEnviadosAWorker(SCRIPT_TRANSFORMADOR,FDServidorWORKER,respuestaScriptTransformador, tamanioScriptTransformador);
	logInfo("Se envia script transformador ");

	mensajesEnviadosAWorker(SCRIPT_TRANSFORMADOR_INICIAL,FDServidorWORKER,respuestaScriptIniciales, tamanioScriptTransformadorIniciales);
	logInfo("Se envia script scriptTransformadorIniciales");

	mensajesEnviadosAWorker(SCRIPT_TRANSFORMADOR_ANUAL,FDServidorWORKER,respuestaScriptAnuales, tamanioScriptTransformadorAnuales);
	logInfo("Se envia script scriptTransformadorAnuales");
*/

	logInfo("Trato de comunicar con worker");

	infoTransformacionParaWorker* mensaje;
	int tamanio = strlen(parametros->archivoTemporal);
	mensaje= malloc(sizeof(int) * 3 + tamanio * sizeof(char));

	mensaje-> nodo = parametros->nodo;
	//logInfo("nodo: %i",mensaje->nodo);
	mensaje-> bloque = parametros->bloque;
	//logInfo("bloque: %i",mensaje->bloque);
	mensaje-> bytesOcupados = parametros->bytesOcupados;
	//logInfo("bytesOcupados: %i",mensaje->bytesOcupados);
	mensaje-> archivoTemporal = parametros->archivoTemporal;
	//logInfo("archivoTemporal: %s",mensaje->archivoTemporal);

	//logInfo("Antes de serializar info para worker");
	char* respuesta = serializarInfoParaWorker(mensaje->nodo, mensaje-> bloque, mensaje-> bytesOcupados,mensaje->archivoTemporal);
	logInfo("Despues de serializar info para worker");

	int tamanioRespuesta = sizeof(int) * 3+ strlen(mensaje->archivoTemporal);

	mensajesEnviadosAWorker(TAREA_WORKER,FDServidorWORKER,respuesta, tamanioRespuesta);

}
ParametrosComunicacionWoker* setParametrosComunicacionConWoker(int puerto, char* ip, int nodo, char* archivo, int bytesOcupados ,int bloque) {

	ParametrosComunicacionWoker* parametros = malloc(sizeof(int)*4+strlen(ip)*sizeof(char)+strlen(archivo)*sizeof(char));

	parametros->ipWoker = ip;
	parametros->puertoWoker = puerto;
	parametros->nodo = nodo;
	parametros->bytesOcupados = bytesOcupados;
	parametros -> archivoTemporal = archivo;
	parametros -> bloque = bloque;


	return parametros;
}

void mensajesEnviadosAWorker(int codigo, int FDServidorWORKER, char* mensaje,int tamanio) {
	Paquete* paqueteDeEnvio;
	switch (codigo) {
case TAREA_WORKER:
	paqueteDeEnvio = crearPaquete(TAREA_WORKER, tamanio, mensaje);

		if (enviarPaquete(FDServidorWORKER, paqueteDeEnvio) == -1) {
			logInfo("Error en envio de  INFOPARAWORKERS");
		}

		destruirPaquete(paqueteDeEnvio);
		break;
case SCRIPT_REDUCCION:
	paqueteDeEnvio = crearPaquete(SCRIPT_REDUCCION, tamanio, mensaje);

		if (enviarPaquete(FDServidorWORKER, paqueteDeEnvio) == -1) {
			logInfo("Error en envio de  INFOPARAWORKERS");
		}

		destruirPaquete(paqueteDeEnvio);
		break;
case SCRIPT_TRANSFORMADOR:
	paqueteDeEnvio = crearPaquete(SCRIPT_TRANSFORMADOR, tamanio, mensaje);

		if (enviarPaquete(FDServidorWORKER, paqueteDeEnvio) == -1) {
			logInfo("Error en envio de  INFOPARAWORKERS");
		}

		destruirPaquete(paqueteDeEnvio);
		break;
case SCRIPT_TRANSFORMADOR_INICIAL:
	paqueteDeEnvio = crearPaquete(SCRIPT_TRANSFORMADOR_INICIAL, tamanio, mensaje);

		if (enviarPaquete(FDServidorWORKER, paqueteDeEnvio) == -1) {
			logInfo("Error en envio de  INFOPARAWORKERS");
		}

		destruirPaquete(paqueteDeEnvio);
		break;
case SCRIPT_TRANSFORMADOR_ANUAL:
	paqueteDeEnvio = crearPaquete(SCRIPT_TRANSFORMADOR_ANUAL, tamanio, mensaje);

		if (enviarPaquete(FDServidorWORKER, paqueteDeEnvio) == -1) {
			logInfo("Error en envio de  INFOPARAWORKERS");
		}

		destruirPaquete(paqueteDeEnvio);
		break;

	}
}


void mensajesRecibidosDeWorker(int codigo, int FDServidorWORKER) {

//Data node recibe de File System
	char pesoMensaje[8];
	int tamanio;
	char* mensaje;
	char* buffer[4];
	int intRecibido;
	char* bufferBloque[4];

	char* mensajeNuevo;

	finTransformacion* finRG;


	switch (codigo) {
	case FIN_TRANSFORMACION: //listo

		logInfo("MASTER RECIBE EL FIN DE LA TRANSFORMACION");

				recv(FDServidorWORKER,pesoMensaje,4,0);
				tamanio= deserializarINT(pesoMensaje);
				logInfo("tamanio de lo que recibo %i", tamanio);
				mensaje = malloc(tamanio + 1);
				mensaje[tamanio] = '\0';
				recv(FDServidorWORKER,mensaje, tamanio,0);
				intRecibido = deserializarINT(mensaje);
				logInfo("Recivi de Worker el fin de la transformacion %i", intRecibido);
				mensajeNuevo = malloc (sizeof(int));
				mensajeNuevo = serializeInt(intRecibido);

				  mensajesEnviadosAYama(FIN_TRANSFORMACION, FD_YAMA,mensajeNuevo, strlen(mensajeNuevo));

		break;

	case FIN_REDUCCION_LOCAL:

		logInfo("MASTER RECIBE EL FIN DE LA REDUCCION LOCAL");

						recv(FDServidorWORKER,pesoMensaje,4,0);
						tamanio= deserializarINT(pesoMensaje);
						logInfo("tamanio de lo que recibo %i", tamanio);
						mensaje = malloc(tamanio + 1);
						mensaje[tamanio] = '\0';
						recv(FDServidorWORKER,mensaje, tamanio,0);
						intRecibido = deserializarINT(mensaje);
						logInfo("Recivi de Worker el fin de la reduccion local %i", intRecibido);


						finTransformacion* finRG = malloc(sizeof(int)*2);
						//finRG->nodo= el nro de nodo se lo manda worker. crear estructuraa
						//  finRG->numeroDeJob=nro_job;
                       // logInfo("Envio a YAMA el fin de la reduccion local");
					//	mensajesEnviadosAYama(FIN_REDUCCION_LOCAL, FD_YAMA,mensajeNuevo, strlen(mensajeNuevo));



		break;

	case FIN_REDUCCION_GLOBAL:
		logInfo("MASTER RECIBE EL FIN DE LA REDUCCION GLOBAL");

							recv(FDServidorWORKER,pesoMensaje,4,0);
							tamanio= deserializarINT(pesoMensaje);
							logInfo("tamanio de lo que recibo %i", tamanio);
							mensaje = malloc(tamanio + 1);
							mensaje[tamanio] = '\0';
							recv(FDServidorWORKER,mensaje, tamanio,0);
							intRecibido = deserializarINT(mensaje);
							logInfo("Recivi de Worker el fin de la reduccion global %i", intRecibido);
							mensajeNuevo = malloc (sizeof(int));
							mensajeNuevo = serializeInt(intRecibido);
							 logInfo("Envio a YAMA el fin de la reduccion global");
					        mensajesEnviadosAYama(FIN_REDUCCION_GLOBAL, FD_YAMA,mensajeNuevo, strlen(mensajeNuevo));





		break;

	case ALMACENADO_FINAL:
		logInfo("MASTER RECIBE EL FIN DEL ALMACENADO FINAL");

									recv(FDServidorWORKER,pesoMensaje,4,0);
									tamanio= deserializarINT(pesoMensaje);
									logInfo("tamanio de lo que recibo %i", tamanio);
									mensaje = malloc(tamanio + 1);
									mensaje[tamanio] = '\0';
									recv(FDServidorWORKER,mensaje, tamanio,0);
									intRecibido = deserializarINT(mensaje);
									logInfo("Recivi de Worker el fin deL almacenado final %i", intRecibido);
									mensajeNuevo = malloc (sizeof(int));
									mensajeNuevo = serializeInt(intRecibido);

							        mensajesEnviadosAYama(FINALIZACION_DE_JOB, FD_YAMA,mensajeNuevo, strlen(mensajeNuevo));







		break;


	default:
		logInfo("MASTER RECIBE UNA SENIAL QUE NO SABE TRADUCIR");
		break;
	}
}
