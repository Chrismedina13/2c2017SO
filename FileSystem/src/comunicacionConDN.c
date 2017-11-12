/*
 * comunicacionConDN.c
 *
 *  Created on: 16/9/2017
 *      Author: utnso
 */
#include "Headers/comunicacionConYama.h"
#include "Headers/comunicacionConDN.h"
#include "SO_lib/estructuras.h"
#include "Headers/FileSystem.h"
#include "Headers/configuracion.h"




void comunicacionDN(ParametrosComunicacion* parametros){

	int socketWorkerServidor;
	socketWorkerServidor = lib_socketServidor(parametros->puertoFS_dn);

	fd_set master;
	fd_set read_fds;
	int fd_max;
	int i;
	int FD_Cliente;
	int bytesRecibidos;
	char buffer[20];
    char buffer_ipWorker[4];
	int cantBloques=20;
	Configuracion *config = leerArchivoDeConfiguracion(ARCHIVO_CONFIGURACION);
	int cantNodos= config->cant_nodos;
	int cantArchivos = config->cant_archivos;
	int h=0;




	FD_SET(socketWorkerServidor,&master);
	fd_max = socketWorkerServidor;

	for(;;){

		read_fds = master;
		if(select(fd_max+1, &read_fds,NULL,NULL,NULL) == -1){

			logInfo("Error en el select");
		}
		for(i = 0;i <= fd_max; i++){

			if(FD_ISSET(i,&read_fds)){

				if(i == socketWorkerServidor){

					if((FD_Cliente = lib_aceptarYRegistrarSocket(socketWorkerServidor)) == -1){

						logInfo("Error en el aceept despues del select");

					}else{
						FD_SET(FD_Cliente,&master);
						if(FD_Cliente > fd_max){
							fd_max = FD_Cliente;
						}
						logInfo("Nueva conexion del socket cliente DataNode de FD: %i",FD_Cliente); //FD_cliente es mi id_nodo

						t_list* list_info_workers = list_create();
						while(h< cantNodos){
						 recv(FD_Cliente, buffer_ipWorker,4,0);
						 int codigo = deserializarINT(buffer_ipWorker);
						 logInfo("Recibi de DATA NODE el codigo : %i", codigo);
						 mensajesRecibidosDeDN(codigo,FD_Cliente);
					   	 h++;
						}

						 //

						tabla_de_nodos.listaNodos = list_create();
						tabla_de_nodos.listaCapacidadNodos = list_create();
						cargarNodos2(FD_Cliente);

                      //esto seria un while por cada archivo


					int index_archivo= pathToIndiceArchivo("/home/utnso/tp-2017-2c-s1st3m4s_0p3r4t1v0s/FileSystem/archivoprueba.txt");


					t_list *	listaBloques=list_create();
					t_list *    listaUbicacionesDelBloque=list_create();

				listaUbicacionesDelBloque=	tabla_de_archivos[index_archivo].ubicaciones; //lista de ubicacionbloquesArchivo
				listaBloques=	tabla_de_archivos[index_archivo].bloques; // lista de bloques de char *

						i=0;
						while(i<list_size(listaBloques)){

							UbicacionBloquesArchivo * ubicacionbloquesarchivo;
						   ubicacionbloquesarchivo= list_get(listaUbicacionesDelBloque,i);
						   int idNodoCopia1=ubicacionbloquesarchivo->ubicacionCopia1.nodo;
						   int idNodoCopia2=ubicacionbloquesarchivo->ubicacionCopia2.nodo;

						int nroBloqueNodo1= ubicacionbloquesarchivo->ubicacionCopia1.desplazamiento;
					    int nroBloqueNodo2= ubicacionbloquesarchivo->ubicacionCopia2.desplazamiento;

						   char * contenido = list_get(listaBloques, i);

							SetBloque *setbloque1= malloc(sizeof(SetBloque));
						    setbloque1->nrobloque= nroBloqueNodo1;
							setbloque1->contenidoBloque=contenido;
							char* mensaje= malloc(sizeof(int)+(sizeof(char)+strlen(setbloque1->contenidoBloque)));
							mensaje = serializarBloque(setbloque1->nrobloque,setbloque1->contenidoBloque);
							int tamanioSetBloque= sizeof(int)+(sizeof(char)+strlen(setbloque1->contenidoBloque));
							mensajesEnviadosADataNode(SET_BLOQUE, idNodoCopia1, mensaje,tamanioSetBloque);

							SetBloque *setbloque2= malloc(sizeof(SetBloque));
						 	setbloque2->nrobloque= nroBloqueNodo2;
							setbloque2->contenidoBloque=contenido;
							char* mensaje2= malloc(sizeof(int)+(sizeof(char)+strlen(setbloque2->contenidoBloque)));
							mensaje2 = serializarBloque(setbloque2->nrobloque,setbloque2->contenidoBloque);
							int tamanioSetBloque2= sizeof(int)+(sizeof(char)+strlen(setbloque2->contenidoBloque));
							mensajesEnviadosADataNode(SET_BLOQUE, idNodoCopia2, mensaje2,tamanioSetBloque2);

                             i++;

                            free(mensaje);
                            free(mensaje2);
						}
					}


					}
				}else{

					//Recibo datos de algun cliente


					if((bytesRecibidos = recv(i,buffer,20,0)) <= 0){

						if(bytesRecibidos == 0){
							logInfo("Conexion cerrada del FD : %i",i);

						}
						close(i);
						FD_CLR(i,&master);

					}else{
						logInfo("Recibi de DATANODE: %s",buffer); // buffer es el mensaje "hola soy data node"



                 // char * nrobloque = malloc(sizeof(int));
                //  int bloque=5;
                // nrobloque = serialize_int(bloque);
                //  int tamanio=strlen(nrobloque);

                 // mensajesEnviadosADataNode(GET_BLOQUE,FD_Cliente,nrobloque,sizeof(tamanio));


						SetBloque *bloque2 = malloc(sizeof(SetBloque));
						bloque2->nrobloque=6;
						bloque2->contenidoBloque="bloque del archivo";
						char* mensaje= malloc(sizeof(int)+(sizeof(char)+strlen(bloque2->contenidoBloque)));
                        mensaje = serializarBloque(bloque2->nrobloque,bloque2->contenidoBloque);
						int tamanioSetBloque= sizeof(int)+(sizeof(char)+strlen(bloque2->contenidoBloque));
						mensajesEnviadosADataNode(SET_BLOQUE, FD_Cliente, mensaje,tamanioSetBloque);


					//free(nrobloque);
						free(mensaje);


					}
				}

				}
			}

		}


