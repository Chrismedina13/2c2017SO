/*
 ============================================================================
 Name        : DataNode.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */


#include "Headers/DataNode.h"

int main(int argc, char *argv[]) {
	//Archivo de Logs

	crearLog("DataNode.log", "DATANODE", 1, log_level_from_string("INFO"));

	//Configuracion
	Configuracion *config = leerArchivoDeConfiguracion(ARCHIVO_CONFIGURACION);

	logInfo("Archivo de configuracion puerto DataNode : %i \n", config->puertoDataNode);
	logInfo("Archivo de configuracion puerto worker : %i \n", config->puertoWorker);
	logInfo("Archivo de configuracion puerto fileSystem : %i \n", config->puertoFileSystem);
	logInfo("Archivo de configuracion ip fileSystem : %s \n", config->ipFileSystem);
	logInfo("Archivo de configuracion nombre nodo : %s \n", config->nombreNodo);
	logInfo("Archivo de configuracion ruta data.bin : %s \n", config->rutaDataBin);

	//socketClienteParaFileSystem

	int FDsocketClienteFileSystem;
	FDsocketClienteFileSystem = lib_SocketCliente(config->ipFileSystem,config->puertoFileSystem);

	logInfo("SocketCliente = %d \n",FDsocketClienteFileSystem);


	if(send(FDsocketClienteFileSystem,"Hola soy DATANODE",17,0) != -1){
		logInfo("Se mando mensaje a FS correctamente");
	}
	free(config);



	return EXIT_SUCCESS;
}
