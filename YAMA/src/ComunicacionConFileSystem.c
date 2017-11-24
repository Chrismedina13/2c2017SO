#include "Headers/ComunicacionConFileSystem.h"
#include "Headers/logYAMA.h"
#include "SO_lib/estructuras.h"

void comunicacionConFileSystem(ParametrosComunicacionConFileSystem* param) {
	//socketClienteParaFileSystem
	Job* jobAEjecutar;
	char pesoCodigo[4];

	int FDsocketClienteFileSystem;
	FDsocketClienteFileSystem = lib_SocketCliente(param->ip, param->puerto);
	logInfo("SocketCliente = %d \n", FDsocketClienteFileSystem);
	char buffer[13];
	if (recv(FDsocketClienteFileSystem, buffer, 13, 0) != -1) {
		logInfo("Se recibio: %s", buffer);

	}

	sem_wait(&semaforoYAMA);
	jobAEjecutar = retirarJobDeLista();
	logInfo("el job a ejecutar es: %s", jobAEjecutar->nombreDelArchivo);

	// Pediria a FS los bloques del job y ejecutaria la planificacion

	int tamanioJOB = strlen(jobAEjecutar->nombreDelArchivo);

	logInfo("%i", tamanioJOB);

	Paquete* paqueteDeEnvioDeJOB = crearPaquete(NOMBRE_ARCHIVO, tamanioJOB,
			jobAEjecutar);

	if (enviarPaquete(FDsocketClienteFileSystem, paqueteDeEnvioDeJOB) == -1) {
		logInfo("Error en envio de job");
	}

	destruirPaquete(paqueteDeEnvioDeJOB);

	/* Aca tiene que hacer un recv de FS la lista de bloque que componen al archivo
	 la ubicacion de sus copias y espacio ocupado en el bloque. Empieza la planificacion
	 con esos datos

	 Recibiria
	 BLOQUE | copia  | copia 2 | bytes ocupados
	 */
	recv(FDsocketClienteFileSystem, pesoCodigo, 4, 0);
	int codigo = deserializarINT(pesoCodigo);

	logInfo("Recibi de FS: %i", codigo);

	mensajesRecibidosDeFS(codigo, FDsocketClienteFileSystem);

	/*int tamanioEstructura, tamanio;
	 char* estructura;
	 char* mensaje;


	 recv(FDsocketClienteFileSystem, estructura, tamanioEstructura, 0);
	 tamanio = deserializarINT(tamanioEstructura);

	 logInfo("Tamanio de lo que recibo %i", tamanio);

	 mensaje = malloc(tamanio + 1);

	 if (recv(FDsocketClienteFileSystem, estructura, tamanio, 0) == -1) {
	 logInfo(
	 "Error en la recepcion de la lista de Bloques que componen el archivo.");
	 } else {
	 t_list* listaDeWorkersAPlanificar = list_create();
	 //t_list* listaDeWorkersAPlanificar = deserializarLISTA(estructura);//FALTA HACER

	 }*/

	logInfo("Creando Planificacion de prueba");

	t_list* listaDeWorkersAPlanificar = list_create();
	UbicacionBloquesArchivo* ubi1 = crearUbicacionBloquesArchivos(0, 12, 1, 12,
			2, 13);
	UbicacionBloquesArchivo* ubi2 = crearUbicacionBloquesArchivos(1, 100, 3, 20,
			1, 19);
	UbicacionBloquesArchivo* ubi3 = crearUbicacionBloquesArchivos(2, 100, 2, 20,
			3, 19);
	list_add(listaDeWorkersAPlanificar, ubi1);
	list_add(listaDeWorkersAPlanificar, ubi2);
	list_add(listaDeWorkersAPlanificar, ubi3);

	logInfo("Se creo la lista de Workers a planificar , empieza planificacion");

	t_list* planificacionDelJOb = planificar(listaDeWorkersAPlanificar,
			param->algoritmo, param->disponibilidadBase, jobAEjecutar);

	/*en esta parte se serializaria el mensaje y se mandaria al master
	 * si esta todo ok en el envio se crea el jobCompleto , se guarda y actualiza la tabla global*/

	JOBCompleto* jobCompleto = crearJobCompleto(jobAEjecutar,
			listaDeWorkersAPlanificar, planificacionDelJOb);

	logInfo("Actualizar Tabla Global");
	ingresarDatosATablaGlobal(jobCompleto);

	/* //Se serializaria la lista serializarRespuestaTransformacionYAMA(); */

	char* respuesta = serializarListaRespuestaTransf(planificacionDelJOb);

	int tamanioRespuesta = list_size(planificacionDelJOb) * (sizeof(int) * 4 + sizeof(char*) * 2);

	mensajesEnviadosAMaster(SOL_TRANSFORMACION, jobCompleto->job->master,
			respuesta, tamanioRespuesta);

	list_add(listaDeJobs, jobCompleto);

}

ParametrosComunicacionConFileSystem* setParametrosComunicacionConFileSystem(
		int puerto, char* ip, char* algoritmo, int disponiblidadBase) {
	ParametrosComunicacionConFileSystem* parametros = malloc(
			sizeof(ParametrosComunicacionConFileSystem));
	parametros->ip = ip;
	parametros->puerto = puerto;
	parametros->algoritmo = algoritmo;
	parametros->disponibilidadBase = disponiblidadBase;
	return parametros;
}

void mensajesRecibidosDeFS(int codigo, int FDsocketClienteFileSystem) {
//Yama recibe de FILESYSTEM
	int tamanioEstructura, tamanio;
	char* mensaje; //en este caso el mensaje es la lista de bloques del archivo
	char pesoMensaje[4];
	t_list * lista_ubicaciones;
	int i;
		UbicacionBloquesArchivo * ubicacionb;


	switch (codigo) {
	case UBICACION_BLOQUES:

		            recv(FDsocketClienteFileSystem, pesoMensaje, 4, 0);
					tamanio = deserializarINT(pesoMensaje);
					logInfo("tamanio de lo que recibo %i", tamanio);
					mensaje = malloc(tamanio + 1);
					mensaje[tamanio] = '\0';

		if (recv(FDsocketClienteFileSystem, mensaje, tamanio, 0) == -1) {
			logInfo(
					"Error en la recepcion de la lista de Bloques que componen el archivo.");
		} else {
	            lista_ubicaciones=list_create();
	            lista_ubicaciones= deserializarUbicacionBloquesArchivos(mensaje);
	            //prueba para ver si llego bien la lista
	      for(i=0;list_size(lista_ubicaciones);i++){
	    	  ubicacionb = list_get(lista_ubicaciones,i);
	    	  logInfo("Se recibio la parte del archivo: %s", ubicacionb->parteDelArchivo);
	    	  logInfo("Se recibio q hay que guardar el nodo %i en el bloque %i", ubicacionb->ubicacionCopia1.nodo,
	    			  ubicacionb->ubicacionCopia1.desplazamiento);



	      }






			logInfo(
					"Se recibió de forma correcta la lista de bloques de archivo enviada por FS.",
					tamanio);
               break;
	case INFO_WORKER:
			recv(FDsocketClienteFileSystem, pesoMensaje, 4, 0);
			tamanio = deserializarINT(pesoMensaje);
			logInfo("tamanio de lo que recibo %i", tamanio);
			mensaje = malloc(tamanio + 1);
			mensaje[tamanio] = '\0';
			recv(FDsocketClienteFileSystem, mensaje, tamanio, 0);
			logInfo("Se recibio la lista de workers: %s ", mensaje);



			break;


		}

	}
}
