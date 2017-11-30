/*
 * ComunicacionConMaster.c
 *
 *  Created on: 16/9/2017
 *      Author: utnso
 */
#include "Headers/ComunicacionConMaster.h"
#include "Headers/logYAMA.h"

void comunicacionConMasters(ParametrosComunicacionConMaster* parametros) {

	//Creo Servidor Principal
	int socketYAMAServidor;
	socketYAMAServidor = lib_socketServidor(parametros->puerto);

	fd_set master;
	fd_set read_fds;
	int fd_max;
	int i;
	int FD_Cliente;
	int bytesRecibidos;
	char buffer[4];

	FD_SET(socketYAMAServidor, &master);
	fd_max = socketYAMAServidor;

	for (;;) {

		read_fds = master;
		if (select(fd_max + 1, &read_fds, NULL, NULL, NULL) == -1) {

			logInfo("Error en el select");
		}
		for (i = 0; i <= fd_max; i++) {

			if (FD_ISSET(i, &read_fds)) {

				if (i == socketYAMAServidor) {

					if ((FD_Cliente = lib_aceptarYRegistrarSocket(
							socketYAMAServidor)) == -1) {

						logInfo("Error en el aceept despues del select");

					} else {
						FD_SET(FD_Cliente, &master);
						if (FD_Cliente > fd_max) {
							fd_max = FD_Cliente;
						}
						logInfo(
								"Nueva conexion del socket cliente Master de FD: %i",
								FD_Cliente);
					}
				} else {

					//Recibo datos de algun cliente
					if ((bytesRecibidos = recv(i, buffer, 4, 0)) <= 0) {
						if (bytesRecibidos == 0) {
							logInfo("Conexion cerrada del FD : %i", i);

						}
						close(i);
						FD_CLR(i, &master);

					} else {

						int codigo = deserializarINT(buffer);
						logInfo("Recibi de Master: %i", codigo);
						mensajesRecibidosDeMaster(codigo, i);
					}
				}

			}
		}

	}
}

ParametrosComunicacionConMaster* setParametrosComunicacionConMaster(int puerto) {
	ParametrosComunicacionConMaster* parametros = malloc(
			sizeof(ParametrosComunicacionConMaster));
	parametros->puerto = puerto;
	return parametros;
}

