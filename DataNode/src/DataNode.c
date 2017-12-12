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
	char buffer2[4];



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

/*
//prueba mandar un get bloque
   char* bloque =malloc(1024*1024 + sizeof(char));
 bloque=get_bloque(0);
 logInfo("el tamanio del bloque es  %i", strlen(bloque));

mensajesEnviadosAFileSystem(RTA_GET_BLOQUE, FDsocketClienteFileSystem, bloque, strlen(bloque)  );

*/
    while(1){

	if(recv(FDsocketClienteFileSystem, buffer2,4,0)>=0){
		int codigo2 = deserializarINT(buffer2);
		logInfo("Recibi de FS el codigo : %i", codigo2);
		//if(codigo2==)
		mensajesRecibidosDeFileSystem(codigo2,FDsocketClienteFileSystem);
	}
}


/*	//ejemplo set_bloque


	char* bloque = malloc(1024*1024);

	bloque= get_bloque(2);
	printf("%s", bloque);


	if (set_bloque(bloque, 1)==0){
		logInfo("READY SET BLOQUE, AVISAR A FILESYSTEM");
	}
	else{
		logInfo("FALLO SET BLOQUE");
	}

	free(bloque);
*/

	free(config);


//free(bloque);

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

	SetBloque* bloque=malloc(1024*1024+sizeof(int));
	char * contenido_bloque;
	int tamanio_contenido;
	char * rta_set_bloque;
	int tamanio_rta_set_bloque;
	char* bloqueRecibido;

	char* bloque_obtenido;
	int tamanio_bloque_obtenido;

	switch (codigo) {
	case SET_BLOQUE:

		logInfo("DataNode Recibe la instruccion 'SetBloque' de FileSystem");

					// ese mensaje tiene que ser un buffer no un char*


		recv(FD_FileSystem, pesoMensaje,4,0);
		tamanio = deserializarINT(pesoMensaje);

		logInfo("tamanio de lo que recibo %i", tamanio);
		mensaje = malloc(tamanio);

		bloque->contenidoBloque = receive_basic(FD_FileSystem,tamanio);
		logInfo("Contenido del bloque : %s",bloque->contenidoBloque);

		recv(FD_FileSystem,bufferBloque,4,0);
		bloque->nrobloque = deserializarINT(bufferBloque);

		if (set_bloque(bloque->contenidoBloque,bloque->nrobloque)==0){
			logInfo("READY SET BLOQUE, AVISAR A FILESYSTEM");
			mensajesEnviadosAFileSystem(RTA_SET_BLOQUE, FD_FileSystem, "GUARDE OK EL BLOQUE", 19);
		}
		else{
			logInfo("FALLO SET BLOQUE");
		}

      free(mensaje);


		break;

	case GET_BLOQUE:

		logInfo("DataNode Recibe la instruccion 'GetBloque' de FileSystem");



		recv(FD_FileSystem,pesoMensaje,4,0);

		tamanio= deserializarINT(pesoMensaje);
		logInfo("tamanio de lo que recibo %i", tamanio);
		mensaje = malloc(tamanio + 1);
		mensaje[tamanio] = '\0';
		recv(FD_FileSystem,mensaje, tamanio,0);
		intRecibido = deserializarINT(mensaje);
		logInfo("Instruccion 'Get Bloque' en el bloque %i", intRecibido);
		bloque_obtenido = malloc(1024*1024 + sizeof(char));
		bloque_obtenido = get_bloque(intRecibido);
		tamanio_bloque_obtenido = strlen(bloque_obtenido);
		mensajesEnviadosAFileSystem(RTA_GET_BLOQUE, FD_FileSystem, bloque_obtenido, tamanio_bloque_obtenido);

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