void mensajesEnviadosADataNode(int codigo, int FD_DataNode, char* mensaje,int tamanio) {
	Paquete * paqueteEnvioDeBloque;
	Paquete* paqueteGetBloque;

	switch (codigo) {

	case SET_BLOQUE:
		paqueteEnvioDeBloque = crearPaquete(SET_BLOQUE, tamanio,mensaje);

		if (enviarPaquete(FD_DataNode, paqueteEnvioDeBloque) == -1) {
			logInfo("Error en envio del Bloque");
		}

		destruirPaquete(paqueteEnvioDeBloque);
		break;

	case GET_BLOQUE:
		logInfo(
				"FILESYSTEM SOLICITA UN BLOQUE");
		paqueteGetBloque= crearPaquete(GET_BLOQUE, tamanio,
				mensaje);

		if (enviarPaquete(FD_DataNode, paqueteGetBloque) == -1) {
			logInfo("Error en envio de GET BLOQUE.");
		}



		destruirPaquete(paqueteGetBloque);

		break;


	default:
		break;
	}
}

void mensajesRecibidosDeDN(int codigo, int FD_DN) {

	char pesoMensaje[8];
	int tamanio;
	char * mensaje;
	Configuracion *config = leerArchivoDeConfiguracion(ARCHIVO_CONFIGURACION);
    int puerto_worker =  config->puerto_worker;
    Info_Workers * infoworker;


	switch (codigo) {
	case SET_BLOQUE:

						recv(FD_DN, pesoMensaje, 8, 0);
						tamanio = deserializarINT(pesoMensaje);

						logInfo("tamanio de lo que recibo %i", tamanio);
						//mensaje = malloc(tamanio + 1);
						//mensaje[tamanio] = '\0';
						logInfo("RECIBI OK DE GUARDADO DE BLOQUE POR PARTE DE DATA NODE");



		break;
		case IP_NODO:

			logInfo("FILE SYSTEM RECICE EL IP DEL WORKER");

			recv(FD_DN, pesoMensaje,4,0);
				tamanio = deserializarINT(pesoMensaje);
				logInfo("tamanio de lo que recibo %i", tamanio);
				mensaje = malloc(tamanio + 1);
				mensaje[tamanio] = '\0';
				recv(FD_DN,mensaje, tamanio,0 );

			   logInfo("Recibi el ip del worker %s", mensaje);

			   infoworker->ipWorker=mensaje;

			   infoworker->puerto=puerto_worker;
			   list_add_in_index(list_info_workers, FD_DN, infoworker);



				break;
	case GET_BLOQUE: //RECIBE EL BLOQUE DE DATOS.


						recv(FD_DN,pesoMensaje,8,0);
						tamanio= deserializarINT(pesoMensaje);
						logInfo("tamanio de lo que recibo %i", tamanio);
						logInfo("RECIBO DE DATA NODE CONTENIDO DE UN BLOQUE");
						mensaje = malloc(1024*1024);
				        recv(FD_DN, mensaje,8,0);//chequiar linea
   //no hay que deseralizar por que el contenido es un *char
   //ver como seguir planificando con el bloque recibido
				        free(mensaje);
				        break;
	}
}


char *serialize_int(int value){
	size_t size = sizeof(int);
	char *stream = calloc(1, size);
	memcpy(stream, &value, size);
	return stream;
}

void cargarNodos2(int idNodo){

		tabla_de_nodos.tamanio=tabla_de_nodos.tamanio+20;
		tabla_de_nodos.bloqueslibres=tabla_de_nodos.bloqueslibres+20;	//FALTA EL CASO DONDE SE DECONECTA EL NODO

		//int* numero = idNodo;

		list_add(tabla_de_nodos.listaNodos,idNodo);

		bloques_nodo* nodo1 = malloc(sizeof(bloques_nodo));
		nodo1->idNodo=idNodo;
		nodo1->bloquesTotales=20;
		nodo1->bloquesLibres=20; // falta ver q pasa con un nodo viejo
		int i=0;
		while(i<20){
		nodo1->bitmap[i]=0;
		i++;
		}

		list_add(tabla_de_nodos.listaCapacidadNodos,nodo1);



		int resp = crearRegistroArchivoNodos(tabla_de_nodos);
		if(resp==0) printf("\nRegistro de Nodo cargado correctamente.\n");
		else printf("\nRegistro de Nodo no pudo ser cargado.\n");


	//	int cantNodos = list_size(tabla_de_nodos.listaNodos);
	//	t_list* lista_nodos;

		//lista_nodos = tablaNodosToNodos(tabla_de_nodos.listaNodos);
		//FIN EJEMPLO NODOS CARGADOS

}