void mensajesEnviadosAMaster(int codigo, int FDMaster,char* mensaje,int tamanio) {
	switch (codigo) {
	case SOL_TRANSFORMACION:
		logInfo("YAMA envia a Master solicitud de transformación.");

		Paquete* paqueteSolicitudTransf = crearPaquete(codigo,
				tamanio, mensaje);

		if (enviarPaquete(FDMaster, paqueteSolicitudTransf) == -1) {
			logInfo("Error en envio de solicitud de transformación a MASTER");
		}

		destruirPaquete(paqueteSolicitudTransf);

		// Yama envia solicitud de transformacion YAMA , serializando esa lista

		break;
	case SOL_REDUCCION_LOCAL:
		logInfo("YAMA envia a Master solicitud de Reducción Local.");

		break;
	case SOL_REDUCCION_GLOBAL:
		logInfo("YAMA envia a Master solicitud de Reducción Global");


		break;
	case SOL_ALMACENADO_FINAL:
		logInfo("YAMA envia a Master solicitud de Almacenado Final");
		break;

	}
}
void mensajesRecibidosDeMaster(int codigo, int FDMaster) {

	char pesoMensaje[4];
	int tamanio;
	char* mensaje;
    RespuestaReduccionLocal* RRL;
    int numeroDeJobFinalReduccionLocal;
    t_list* RRG;
    Replanificacion* parametrosReplanif;
    respuestaAlmacenadoFinal* RAF;
    t_list* nuevaPlanificacion;
    int jobQueFinalizo;

	switch (codigo) {
	case REPLANIFICACION:
		recv(FDMaster, pesoMensaje, 4, 0);
		tamanio = deserializarINT(pesoMensaje);
		logInfo("tamanio de lo que recibo %i", tamanio);
		mensaje = malloc(tamanio + 1);
		mensaje[tamanio] = '\0';
		recv(FDMaster, mensaje, tamanio, 0);

		parametrosReplanif = deserializarReplanificacion(mensaje);

		nuevaPlanificacion = replanificacion(parametrosReplanif,FDMaster);

		actualizarNodosCaidosReplanificacion(parametrosReplanif,FDMaster);

		//serializarRespuestaTransformacionYAMA(nuevaPlanificacion);

		//mensajesEnviadosAmaster();

		insertarNodosNuevosPlanificados(nuevaPlanificacion,FDMaster,parametrosReplanif->numeroDeJOb);

		break;
	case NOMBRE_ARCHIVO:
		recv(FDMaster, pesoMensaje, 4, 0);
		tamanio = deserializarINT(pesoMensaje);
		logInfo("tamanio de lo que recibo %i", tamanio);
		mensaje = malloc(tamanio + 1);
		mensaje[tamanio] = '\0';
		recv(FDMaster, mensaje, tamanio, 0);
		logInfo("Se recibio el nombre del archivo: %s de tamanio %i", mensaje,
				tamanio);


		Job* job = crearJOB(FDMaster,mensaje);

		//enviarAMasterElnumeroDejob

		agregarJObACola(job);
		//sem_post(&semaforoYAMA);

		atenderJOB();


		break;

	case FIN_TRANSFORMACION:
		logInfo("YAMA recibe señal de finalización de Transformación.");
		recv(FDMaster, pesoMensaje, 4, 0);
		tamanio = deserializarINT(pesoMensaje);
		logInfo("tamanio de lo que recibo %i", tamanio);
		mensaje = malloc(tamanio + 1);
		mensaje[tamanio] = '\0';

		recv(FDMaster, mensaje, tamanio, 0);

		finTransformacion* finTrans = deserializarFinTransformacion(mensaje);

		actualizarTablaDeEstados(finTrans->numeroDeJob,FDMaster,finTrans->nodo,2,"OK");

		RRL = respuestaReduccionLocal(finTrans,FDMaster);

		//serializarRespuestaReduccionLocal(RRL);
		//mensajesEnviadosAMaster();

		agregarEntradasReduccionLocal(finTrans,RRL,FDMaster);

		break;

	case FIN_REDUCCION_LOCAL:

		//recibe numero de job
		logInfo("YAMA recibe señal de finalización de Reducción Local");
		recv(FDMaster, pesoMensaje, 4, 0);
		tamanio = deserializarINT(pesoMensaje);
		logInfo("tamanio de lo que recibo %i", tamanio);
		mensaje = malloc(tamanio + 1);
		mensaje[tamanio] = '\0';
		recv(FDMaster, mensaje, tamanio, 0);
		numeroDeJobFinalReduccionLocal = deserializarINT(mensaje);

		actualizarTablaDeEstadosFinReduccionLocal(FDMaster,numeroDeJobFinalReduccionLocal);

		RRG = respuestaReduccionGlobal(numeroDeJobFinalReduccionLocal,FDMaster);

		//serializarRespuestaReduccionLocal(RRL);

		//mensajesEnviadosAMaster();

		crearEntradasReduccionGlobal(RRG,FDMaster,numeroDeJobFinalReduccionLocal);

		asignarNodoEncargadoAJobCompleto(RRG,FDMaster,numeroDeJobFinalReduccionLocal);
		actualizarCargaWorkerReduccionGlobal(RRG);

		break;

	case FIN_REDUCCION_GLOBAL:

		logInfo("YAMA recibe señal de finalización de Reducción Global");
		recv(FDMaster, pesoMensaje, 4, 0);
		tamanio = deserializarINT(pesoMensaje);
		logInfo("tamanio de lo que recibo %i", tamanio);
		mensaje = malloc(tamanio + 1);
		mensaje[tamanio] = '\0';
		recv(FDMaster, mensaje, tamanio, 0);

		finTransformacion* finRG = deserializarFinTransformacion(mensaje);

		actualizarTablaDeEstadosFinReduccionGlobal(finRG->numeroDeJob,FDMaster);

		RAF = RespuestaAlmacenadoFinal(finRG,FDMaster);

		//serializarRespuestaAlmacenadoFinal(RAF);
		//mensajesEnviadosAMaster
		crearEntradasAlmacenamientoFinal(RAF,finRG,FDMaster);




		break;
	case SOL_ALMACENADO_FINAL:
		logInfo("Yama recibe señal de finalización de Almacenamiento Final.");
		recv(FDMaster, pesoMensaje, 4, 0);
		tamanio = deserializarINT(pesoMensaje);
		logInfo("tamanio de lo que recibo %i", tamanio);
		mensaje = malloc(tamanio + 1);
		mensaje[tamanio] = '\0';
		recv(FDMaster, mensaje, tamanio, 0);
		break;
	case FINALIZACION_DE_JOB:

		logInfo("Yama recibe señal de finalización de JOB ");
		recv(FDMaster, pesoMensaje, 4, 0);
		tamanio = deserializarINT(pesoMensaje);
		logInfo("tamanio de lo que recibo %i", tamanio);
		mensaje = malloc(tamanio + 1);
		mensaje[tamanio] = '\0';
		recv(FDMaster, mensaje, tamanio, 0);

		jobQueFinalizo = deserializarINT(mensaje);

		logInfo("Actualizar estructuras por finalizacion de job ");
		actualizarEstructurasFinalizacionDeJOB(jobQueFinalizo, FDMaster);

		break;

	default:
		logInfo("YAMA recibe un señal que no sabe traducir.");
		break;
	}
}

