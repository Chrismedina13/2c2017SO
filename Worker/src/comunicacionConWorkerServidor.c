/*
 * comunicacionConWorker.c
 *
 *  Created on: 11/12/2017
 *      Author: utnso
 */
#include "Headers/comunicacionConWorkerServidor.h"

archivo* comunicacionConWorkerServidor(infoParaReduccionGlobal* param,
		char* rutaAGuardar) {

	archivo* archivoRecibido;

	int FDsocketClienteWorkerServidor;
	FDsocketClienteWorkerServidor = lib_SocketCliente(param->ipWorker,
			param->puerto);
	logInfo("SocketCliente = %d \n", FDsocketClienteWorkerServidor);

	FD_WorkerServidor = FDsocketClienteWorkerServidor;

	char* mensaje = param->archivoTemporalReduccionLocal;
	int tamanio = strlen(mensaje);

	//ENVIO LA SOLICITUD DEL ARCHIVO TEMPORAL AL WORKER SERVIDOR
	mensajesEnviadosAWorkerServidor(SOLICITUD_ARCHIVO_TEMPORAL,
			FD_WorkerServidor, mensaje, tamanio);

	//RECIBO EL ARCHIVO TEMPORAL, LO DESERIALIZO Y LO GUARDO EN /WORKER/TMP
	mensajesRecibidosDeWorkerServidor(SOLICITUD_ARCHIVO_TEMPORAL,
			FD_WorkerServidor, rutaAGuardar);

	return archivoRecibido;
}

ParametrosComunicacionConWorkerServidor* setParametrosComunicacionConWorkerServidor(
		int puerto, char* ip) {
	ParametrosComunicacionConWorkerServidor* parametros = malloc(
			sizeof(ParametrosComunicacionConWorkerServidor));
	parametros->ip = ip;
	parametros->puerto = puerto;
	return parametros;
}

void mensajesEnviadosAWorkerServidor(int codigo, int FD_FileSystem,
		char* mensaje, int tamanio) {
	Paquete * paqueteEnvio;
	switch (codigo) {

	case SOLICITUD_ARCHIVO_TEMPORAL:
		logInfo("se manda a file system el saludo");
		paqueteEnvio = crearPaquete(ALMACENADO_FINAL, tamanio, mensaje);
		if (enviarPaquete(FD_FileSystem, paqueteEnvio) == -1) {
			logInfo("ERROR EN EL ENVIO DE SALUDO");
		}

		destruirPaquete(paqueteEnvio);
		free(mensaje);
		break;

	default:
		break;
	}
}

void mensajesRecibidosDeWorkerServidor(int codigo, int FDServidorWORKER,
		char* rutaAGuardar) {

	char* pesoMensaje;
	int tamanio;
	char* mensaje;

	switch(codigo){
	case SOLICITUD_ARCHIVO_TEMPORAL:
		logInfo("WORKER CLIENTE RECIBE ARCHIVO TEMPORAL DEL WORKER SERVIDOR");

		recv(FDServidorWORKER, pesoMensaje, sizeof(int), 0);
		tamanio = deserializarINT(pesoMensaje);
		logInfo("tamanio de lo que recibo %i", tamanio);

		mensaje = malloc(tamanio);
		recv(FDServidorWORKER, mensaje, tamanio, 0);
		archivo* archivoTemporal = deserializarArchivo(mensaje);

		//VUELVO A CREAR EL ARCHIVO EN EL LUGAR EN /WORKER/TMP/
		char* nombre = string_new();
		string_append(nombre,"/home/utnso/tp-2017-2c-s1st3m4s_0p3r4t1v0s/Worker/tmp/");
		string_append(nombre,archivoTemporal->nombre);
		crearArchivo(archivoTemporal->contenido,nombre);
		break;
	default:
		break;
	}
}
