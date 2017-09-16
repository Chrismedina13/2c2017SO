#include "Headers/ComunicacionConFileSystem.h"

void comunicacionConFileSystem(ParametrosComunicacionConFileSystem* param) {
	//socketClienteParaFileSystem
	int FDsocketClienteFileSystem;
	FDsocketClienteFileSystem = SocketCliente(param->ip,param->puerto);
	logInfo("SocketCliente = %d \n", FDsocketClienteFileSystem);
	char buffer[13];
	if (recv(FDsocketClienteFileSystem, buffer, 13, 0) != -1) {
		logInfo("Se recibio: %s", buffer);
	}
}

ParametrosComunicacionConFileSystem* setParametrosComunicacionConFileSystem(int puerto,char* ip){
	ParametrosComunicacionConFileSystem* parametros = malloc(sizeof(ParametrosComunicacionConFileSystem));
	parametros->ip = ip;
	parametros->puerto = puerto;
	return parametros;
}