RespuestaReduccionLocal* respuestaReduccionLocal(finTransformacion* fin,int master){

	int i =0;
	RespuestaReduccionLocal* RRL;
	t_list* listaArchivosTemporalesTrans;
	char* ArchivoreduccionLocal;

	while(i< list_size(tabla_estados)){
		t_reg* registro = list_get(tabla_estados,i);

		if(registro->job == fin->numeroDeJob && registro->nodo == fin->nodo && registro->master == master){

			listaArchivosTemporalesTrans = listaDeArchivosTemporalesTransformacion(registro->job,registro->master,registro->nodo);
			VariableReduccionLocal++;
			ArchivoreduccionLocal = generarNombreArchivoReduccionLocal(VariableReduccionLocal);
			Info_Workers* worker = list_get(list_info_workers,((fin->nodo)-1));
			RRL = crearRespuestaReduccionLocal(registro->nodo,worker->puerto,worker->ipWorker,listaArchivosTemporalesTrans,ArchivoreduccionLocal);

			return RRL;
		}else{

			i++;
		}

	}

	return NULL;
}

t_list* listaDeArchivosTemporalesTransformacion(int job,int master,int nodo){

	t_list* listaArchvosTemporales = list_create();
	int i = 0;
	char* transforamcion = "TRANSFORMACION";

	while(i < list_size(tabla_estados)){

		t_reg* registro = list_get(tabla_estados,i);

		if(registro->job == job && registro->master == master && registro->etapa == transforamcion && registro->nodo == nodo){

			list_add(listaArchvosTemporales,registro->arch_temp);

			i++;
		}else{

			i++;
		}
	}

	return listaArchvosTemporales;
}


//devuelve una lista de respuestas de trnasformaciones globales
t_list* respuestaReduccionGlobal(int numeroDeJob,int master){

	int i;
	t_list* listaRRG = list_create();
	int* nodoConMenorCarga;
	char* reduccionLocal = "REDUCCION LOCAL";

	while(i < list_size(tabla_estados)){

		t_reg* registro = list_get(tabla_estados,i);

		if(numeroDeJob == registro->job && master == registro->master && registro->etapa == reduccionLocal){

			nodoConMenorCarga = nodoConMenorCargaDeTrabajoParaReduccionGlobal(numeroDeJob,master);

			if(registro->nodo == nodoConMenorCarga){

				Info_Workers* info = list_get(ipYPuertoWoerkers,((registro->nodo)-1));
				variableReduciionGlobal++;
				char* archivoRRG = generarNombreArchivoReduccionGlobal(variableReduciionGlobal);

				RespuestaReduccionGlobal* RRGEncargado = crearRespuestaReduccionGlobal(registro->nodo,info->puerto,info->ipWorker
						,registro->arch_temp,archivoRRG,true);

				list_add(listaRRG,RRGEncargado);
			}else{

				Info_Workers* info = list_get(list_info_workers,((registro->nodo)-1));
				variableReduciionGlobal++;
				char* archivoRRGlobal = generarNombreArchivoReduccionGlobal(variableReduciionGlobal);

				RespuestaReduccionGlobal* RRGNOEncargado = crearRespuestaReduccionGlobal(registro->nodo,info->puerto,info->ipWorker
						,registro->arch_temp,archivoRRGlobal,false);

				list_add(listaRRG,RRGNOEncargado);

			}

			i++;
		}else{

			i++;
		}

	}

	return listaRRG;

}

