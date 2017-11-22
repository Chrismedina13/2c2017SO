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
	//Archivo de Lo


	//char* buffer = malloc(4);

	char bufferBloque[4];
	char buffer[4];


	crearLog("DataNode.log", "DATANODE", 1, log_level_from_string("INFO"));

	//Configuracion
	Configuracion *config = leerArchivoDeConfiguracion(ARCHIVO_CONFIGURACION);

	logInfo("Archivo de configuracion puerto DataNode : %i \n", config->puertoDataNode);
	logInfo("Archivo de configuracion puerto worker : %i \n", config->puertoWorker);
	logInfo("Archivo de configuracion puerto fileSystem : %i \n", config->puertoFileSystem);
	logInfo("Archivo de configuracion ip fileSystem : %s \n", config->ipFileSystem);
	logInfo("Archivo de configuracion nombre nodo : %s \n", config->nombreNodo);
	logInfo("Archivo de configuracion ruta data.bin : %s \n", config->rutaDataBin);
	logInfo("Archivo de configuracion ruta data.bin : %s \n", config->ipNodo);

	//socketClienteParaFileSystem

	int FDsocketClienteFileSystem;
	FDsocketClienteFileSystem = lib_SocketCliente(config->ipFileSystem,config->puertoFileSystem);

	logInfo("SocketCliente = %d \n",FDsocketClienteFileSystem);
     char* saludo = "HOLA, SOY DATA NODE";
     int tamanioSaludo = strlen(saludo);
     mensajesEnviadosAFileSystem(SALUDO, FDsocketClienteFileSystem, saludo,tamanioSaludo);

	//if(send(FDsocketClienteFileSystem,"Hola soy DATANODE",20,0) != -1){
		//logInfo("Se mando mensaje a FS correctamente");
//	}

	//mensajesEnviadosAFileSystem(IP_NODO,FDsocketClienteFileSystem,config->ipNodo, (sizeof(char)+strlen(config->ipNodo)));


	//recv(FDsocketClienteFileSystem, buffer,4,0);
	//int codigo = deserializarINT(buffer);
	//logInfo("Recibi de FS el codigo : %i", codigo);
	//mensajesRecibidosDeFileSystem(codigo,FDsocketClienteFileSystem);


	//recv(FDsocketClienteFileSystem, bufferBloque,4, 0);
	//int codigo2 =deserializarINT(bufferBloque);
	//logInfo("Recibi de FS el codigo : %i", codigo2);
//	mensajesRecibidosDeFileSystem(codigo2,FDsocketClienteFileSystem);



	free(config);




	return EXIT_SUCCESS;
}


void mensajesRecibidosDeFileSystem(int codigo, int FD_FileSystem) {

//Data node recibe de File System
	char pesoMensaje[8];
	int tamanio;
	char* mensaje;
	char* buffer[4];
	int intRecibido;
	char* bufferBloque[4];

	SetBloque* bloque;

	switch (codigo) {
	case SET_BLOQUE:

		logInfo("DataNode Recibe la instruccion 'SetBloque' de FileSystem");

					// ese mensaje tiene que ser un buffer no un char*


		recv(FD_FileSystem, pesoMensaje,4,0);
		tamanio = deserializarINT(pesoMensaje);
		logInfo("tamanio de lo que recibo %i", tamanio);
		mensaje = malloc(tamanio + 1);
		mensaje[tamanio] = '\0';
		recv(FD_FileSystem,mensaje, tamanio,0 );
    	bloque=deserilizarBloque(mensaje);
	   logInfo("Recibi el nro de bloque %i", bloque->nrobloque);
	   logInfo("Contenido del bloque : %s",bloque->contenidoBloque);


	//DESERIALIZAR SET BLOQUE

free(mensaje);
// ir a nodos.bin y guardarlo

		//sem_post(&semaforoYAMA);
		break;

	case GET_BLOQUE: //ESTARIA TERMINADO

		logInfo("DataNode Recibe la instruccion 'GetBloque' de FileSystem");

				// ese mensaje tiene que ser un buffer no un char*

		recv(FD_FileSystem,pesoMensaje,4,0);

		tamanio= deserializarINT(pesoMensaje);
		logInfo("tamanio de lo que recibo %i", tamanio);
		mensaje = malloc(tamanio + 1);
		mensaje[tamanio] = '\0';
		recv(FD_FileSystem,mensaje, tamanio,0);

		intRecibido = deserializarINT(mensaje);
		logInfo("Instruccion 'Get Bloque' en el bloque %i", intRecibido);

//IR A NODOS.BIN Y AGARRAR ESE BLOQUE Y MANDARLO
free(mensaje);
		break;

	default:
		logInfo("DATA NODE RECIBE UNA SENIAL QUE NO SABE TRADUCIR");
		break;
	}
}


void mensajesEnviadosAFileSystem(int codigo, int FD_FileSystem, char* mensaje, int tamanio) {
	Paquete * paqueteEnvio;
	switch (codigo) {

	case SALUDO:
      logInfo("se manda a file system el saludo");
		paqueteEnvio = crearPaquete(SALUDO, tamanio,mensaje);
				if (enviarPaquete(FD_FileSystem, paqueteEnvio) == -1) {
					logInfo("ERROR EN EL ENVIO DE SALUDO");
				}

				destruirPaquete(paqueteEnvio);
				free(mensaje);
				break;
	case IP_NODO:

			paqueteEnvio = crearPaquete(IP_NODO, tamanio,mensaje); //cuando envia set bloque es porque lo guardo OK

			if (enviarPaquete(FD_FileSystem, paqueteEnvio) == -1) {
				logInfo("Error en envio de respuesta del Set Bloque");
			}

			destruirPaquete(paqueteEnvio);
			free(mensaje);
			break;

	case SET_BLOQUE:

		paqueteEnvio = crearPaquete(SET_BLOQUE, tamanio,mensaje); //cuando envia set bloque es porque lo guardo OK

		if (enviarPaquete(FD_FileSystem, paqueteEnvio) == -1) {
			logInfo("Error en envio de respuesta del Set Bloque");
		}

		destruirPaquete(paqueteEnvio);
		free(mensaje);
		break;

	case GET_BLOQUE:


		logInfo(
				"DATA NODE ENVIA EL CONTENIDO DE UN BLOQUE A FILESYSTEM");
		paqueteEnvio= crearPaquete(GET_BLOQUE, tamanio,mensaje);

		if (enviarPaquete(FD_FileSystem, paqueteEnvio) == -1) {
			logInfo("Error en envio de contenido del bloque.");
		}

		destruirPaquete(paqueteEnvio);
		free(mensaje);
		break;


	default:
		break;
	  }
}


