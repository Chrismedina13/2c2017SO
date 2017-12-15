#include "Headers/ComunicacionConFileSystem.h"
#include "Headers/logYAMA.h"
#include "SO_lib/estructuras.h"
#include "SO_lib/serializacion.h"

void comunicacionConFileSystem(){
	//socketClienteParaFileSystem

	FDsocketClienteFileSystem = lib_SocketCliente(parametrosFileSystem->ip, parametrosFileSystem->puerto);
	logInfo("SocketCliente = %d \n", FDsocketClienteFileSystem);
	char buffer[13];
	if (recv(FDsocketClienteFileSystem, buffer, 13, 0) != -1) {
		logInfo("Se recibio: %s", buffer);

	}

	while(1){

		recv(FDsocketClienteFileSystem,buffer,4,0);
		int codigo = deserializarINT(buffer);
		mensajesRecibidosDeFS(codigo,FDsocketClienteFileSystem);

	}


}

ParametrosComunicacionConFileSystem* setParametrosComunicacionConFileSystem(int puerto, char* ip, char* algoritmo, int disponiblidadBase,int retardo) {
	ParametrosComunicacionConFileSystem* parametros = malloc(sizeof(int)*4 + strlen(algoritmo));

	parametros->ip = ip;
	parametros->puerto = puerto;
	parametros->algoritmo = algoritmo;
	parametros->disponibilidadBase = disponiblidadBase;
	parametros->retardo = retardo;
	return parametros;

}