int* nodoConMenorCargaDeTrabajoParaReduccionGlobal(int master,int job){

	t_list* nodosIntervinientes = list_create();
	int i = 0;
	int a = 0;
	int* nodoConMenorCarga;
	int carga1;
	int carga2;
	int* nodo1;
	int* nodo2;
	char* reduccionLocal = "REDUCCION LOCAL";

	while(i < list_size(tabla_estados)){

		t_reg* registroNodo = list_get(tabla_estados,i);
		if(registroNodo->job == job && registroNodo->master == master && registroNodo->etapa == reduccionLocal){

			list_add(nodosIntervinientes,&registroNodo->nodo);
			i++;

		}else{

			i++;
		}
	}

	while(a < (list_size(nodosIntervinientes)-1)){


		nodo1 = list_get(nodosIntervinientes,a);
		nodo2 = list_get(nodosIntervinientes,(a+1));

		carga1 = cargaDeTrabajoDelNodo(nodo1);
		carga2 = cargaDeTrabajoDelNodo(nodo2);

		if(carga1 <= carga2){

			nodoConMenorCarga = list_get(nodosIntervinientes,a);
			a++;
		}else{

			nodoConMenorCarga = list_get(nodosIntervinientes,(a+1));
			a++;
		}

	}

	list_destroy_and_destroy_elements(nodosIntervinientes,free);

	return nodoConMenorCarga;
}

void asignarNodoEncargadoAJobCompleto(t_list* RRG,int master, int job){

	int i = 0;
	while(i < list_size(RRG)){

		RespuestaReduccionGlobal* rrg = list_get(RRG,i);
		if(rrg->encargado == true){

			elNodoEncargadoEs(rrg->nodo,master,job);
			i++;
		}else{

			i++;
		}
	}
}


void elNodoEncargadoEs(int nodo,int master,int job){

	int i = 0;
	while(i < list_size(listaDeJobs)){

		JOBCompleto * jc = list_get(listaDeJobs,i);
		if(jc->job->identificadorJob == job && jc->job->master){
			JOBCompleto * jcModificar = list_remove(listaDeJobs,i);
			jcModificar->nodoEncargado = nodo;
		}
		i++;
	}
}

int cargaDeTrabajoDelNodo(int* nodo){

	int i = 0;
	while(i < list_size(listaDeWorkerTotales)){
		nodoParaPlanificar* n = list_get(listaDeWorkerTotales,i);
		if(n->nodo == nodo){

			return n->carga;
		}else{

			i++;
		}
	}
	return -1;
}

respuestaAlmacenadoFinal* RespuestaAlmacenadoFinal(finTransformacion* finRG,int master){

	int i = 0;
	respuestaAlmacenadoFinal* respuestaAF;
	char* reduccionGlobal = "REDUCCION GLOBAL";

	while(i < list_size(tabla_estados)){
		t_reg* registroAAnalizar = list_get(tabla_estados,i);

		if(registroAAnalizar->job == finRG->numeroDeJob && registroAAnalizar->etapa == reduccionGlobal && registroAAnalizar->master == master
			&& registroAAnalizar->nodo == finRG->nodo ){

			Info_Workers* info = list_get(list_info_workers,finRG->nodo);

			respuestaAF = crearRespuestaAlmacenadoFinal(registroAAnalizar->nodo,info->puerto,
					info->ipWorker,registroAAnalizar->arch_temp);

			return respuestaAF;
		}

		i++;
	}

	return NULL;
}

t_list* replanificacion(Replanificacion* replani,int master){

	t_list* nuevaPlanificacion;
	int i = 0;

	while(i< list_size(listaDeJobs)){

		JOBCompleto* job = list_get(listaDeJobs,i);
		if(job->job->master == master && job->job->identificadorJob == replani->numeroDeJOb){

		nuevaPlanificacion = crearNuevaPlanificacion(job->respuestaTransformacion,job->ubicacionDeLasPartesDelJOB,replani->nodoCaido);

		return nuevaPlanificacion;

		}else{

			i++;
		}

	}

	return NULL;

}

