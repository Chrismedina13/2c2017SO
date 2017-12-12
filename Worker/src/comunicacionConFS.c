/*
 * comunicacionConFS.c
 *
 *  Created on: 19/9/2017
 *      Author: utnso
 */

#include "Headers/comunicacionConFileSystem.h"
#include "Headers/Worker.h"

void comunicacionConFileSystem(ParametrosComunicacionConFileSystem* param) {
	//socketClienteParaFileSystem
	char buffer2[4];



	int FDsocketClienteFileSystem;
	FDsocketClienteFileSystem = lib_SocketCliente(param->ip,param->puerto);
	logInfo("SocketCliente = %d \n", FDsocketClienteFileSystem);

	FD_Filesystem=FDsocketClienteFileSystem;
}

ParametrosComunicacionConFileSystem* setParametrosComunicacionConFileSystem(int puerto,char* ip){
	ParametrosComunicacionConFileSystem* parametros = malloc(sizeof(ParametrosComunicacionConFileSystem));
	parametros->ip = ip;
	parametros->puerto = puerto;
	return parametros;
}

void mensajesEnviadosAFileSystem(int codigo, int FD_FileSystem, char* mensaje, int tamanio) {
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


void enviarAlmacenadoFinal(char * nombre_archivo , char* contenido, int FD_FS){
	int tamanioMensaje = strlen(nombre_archivo)+ strlen(contenido);
	archivo* arch_final= malloc(tamanioMensaje);
	arch_final->contenido = contenido;
	arch_final->nombre = nombre_archivo;
    char * arch_final_serializado = serializarArchivo(arch_final);

	mensajesEnviadosAFileSystem(ALMACENADO_FINAL,FD_FS, arch_final_serializado, tamanioMensaje + sizeof(int)*2);

	free(arch_final_serializado);
	free(arch_final);
}