void mensajesRecibidosDeFS(int codigo, int FDsocketClienteFileSystem) {
//Yama recibe de FILESYSTEM
	int tamanioEstructura, tamanio;
	char* mensaje; //en este caso el mensaje es la lista de bloques del archivo
	char pesoMensaje[4];
	t_list * lista_ubicaciones;
	int i = 0;
	int a = 0;
	t_list* lista;
	Job* jobAEjecutar;

	switch (codigo) {
	case UBICACION_BLOQUES:

		logInfo("RECIBIENOD LA LISTA DE UBICACION BLOQUES ARCHIVOS");
		            recv(FDsocketClienteFileSystem, pesoMensaje, 4, 0);
					tamanio = deserializarINT(pesoMensaje);
					logInfo("tamanio de lo que recibo %i", tamanio);
					mensaje = malloc(tamanio + 1);
					mensaje[tamanio] = '\0';

					if (recv(FDsocketClienteFileSystem, mensaje, tamanio, 0) == -1) {
						logInfo("Error en la recepcion de la lista de Bloques que componen el archivo.");
					} else  {

	            	  lista_ubicaciones= deserializarListaUbicacionBloquesArchivo2(mensaje);

					logInfo("se RECIBIO LA LISTA DE UBICACIONES CON %i PARTES DEL ARCHIVO", list_size(lista_ubicaciones));


	            	  for(i=0;list_size(lista_ubicaciones);i++){

	      	  	  	  UbicacionBloquesArchivo*ubicacionb = list_get(lista_ubicaciones,i);
	    	  	  	  logInfo("Se recibio la parte del archivo: %i", ubicacionb->parteDelArchivo);
	    	  	  	  logInfo("Se recibio el ubi 1 en nodo %i desplazamiento %i", ubicacionb->ubicacionCopia1.nodo, ubicacionb->ubicacionCopia1.desplazamiento);
	    	  	  	  logInfo("Se recibio el ubi 2 en nodo %i desplazamiento %i", ubicacionb->ubicacionCopia2.nodo, ubicacionb->ubicacionCopia2.desplazamiento);
	    	  	  	  logInfo("Con la cantidad de bytes Ocupados: %i", ubicacionb->bytesOcupados);
	      }


			logInfo("Se recibió de forma correcta la lista de bloques de archivo enviada por FS.",tamanio);
               break;

            //mutex1

   			jobAEjecutar = retirarJobDeLista();


/*           	logInfo("Creando Planificacion de prueba");

           	t_list* listaDeWorkersAPlanificar = list_create();
           	UbicacionBloquesArchivo2* ubi0 = crearUbicacionBloquesArchivos2(0, 500, 2, 12, 2, 1);
           	UbicacionBloquesArchivo2* ubi1 = crearUbicacionBloquesArchivos2(1, 100, 2, 20, 3, 2);
           	UbicacionBloquesArchivo2* ubi2 = crearUbicacionBloquesArchivos2(2, 100, 2, 20, 3, 3);
           	UbicacionBloquesArchivo2* ubi3 = crearUbicacionBloquesArchivos2(3, 100, 1, 20, 2, 4);
           	UbicacionBloquesArchivo2* ubi4 = crearUbicacionBloquesArchivos2(4, 100, 1, 20, 3, 5);
           	UbicacionBloquesArchivo2* ubi5 = crearUbicacionBloquesArchivos2(5, 100, 2, 20, 3, 6);
           	UbicacionBloquesArchivo2* ubi6 = crearUbicacionBloquesArchivos2(6, 100, 1, 20, 2, 19);


           	list_add(listaDeWorkersAPlanificar, ubi0);
           	list_add(listaDeWorkersAPlanificar, ubi1);
           	list_add(listaDeWorkersAPlanificar, ubi2);
           	list_add(listaDeWorkersAPlanificar, ubi3);
           	list_add(listaDeWorkersAPlanificar, ubi4);
           	list_add(listaDeWorkersAPlanificar, ubi5);
           	list_add(listaDeWorkersAPlanificar, ubi6);
*/

           	logInfo("Se creo la lista de Workers a planificar , empieza planificacion");

           	t_list* planificacionDelJOb = planificar(lista_ubicaciones,parametrosFileSystem->algoritmo, parametrosFileSystem->disponibilidadBase, jobAEjecutar);

           	logInfo("Se crea JOB Completo");
           	JOBCompleto* jobCompleto = crearJobCompleto(jobAEjecutar,lista_ubicaciones, planificacionDelJOb);

           	logInfo("Actualizar Tabla Global");
           	ingresarDatosATablaGlobal(jobCompleto);

           	logInfo("Serializar respueta transformacion a YAMA");

           	int tamanioRespuesta = (tamanioRespuestaTransformacionYAMA(planificacionDelJOb) +
           			((sizeof(int)*2)*list_size(planificacionDelJOb)) + (2*list_size(planificacionDelJOb)));

           	char* respuesta = serializarListaYAMA(planificacionDelJOb);

           	logInfo("listo para enviar el tamanio de la respuesta es %i", tamanioRespuesta);

           	mensajesEnviadosAMaster(SOL_TRANSFORMACION, jobCompleto->job->master,
           			respuesta, tamanioRespuesta);

           	logInfo("Se envio respuesta correctamaente");


           	list_add(listaDeJobs, jobCompleto);

           	mostrarTabla();
					}
           	break;
	case INFO_WORKER:
			recv(FDsocketClienteFileSystem, pesoMensaje, 4, 0);
			tamanio = deserializarINT(pesoMensaje);
			logInfo("tamanio de lo que recibo %i", tamanio);
			mensaje = malloc(tamanio + 1);
			mensaje[tamanio] = '\0';
			recv(FDsocketClienteFileSystem, mensaje, tamanio, 0);

			logInfo("Se recibe la lista de los ips y puertos de los workers");

			lista = deserializarLista_info_workers(mensaje);

			logInfo("Se deserializo la lista de ips y puertos de los workers");

			logInfo("Hay conectados %i", list_size(lista));

			ipYPuertoWoerkers = lista;

			while(a < list_size(lista)){
				Info_Workers* info = list_get(lista,a);
				logInfo("tomo el nodo %i", a);

				logInfo("puerto de nodo = %i \n ", info->puerto);
				logInfo("ip de nodo  = %s \n ", info->ipWorker);

				a++;
			}

			break;
		}

	}