t_list* crearNuevaPlanificacion(t_list* respuestaTransformacion,t_list* ubicacionDeLosBloques,int nodoCaido){

	t_list* listaConNuevaRespuesta = list_create();
	int i = 0;
	RespuestaTransformacionYAMA* respuestaAAnalizar;

	while(i<list_size(respuestaTransformacion)){
	 respuestaAAnalizar= list_get(respuestaTransformacion,i);
		if(respuestaAAnalizar->nodo == nodoCaido){

			RespuestaTransformacionYAMA* respuestaAModificar = list_remove(respuestaTransformacion,i);
			UbicacionBloque* ubicacionNuevoBloque = otroNodoDondeEstaLaParte(ubicacionDeLosBloques,respuestaAModificar->nodo,respuestaAModificar->bloque);
			Info_Workers* info = list_get(list_info_workers,((ubicacionNuevoBloque->nodo)+1));

			RespuestaTransformacionYAMA* nuevaRespuesta = setRespuestaTransformacionYAMA(ubicacionNuevoBloque->nodo,info->puerto,info->ipWorker,
					ubicacionNuevoBloque->desplazamiento,respuestaAModificar->bytesOcupados,respuestaAModificar->archivoTemporal);

			list_add(listaConNuevaRespuesta,nuevaRespuesta);
			free(ubicacionNuevoBloque);
			i++;
		}
		else{

			RespuestaTransformacionYAMA* respuestaAModificar = list_remove(respuestaTransformacion,i);
			list_add(listaConNuevaRespuesta,respuestaAModificar);
			i++;
		}
	}

	return listaConNuevaRespuesta;

}

UbicacionBloque* otroNodoDondeEstaLaParte(t_list* ubicacionDeLosBloques,int nodo,int bloque){

	int i = 0;

	while(i < list_size(ubicacionDeLosBloques)){

		UbicacionBloquesArchivo* ubi = list_get(ubicacionDeLosBloques,i);

		UbicacionBloque* ubiBloque1 = malloc(sizeof(UbicacionBloque));
		UbicacionBloque* ubiBloque2= malloc(sizeof(UbicacionBloque));

		ubiBloque1->nodo = ubi->ubicacionCopia1.nodo;
		ubiBloque1->desplazamiento = ubi->ubicacionCopia1.desplazamiento;
		ubiBloque2->nodo = ubi->ubicacionCopia2.nodo;
		ubiBloque2->desplazamiento = ubi->ubicacionCopia2.desplazamiento;

		if(ubiBloque1->nodo == nodo && ubiBloque1->desplazamiento == bloque){

			free(ubiBloque1);
			return  ubiBloque2;
		}
		if(ubiBloque2->nodo == nodo && ubiBloque2->desplazamiento == bloque){

			free(ubiBloque2);
			return ubiBloque1;
		}

		free(ubiBloque1);
		free(ubiBloque2);

		i++;
	}
	return NULL;
}

//Esto va en serializacion.c

Replanificacion* deserializarReplanificacion(char* replanifSerializado){

	Replanificacion* replanif = malloc(sizeof(int)*2);
	int desplazamiento = 0;
	deserializarDato(&(replanif->numeroDeJOb), replanifSerializado, sizeof(int),&desplazamiento);
	deserializarDato(&(replanif->nodoCaido), replanifSerializado, sizeof(int),&desplazamiento);

	return replanif;

}

char* serializarReplanificacion(int numeroJob,int nodoCaido){

	char* replanifSerializado = malloc(sizeof(int)*2);

	int desplazamiento = 0;

	serializarDato(replanifSerializado, &(numeroJob), sizeof(int),&desplazamiento);
	serializarDato(replanifSerializado, &(nodoCaido), sizeof(int), &desplazamiento);

	return replanifSerializado;
}

void actualizarCargaWorkerReduccionGlobal(t_list* RRG){

	int nodoEncargado = nodoEncargadoDeLaReduccionGlobal(RRG);
	if(nodoEncargado != -1){
	int cantidadDeArchivosAEfectuarRG = list_size(RRG);
	int cantidadDeTrabajoAAumentar = (cantidadDeArchivosAEfectuarRG / 2);

	actualizarCargaANodo(nodoEncargado,cantidadDeTrabajoAAumentar);
	}
	else{

		logInfo("Error de encontrar el nodo para actualizar carga");
	}
}

int nodoEncargadoDeLaReduccionGlobal(t_list* RRG){

	int i = 0;
	while(i < list_size(RRG)){
		RespuestaReduccionGlobal* respuestaRG = list_get(RRG,i);
		if(respuestaRG->encargado == true){

			return respuestaRG->nodo;
		}
		else{

			i++;
		}
	}

	return -1;
}

int actualizarCargaANodo(int nodo, int cantidadDeCarga){

	int i = 0;

	while(i < list_size(listaDeWorkerTotales)){

		nodoParaPlanificar* nodo = list_get(listaDeWorkerTotales,i);
		if(nodo->nodo == nodo){

			nodoParaPlanificar* nodo = list_remove(listaDeWorkerTotales,i);
			nodo->carga += cantidadDeCarga;
			list_add_in_index(listaDeWorkerTotales,i,nodo);
			return 0;
		}else{
			i++;
		}
	}

	return -1;
}

