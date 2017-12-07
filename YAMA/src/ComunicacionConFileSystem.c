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

		//recibiria un codigo de mensaje y hace lo que tiene que hacer
		//hacer el if para recibir los bloques de archivos
		recv(FDsocketClienteFileSystem,buffer,4,0);
		int codigo = deserializarINT(buffer);
		mensajesRecibidosDeFS(codigo,FDsocketClienteFileSystem);

	}


}

void atenderJOB(){

	// esta funcion va a desaparecer es solo ahora para las pruebas

	Job* jobAEjecutar = retirarJobDeLista();

	logInfo("Creando Planificacion de prueba");

	t_list* listaDeWorkersAPlanificar = list_create();
	UbicacionBloquesArchivo* ubi0 = crearUbicacionBloquesArchivos(0, 500, 1, 12, 2, 13);
	UbicacionBloquesArchivo* ubi1 = crearUbicacionBloquesArchivos(1, 100, 1, 20, 3, 19);
	UbicacionBloquesArchivo* ubi2 = crearUbicacionBloquesArchivos(2, 100, 2, 20, 3, 19);
	UbicacionBloquesArchivo* ubi3 = crearUbicacionBloquesArchivos(3, 100, 1, 20, 2, 19);
	UbicacionBloquesArchivo* ubi4 = crearUbicacionBloquesArchivos(4, 100, 1, 20, 3, 19);
	UbicacionBloquesArchivo* ubi5 = crearUbicacionBloquesArchivos(5, 100, 2, 20, 3, 19);
	UbicacionBloquesArchivo* ubi6 = crearUbicacionBloquesArchivos(6, 100, 1, 20, 2, 19);


	list_add(listaDeWorkersAPlanificar, ubi0);
	list_add(listaDeWorkersAPlanificar, ubi1);
	list_add(listaDeWorkersAPlanificar, ubi2);
	list_add(listaDeWorkersAPlanificar, ubi3);
	list_add(listaDeWorkersAPlanificar, ubi4);
	list_add(listaDeWorkersAPlanificar, ubi5);
	list_add(listaDeWorkersAPlanificar, ubi6);


	logInfo("Se creo la lista de Workers a planificar , empieza planificacion");

	t_list* planificacionDelJOb = planificar(listaDeWorkersAPlanificar,parametrosFileSystem->algoritmo, parametrosFileSystem->disponibilidadBase, jobAEjecutar);

	logInfo("Se crea JOB Completo");
	JOBCompleto* jobCompleto = crearJobCompleto(jobAEjecutar,listaDeWorkersAPlanificar, planificacionDelJOb);

	logInfo("Actualizar Tabla Global");
	//probar//ingresarDatosATablaGlobal(jobCompleto);

	logInfo("Serializar respueta transformacion a YAMA");

	int tamanioRespuesta = (tamanioRespuestaTransformacionYAMA(planificacionDelJOb) +
			((sizeof(int)*2)*list_size(planificacionDelJOb)) + (2*list_size(planificacionDelJOb)));

	char* respuesta = serializarListaYAMA(planificacionDelJOb);

	logInfo("listo para enviar el tamanio de la respuesta es %i", tamanioRespuesta);

	mensajesEnviadosAMaster(SOL_TRANSFORMACION, jobCompleto->job->master,
			respuesta, tamanioRespuesta);

	logInfo("Se envio respuesta correctamaente");


	list_add(listaDeJobs, jobCompleto);

}

