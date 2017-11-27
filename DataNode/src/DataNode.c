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

	//char bufferBloque[4];
	char buffer[4];



	crearLog("DataNode.log", "DATANODE", 1, log_level_from_string("INFO"));

	//Configuracion
	Configuracion *config = leerArchivoDeConfiguracion(ARCHIVO_CONFIGURACION);

	logInfo("Archivo de configuracion puerto DataNode : %i \n", config->puertoDataNode);
	logInfo("Archivo de configuracion puerto worker : %i \n", config->puertoWorker);
	logInfo("Archivo de configuracion puerto fileSystem : %i \n", config->puertoFileSystem);
	logInfo("Archivo de configuracion ip fileSystem : %s \n", config->ipFileSystem);
	logInfo("Archivo de configuracion nombre nodo : %i \n", config->nombreNodo);
	logInfo("Archivo de configuracion ruta data.bin : %s \n", config->rutaDataBin);
	logInfo("Archivo de configuracion ip Worker : %s \n", config->ipNodo);
	logInfo("Archivo de configuracion capacidad del nodo : %i \n", config->capacidadNodo);

	//socketClienteParaFileSystem

	int FDsocketClienteFileSystem;
	FDsocketClienteFileSystem = lib_SocketCliente(config->ipFileSystem,config->puertoFileSystem);

	logInfo("SocketCliente = %d \n",FDsocketClienteFileSystem);
	saludo_datanode * saludo_dn = malloc(sizeof(int)*2+ strlen(config->ipNodo)+ sizeof(char)*2 + 20);

	saludo_dn->capacidad_nodo=config->capacidadNodo;
	saludo_dn->nombre_nodo=config->nombreNodo;
	saludo_dn->ip_worker=config->ipNodo;

	char *saludoSerializado;// malloc(( strlen(saludo_dn->saludo)+strlen(saludo_dn->ip_worker) + sizeof(int)*2+ sizeof(char)*2 ));
			saludoSerializado = serializar_saludo(saludo_dn->nombre_nodo ,saludo_dn->capacidad_nodo, saludo_dn->ip_worker);

     int tamanioSaludo = ((sizeof(char)+strlen(saludo_dn->ip_worker)) + sizeof(int)*2) ;

     mensajesEnviadosAFileSystem(SALUDO, FDsocketClienteFileSystem, saludoSerializado,tamanioSaludo);




	//recv(FDsocketClienteFileSystem, buffer,4,0);
	//int codigo = deserializarINT(buffer);
	//logInfo("Recibi de FS el codigo : %i", codigo);
	//mensajesRecibidosDeFileSystem(codigo,FDsocketClienteFileSystem);


	recv(FDsocketClienteFileSystem, buffer,4, 0);
	int codigo =deserializarINT(buffer);
	logInfo("Recibi de FS el codigo : %i", codigo);
    mensajesRecibidosDeFileSystem(codigo,FDsocketClienteFileSystem);




	char* bloque = malloc(1024*1024);

	bloque= get_bloque(3);
	logInfo(bloque);
     free(bloque);



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
	char * contenido_bloque;
	int tamanio_contenido;
	char * rta_set_bloque;
	int tamanio_rta_set_bloque;

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



      free(mensaje);
// ir a nodos.bin y guardarlo

    //  rta_set_bloque = "Guarde el ok el contenido en el bloque";
    //  tamanio_rta_set_bloque= strlen(rta_set_bloque);
 //mensajesEnviadosAFileSystem(RTA_SET_BLOQUE, FD_FileSystem, rta_set_bloque, tamanio_rta_set_bloque);

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
	//contenido_bloque = funcion que recorra el data.bin y me traiga el mega
		//tamanio_contenido= sizeof(char) + strlen(contenido_bloque);
      //mensajesEnviadosAFileSystem(RTA_GET_BLOQUE, FD_FileSystem,contenido_bloque, tamanio_contenido );


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

	case RTA_SET_BLOQUE:

		paqueteEnvio = crearPaquete(RTA_SET_BLOQUE, tamanio,mensaje); //cuando envia set bloque es porque lo guardo OK

		if (enviarPaquete(FD_FileSystem, paqueteEnvio) == -1) {
			logInfo("Error en envio de respuesta del Set Bloque");
		}

		destruirPaquete(paqueteEnvio);
		free(mensaje);
		break;

	case RTA_GET_BLOQUE:


		logInfo(
				"DATA NODE ENVIA EL CONTENIDO DE UN BLOQUE A FILESYSTEM");
		paqueteEnvio= crearPaquete(RTA_GET_BLOQUE, tamanio,mensaje);

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