void actualizarEstructurasFinalizacionDeJOB(int numeroDeJob, int master){

	actualizarAlmacenadoFinalOK(numeroDeJob,master);

	actualizarCargaDeTrabajoFinalizacionDeJOBYEliminarJOB(numeroDeJob,master);
}


void actualizarCargaDeTrabajoFinalizacionDeJOBYEliminarJOB(int numeroDeJob,int master){

	int i= 0;

	while(i< list_size(listaDeJobs)){
		JOBCompleto* jc = list_get(listaDeJobs,i);
		if(jc->job->identificadorJob == numeroDeJob && jc->job->master == master){

			actualizarCargaDeTrabajoWorkerPorFinalizacion(jc);
			JOBCompleto* jc = list_remove(listaDeJobs,i);

			//destruirJobCompleto(jc);
		}else{

			i++;
		}
	}
}

void actualizarCargaDeTrabajoWorkerPorFinalizacion(JOBCompleto* jc){

	int i = 0;
	while(i<list_size(listaDeWorkerTotales)){

		nodoParaPlanificar* nodo = list_get(listaDeWorkerTotales,i);
		if(estaNodoEnLaRespuestaDeTransformacion(nodo->nodo,jc->respuestaTransformacion)){

			if(nodo->nodo == jc->nodoEncargado){

			int cantidadAReducirPorSerEncargado = cantidadDeNodosEnLaRespuestaTransformacion(jc->respuestaTransformacion);
			int cantidadAReducir = cantidadDeTrasformacionesYReduccionesQueHace(nodo->nodo,jc->respuestaTransformacion);
			reducirCargaANodo(nodo->nodo,(cantidadAReducir + cantidadAReducirPorSerEncargado));

			}else{

			int cantidadAReducir = cantidadDeTrasformacionesYReduccionesQueHace(nodo->nodo,jc->respuestaTransformacion);
			reducirCargaANodo(nodo->nodo,cantidadAReducir);

			}
			i++;
		}else{
			i++;
		}
	}
}

bool estaNodoEnLaRespuestaDeTransformacion(int nodo,t_list* respuestaTransformacion){
	int i = 0;
	while(i < list_size(respuestaTransformacion)){
		RespuestaTransformacionYAMA* rty = list_get(respuestaTransformacion,i);
		if(rty->nodo == nodo){

			return true;
		}else{
			i++;	}
	}
	return false;
}

int cantidadDeTrasformacionesYReduccionesQueHace(int nodo,t_list* respuestaTransformacionYama){
	int i = 0;
	int cantidad = 0;
	while(i < list_size(respuestaTransformacionYama)){
		RespuestaTransformacionYAMA* rty = list_get(respuestaTransformacionYama,i);
		if(rty->nodo == nodo){

			cantidad += 1;
			i++;
		}else{
			i++;
		}
	}

	return cantidad;
}

int reducirCargaANodo(int nodo, int cantidadDeCarga){

	int i = 0;

	while(i < list_size(listaDeWorkerTotales)){

		nodoParaPlanificar* nodo = list_get(listaDeWorkerTotales,i);
		if(nodo->nodo == nodo){

			nodoParaPlanificar* nodo = list_remove(listaDeWorkerTotales,i);
			nodo->carga -= cantidadDeCarga;
			list_add_in_index(listaDeWorkerTotales,i,nodo);
			return 0;
		}else{
			i++;
		}
	}

	return -1;
}

int cantidadDeNodosEnLaRespuestaTransformacion(t_list* respuestaTransformacionYama){

	t_list* nodos = list_create();
	int i = 0;
	int cantidadDeNodos;
	while(i < list_size(respuestaTransformacionYama)){
		RespuestaTransformacionYAMA* rty = list_get(respuestaTransformacionYama,i);

		if(estaNodoEnlista(nodos,rty->nodo)){

			i++;
		}else{

			list_add(nodos,rty->nodo);
			i++;
		}
	}
	cantidadDeNodos = list_size(nodos);
	list_destroy_and_destroy_elements(nodos,free);
	return cantidadDeNodos;

}

bool estaNodoEnlista(t_list* lista, int nodo){

	if(list_is_empty(lista)){

		return false;
	}
	int i = 0;
	while(i<list_size(lista)){
		int a = list_get(lista,i);

		if(a == nodo){

			return true;
		}else{

			i++;
		}
	}
	return false;

}
