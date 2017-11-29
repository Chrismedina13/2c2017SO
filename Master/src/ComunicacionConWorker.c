#include "Headers/ComunicacionConWorker.h"
#include "SO_lib/estructuras.h"

void comunicacionWorkers(ParametrosComunicacionWoker* parametros) {
	int FDServidorWORKER;
	FDServidorWORKER = lib_SocketCliente(parametros->ipWoker,parametros->puertoWoker);

		/*printf("Se conecto un Worker su FD es el  = %d\n", FDServidorWORKER);

		logInfo("FD del Worker : %i \n", FDServidorWORKER);

		if (send(FDServidorWORKER, "Hola WORKER", 13, 0) != -1) {


				logInfo("Comunicacion con WORKER establecida, Mensaje enviado correctamente");


			} else {

				logInfo("Error en la comunicacion con WORKER ");

			}*/
	/* //Se serializaria la lista serializarRespuestaTransformacionYAMA()solo los 4 parametros; */

	infoParaWorker* mensaje;

	mensaje -> nodo = parametros ->nodo;
	mensaje -> bloque = parametros->bloque;
	mensaje -> bytesOcupados = parametros->bytesOcupados;
	mensaje -> archivoTemporal = parametros ->archivoTemporal;

	char* respuesta = serializarInfoParaWorker(mensaje);

	int tamanioRespuesta = (sizeof(int) * 3+ sizeof(char*));

	mensajesEnviadosAWorker(TAREA_WORKER,FDServidorWORKER,respuesta, tamanioRespuesta);

}
ParametrosComunicacionWoker* setParametrosComunicacionConWoker(int puerto, char* ip, int nodo, char* archivo, int bytesOcupados ,int bloque) {

	ParametrosComunicacionWoker* parametros = malloc(
			sizeof(ParametrosComunicacionWoker));

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
			logInfo("Error en envio de  INfoParaWorkers");
		}

		destruirPaquete(paqueteDeEnvio);
		break;
	}
}
