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
	tabla_de_nodos.listaNodos = list_create();
	tabla_de_nodos.listaCapacidadNodos = list_create();
	list_nodos_id_fd = list_create();

	int socketFSServidor;
	socketFSServidor = lib_socketServidor(parametros->puertoFS_dn);




	Configuracion *config = leerArchivoDeConfiguracion(ARCHIVO_CONFIGURACION);



	//Creo Servidor Principal

	fd_set master;
	fd_set read_fds;
	int fd_max;
	int i;
	int FD_Cliente;
	int bytesRecibidos;
	char buffer2[4];

	FD_SET(socketFSServidor, &master);
	fd_max = socketFSServidor;

	for (;;) {

		read_fds = master;
		if (select(fd_max + 1, &read_fds, NULL, NULL, NULL) == -1) {

			logInfo("Error en el select");
		}
		for (i = 0; i <= fd_max; i++) {

			if (FD_ISSET(i, &read_fds)) {

				if (i == socketFSServidor) {

					if ((FD_Cliente = lib_aceptarYRegistrarSocket(
							socketFSServidor)) == -1) {

						logInfo("Error en el acept despues del select");

					} else {
						FD_SET(FD_Cliente, &master);
						if (FD_Cliente > fd_max) {
							fd_max = FD_Cliente;
						}
						logInfo(
								"Nueva conexion del socket cliente Data Node de FD: %i",
								FD_Cliente);
					}
				} else {

					//Recibo datos de algun cliente
					if ((bytesRecibidos = recv(i, buffer2, 4, 0)) <= 0) {
						if (bytesRecibidos == 0) {
							logInfo("Conexion cerrada del FD : %i", i);//VER QUE HACER ACA
						}
						close(i);
						FD_CLR(i, &master);

					} else {

						int codigo = deserializarINT(buffer2);
						logInfo("Recibi de DATA NODE: %i", codigo);
						mensajesRecibidosDeDN(codigo, i);
					}
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
    saludo_datanode * saludo;
    nodos_id_fd * nodos;
    char* get_bloque;


	switch (codigo) {
	case RTA_SET_BLOQUE:

		                        recv(FD_DN,pesoMensaje,4,0);
								tamanio= deserializarINT(pesoMensaje);
								logInfo("tamanio de lo que recibo %i", tamanio);
								mensaje = malloc(tamanio + 1);
								mensaje[tamanio] = '\0';
								recv(FD_DN, mensaje,tamanio,0);
								logInfo("Se reibio la respuesta de Set Bloque de Data Node: %s",  mensaje);
                                free(mensaje);

		break;

	case RTA_GET_BLOQUE: //RECIBE EL BLOQUE DE DATOS.


						recv(FD_DN,pesoMensaje,4,0);
						tamanio= deserializarINT(pesoMensaje);
						logInfo("tamanio de lo que recibo %i", tamanio);
						mensaje = malloc(tamanio + 1);
						mensaje[tamanio] = '\0';
						recv(FD_DN, mensaje,tamanio,0);
						logInfo("Se reibio el contenido de bloque: %s",  mensaje);


   //no hay que deseralizar por que el contenido es un *char
   //ver como seguir planificando con el bloque recibido



				        free(mensaje);
				        break;
	case SALUDO:

						recv(FD_DN,pesoMensaje,4,0);
						tamanio= deserializarINT(pesoMensaje);
						logInfo("tamanio de lo que recibo %i", tamanio);
						mensaje = malloc(tamanio + 1);
				        mensaje[tamanio] = '\0';
				        recv(FD_DN, mensaje,tamanio,0);
				        saludo = deserializar_saludo_datanode(mensaje);
				        logInfo("ID NODO: %i " , saludo->nombre_nodo);
				        logInfo("CAPACIDAD : %i " , saludo->capacidad_nodo);
				    	logInfo("IP DEL WORKER: %s " , saludo->ip_worker);
				        free(mensaje);

				        cargarNodos2(saludo->nombre_nodo, saludo->capacidad_nodo);
				        infoworker->ipWorker=saludo->ip_worker;
				        infoworker->puerto=puerto_worker;
				        list_add_in_index(list_info_workers,(saludo->nombre_nodo - 1), infoworker);


				        nodos->id_nodo=saludo->nombre_nodo;
						nodos->nodo_fd=FD_DN;
				        list_add_in_index(list_nodos_id_fd, (saludo->nombre_nodo - 1), nodos);



                        sem_post(&cantNodosAux);

				        break;

	    default:
		break;
	}
}


char *serialize_int(int value){
	size_t size = sizeof(int);
	char *stream = calloc(1, size);
	memcpy(stream, &value, size);
	return stream;
}

void cargarNodos2(int idNodo, int capacidad){






	/*
		 * int nodo
		nodos_id_fd * nodo;
		list_any_satisfy()

		for(int i=0;list_size(list_nodos_id_fd); i++){
		nodo=list_get(list_nodos_id_fd, i);
	      if(idNodo==nodo->id_nodo){
	    	  logInfo("Se volvio a conectar el nodo %s", idNodo);
	      }else{
	    	  logInfo("");
	      }*/




		tabla_de_nodos.tamanio=tabla_de_nodos.tamanio+capacidad;
		tabla_de_nodos.bloqueslibres=tabla_de_nodos.bloqueslibres+20;	//FALTA EL CASO DONDE SE DECONECTA EL NODO

		//int* numero = idNodo;

		list_add_in_index(tabla_de_nodos.listaNodos,idNodo - 1, idNodo);

		int* nodos;
		int cantidad = list_size(tabla_de_nodos.listaNodos);
		int count = 0;

	//	while(count<cantidad){
	//		nodos = list_get(tabla_de_nodos.listaNodos, count);
	//		logInfo("%d", nodos);
	//		count++;
	//	}

		bloques_nodo* nodo1 = malloc(sizeof(int)*23);
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





/*
					index_archivo= pathToIndiceArchivo("/home/utnso/tp-2017-2c-s1st3m4s_0p3r4t1v0s/FileSystem/archivoprueba.txt");
					if(index_archivo==-1){
						logInfo("RUTA DE ARCHIVO INVALIDA");
					}

					else{
						logInfo("PARTIENDO ARCHIVO ....");
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

							SetBloque *setbloque1= malloc(sizeof(char)*1024+sizeof(int));
							setbloque1->nrobloque= nroBloqueNodo1;
							setbloque1->contenidoBloque=contenido;
							char* mensaje= malloc(sizeof(int)+(sizeof(char)+strlen(setbloque1->contenidoBloque)));
							mensaje = serializarBloque(setbloque1->nrobloque,setbloque1->contenidoBloque);
							int tamanioSetBloque= sizeof(int)+(sizeof(char)+strlen(setbloque1->contenidoBloque));
							mensajesEnviadosADataNode(SET_BLOQUE, idNodoCopia1, mensaje,tamanioSetBloque);

							SetBloque *setbloque2= malloc(sizeof(char)*1024+sizeof(int));
							setbloque2->nrobloque= nroBloqueNodo2;
							setbloque2->contenidoBloque=contenido;
							char* mensaje2= malloc(sizeof(int)+(sizeof(char)+strlen(setbloque2->contenidoBloque)));
							mensaje2 = serializarBloque(setbloque2->nrobloque,setbloque2->contenidoBloque);
							int tamanioSetBloque2= sizeof(int)+(sizeof(char)+strlen(setbloque2->contenidoBloque));
							mensajesEnviadosADataNode(SET_BLOQUE, idNodoCopia2, mensaje2,tamanioSetBloque2);

							i++;

							free(mensaje);
							free(mensaje2);
*/





/*
				//getbloque
				char * nrobloque = malloc(sizeof(int));
				int bloque=5;
				nrobloque = serialize_int(bloque);
				int tamanio=strlen(nrobloque);

				mensajesEnviadosADataNode(GET_BLOQUE,FD_Cliente,nrobloque,sizeof(tamanio));


				SetBloque *bloque2 = malloc(sizeof(SetBloque));
				bloque2->nrobloque=6;
				bloque2->contenidoBloque="bloque del archivo";
				char* mensaje= malloc(sizeof(int)+(sizeof(char)+strlen(bloque2->contenidoBloque)));
				mensaje = serializarBloque(bloque2->nrobloque,bloque2->contenidoBloque);
				int tamanioSetBloque= sizeof(int)+(sizeof(char)+strlen(bloque2->contenidoBloque));
				mensajesEnviadosADataNode(SET_BLOQUE, FD_Cliente, mensaje,tamanioSetBloque);


				free(nrobloque);
				free(mensaje);
*/