ParametrosComunicacionConFileSystem* setParametrosComunicacionConFileSystem(
		int puerto, char* ip, char* algoritmo, int disponiblidadBase,int retardo) {
	ParametrosComunicacionConFileSystem* parametros = malloc(
			sizeof(ParametrosComunicacionConFileSystem));
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

	switch (codigo) {
	case PRUEBAS:
		logInfo("RECIBIENDO Pruebas de serializacion");
				recv(FDsocketClienteFileSystem,pesoMensaje,4,0);
				tamanio = deserializarINT(pesoMensaje);
				logInfo("tamanio de lo que recibo %i", tamanio);
				mensaje = malloc(tamanio + 1);
				mensaje[tamanio] = '\0';
				if (recv(FDsocketClienteFileSystem, mensaje, tamanio, 0) == -1) {
					logInfo("Error en la recepcion pruebas de serializacion.");
				}
				logInfo("Recibiendo respuesta de la prueba");

				t_list* respuestaAlmacenamientoGlobal = deserializarListaRespuesReduccionGlobal(mensaje);

				while(i < list_size(respuestaAlmacenamientoGlobal)){
					RespuestaReduccionGlobal* respuesta = list_get(respuestaAlmacenamientoGlobal,i);
					logInfo("Agarro el nodo %i",i);
					logInfo("EL NODO ES %i",respuesta->nodo);
					logInfo("EL PUERTO ES %i",respuesta->puertoWorker);
					logInfo("EL IP ES %s",respuesta->ipWorker);
					logInfo("EL ARCHIVO DE REDUCCION LOCAL ES %s",respuesta->archivoReduccionLocal);
					logInfo("EL ARCHIVO DE REDUCCION global ES %s",respuesta->archivoReduccionGlobal);
					logInfo("ES EL NODO ENCARGADO %i",respuesta->encargado);
					logInfo("-------------------------------------");
					i++;
				}



		break;
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

	            	  lista_ubicaciones= deserializarUbicacionBloquesArchivos(mensaje);

					logInfo("se RECIBIO LA LISTA DE UBICACIONES CON %i PARTES DEL ARCHIVO", list_size(lista_ubicaciones));


	            	  for(i=0;list_size(lista_ubicaciones);i++){

	      	  	  	  UbicacionBloquesArchivo*ubicacionb = list_get(lista_ubicaciones,i);
	    	  	  	  logInfo("Se recibio la parte del archivo: %i", ubicacionb->parteDelArchivo);
	    	  	  	  logInfo("Se recibio el ubi 1 en nodo %i desplazamiento %i", ubicacionb->ubicacionCopia1.nodo, ubicacionb->ubicacionCopia1.desplazamiento);
	    	  	  	  logInfo("Se recibio el ubi 2 en nodo %i desplazamiento %i", ubicacionb->ubicacionCopia2.nodo, ubicacionb->ubicacionCopia2.desplazamiento);
	    	  	  	  logInfo("Con la cantidad de bytes Ocupados: %i", ubicacionb->bytesOcupados);
	      }

			sem_wait(&semaforoYAMA);
			Job* jobAEjecutar = retirarJobDeLista();



			logInfo("Se recibió de forma correcta la lista de bloques de archivo enviada por FS.",tamanio);
               break;


           	logInfo("Creando Planificacion de prueba");

           	t_list* listaDeWorkersAPlanificar = list_create();
           	UbicacionBloquesArchivo* ubi0 = crearUbicacionBloquesArchivos(0, 500, 1, 12, 2, 13);
           	UbicacionBloquesArchivo* ubi1 = crearUbicacionBloquesArchivos(1, 100, 1, 20, 3, 19);
           	UbicacionBloquesArchivo* ubi2 = crearUbicacionBloquesArchivos(2, 100, 2, 20, 3, 19);
           	UbicacionBloquesArchivo* ubi3 = crearUbicacionBloquesArchivos(3, 100, 1, 20, 2, 19);
           	UbicacionBloquesArchivo* ubi4 = crearUbicacionBloquesArchivos(4, 100, 1, 20, 3, 19);
           	UbicacionBloquesArchivo* ubi5 = crearUbicacionBloquesArchivos(5, 100, 2, 20, 3, 19);
           	UbicacionBloquesArchivo* ubi6 = crearUbicacionBloquesArchivos(6, 100, 1, 20, 2, 19);


           	list_add(listaDeWorkersAPlanificar, ubi0);
           	list_add(listaDeWorkersAPlanificar, ubi1);
           	list_add(listaDeWorkersAPlanificar, ubi2);
           	list_add(listaDeWorkersAPlanificar, ubi3);
           	list_add(listaDeWorkersAPlanificar, ubi4);
           	list_add(listaDeWorkersAPlanificar, ubi5);
           	list_add(listaDeWorkersAPlanificar, ubi6);


           	logInfo("Se creo la lista de Workers a planificar , empieza planificacion");

           	t_list* planificacionDelJOb = planificar(listaDeWorkersAPlanificar,parametrosFileSystem->algoritmo, parametrosFileSystem->disponibilidadBase, jobAEjecutar);

           	logInfo("Se crea JOB Completo");
           	JOBCompleto* jobCompleto = crearJobCompleto(jobAEjecutar,listaDeWorkersAPlanificar, planificacionDelJOb);

           	logInfo("Actualizar Tabla Global");
           	//probar//ingresarDatosATablaGlobal(jobCompleto);

           	logInfo("Serializar respueta transformacion a YAMA");

           	int tamanioRespuesta = (tamanioRespuestaTransformacionYAMA(planificacionDelJOb) +
           			((sizeof(int)*2)*list_size(planificacionDelJOb)) + (2*list_size(planificacionDelJOb)));

           	char* respuesta = serializarListaYAMA(planificacionDelJOb);

           	logInfo("listo para enviar el tamanio de la respuesta es %i", tamanioRespuesta);

           	mensajesEnviadosAMaster(SOL_TRANSFORMACION, jobCompleto->job->master,
           			respuesta, tamanioRespuesta);

           	logInfo("Se envio respuesta correctamaente");


           	list_add(listaDeJobs, jobCompleto);

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

}
