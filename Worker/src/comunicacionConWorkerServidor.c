/*
 * comunicacionConWorker.c
 *
 *  Created on: 11/12/2017
 *      Author: utnso
 */
#include "Headers/comunicacionConWorkerServidor.h"

void comunicacionConWorkerServidor(ParametrosComunicacionConWorkerServidor* param) {
	//socketClienteParaFileSystem
	char buffer2[4];

	int FDsocketClienteWorkerServidor;
	FDsocketClienteWorkerServidor = lib_SocketCliente(param->ip,param->puerto);
	logInfo("SocketCliente = %d \n", FDsocketClienteWorkerServidor);

	FD_WorkerServidor=FDsocketClienteWorkerServidor;
}

ParametrosComunicacionConWorkerServidor* setParametrosComunicacionConWorkerServidor(int puerto,char* ip){
	ParametrosComunicacionConWorkerServidor* parametros = malloc(sizeof(ParametrosComunicacionConWorkerServidor));
	parametros->ip = ip;
	parametros->puerto = puerto;
	return parametros;
}

void mensajesEnviadosAWorkerServidor(int codigo, int FD_FileSystem, char* mensaje, int tamanio) {
	Paquete * paqueteEnvio;
	switch (codigo) {

	case ALMACENADO_FINAL:
      logInfo("se manda a file system el saludo");
		paqueteEnvio = crearPaquete(ALMACENADO_FINAL, tamanio,mensaje);
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

