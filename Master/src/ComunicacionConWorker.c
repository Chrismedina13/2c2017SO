#include "Headers/ComunicacionConWorker.h"
#include "SO_lib/estructuras.h"
#include "SO_lib/FuncionesAuxiliares.h"
#include "SO_lib/Protocolo.h"

void comunicacionWorkers(ParametrosComunicacionWoker* parametros) {
	int FDServidorWORKER;
	FDServidorWORKER = lib_SocketCliente(parametros->ipWoker,parametros->puertoWoker);
	//logInfo("%i",FDServidorWORKER);
	script* scriptReductor = setScript("/home/utnso/tp-2017-2c-s1st3m4s_0p3r4t1v0s/Master/reductor.py");
	int tamanioScriptReductor = tamanioScript(scriptReductor);
		script* scriptTransformador = setScript("/home/utnso/tp-2017-2c-s1st3m4s_0p3r4t1v0s/Master/transformador.py");
	int tamanioScriptTransformador = tamanioScript(scriptTransformador);
	script* scriptTransformadorIniciales = setScript("/home/utnso/tp-2017-2c-s1st3m4s_0p3r4t1v0s/Master/transformador.py");
	int tamanioScriptTransformadorIniciales = tamanioScript(scriptTransformadorIniciales);
	script* scriptTransformadorAnuales = setScript("/home/utnso/tp-2017-2c-s1st3m4s_0p3r4t1v0s/Master/transformador.py");
	int tamanioScriptTransformadorAnuales = tamanioScript(scriptTransformadorAnuales);

	mensajesEnviadosAWorker(SCRIPT,FDServidorWORKER,scriptReductor,tamanioScriptReductor);
	logInfo("Se envia script reductor");

	mensajesEnviadosAWorker(SCRIPT,FDServidorWORKER,scriptTransformador, tamanioScriptTransformador);
	logInfo("Se envia script transformador ");

	mensajesEnviadosAWorker(SCRIPT,FDServidorWORKER,scriptTransformadorIniciales, tamanioScriptTransformadorIniciales);
	logInfo("Se envia script reductor");

	mensajesEnviadosAWorker(SCRIPT,FDServidorWORKER,scriptTransformadorAnuales, tamanioScriptTransformadorAnuales);
	logInfo("Se envia script reductor");

	logInfo("Trato de comunicar con worker");

	infoParaWorker* mensaje;
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
	}
}
