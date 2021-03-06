/*voy a crear una lista por cada master que se conecte*/
/*voy a tener que saber cuantos nodos hay en fs, tabla de nodos del fs*/
#include "Headers/tabla_estados.h"

void agregarReguistroATablaDeEstados(t_reg* reg){

	list_add(tabla_estados,reg);

}

// si es 1 aModificar es la etapa, si es 2 aModificar es el estado

void actualizarTablaDeEstados(int job,int master,int nodo,int aModificar,char* modificado){


	pthread_mutex_lock(&mutexTablaDeEStados);
	int a = 0;
	while( a < list_size(tabla_estados)){

		t_reg* registro = list_remove(tabla_estados,a);
		if(registro->job == job  && registro->nodo == nodo){

			if(aModificar == 1){

				registro->etapa = modificado;
				list_add_in_index(tabla_estados,a,registro);

			}else{

				registro->estado = modificado;
				list_add_in_index(tabla_estados,a,registro);

			}
			if(aModificar != 1 || aModificar != 2){

				logInfo("Flag no valido, no se actualizo la tabla de estados");
			}
		}
		a++;
	}

	pthread_mutex_unlock(&mutexTablaDeEStados);

}

t_reg* crearRegistroTablaGlobal(int job,int master,int nodo,int bloque,char* etapa,char* arch_temp,char* estado){

	t_reg* registro = malloc((sizeof(int)*4)+strlen(etapa)+strlen(arch_temp)+strlen(estado));
	registro->job= job;
	registro->master = master;
	registro->nodo = nodo;
	registro->bloque = bloque;
	registro->etapa = etapa;
	registro->estado = estado;
	registro->arch_temp = arch_temp;

	return registro;

}


void ingresarDatosATablaGlobal(JOBCompleto* jobCompleto){

	int i = 0;
	while(i<list_size(jobCompleto->respuestaTransformacion)){

		RespuestaTransformacionYAMA* respuesta = list_get(jobCompleto->respuestaTransformacion,i);

		t_reg* registro = crearRegistroTablaGlobal(jobCompleto->job->identificadorJob,jobCompleto->job->master,
				respuesta->nodo,respuesta->bloque,"TRANSFORMACION",respuesta->archivoTemporal,"EN PROCESO");


		pthread_mutex_lock(&mutexTablaDeEStados);

		agregarReguistroATablaDeEstados(registro);

		pthread_mutex_unlock(&mutexTablaDeEStados);

		i++;

	}
}


// si bloque es 0, Para la entradas de Reduccion Local no tiene bloque
void agregarEntradasReduccionLocal(finTransformacion* ft,RespuestaReduccionLocal* RRL,int numeroMaster){

	t_reg* registro = crearRegistroTablaGlobal(ft->numeroDeJob,numeroMaster,
			ft->nodo,0,"REDUCCION LOCAL",RRL->archivoReduccionLocal,"EN  PROCESO");

	pthread_mutex_lock(&mutexTablaDeEStados);

	agregarReguistroATablaDeEstados(registro);

	pthread_mutex_unlock(&mutexTablaDeEStados);


}


void actualizarTablaDeEstadosFinReduccionLocal(int master,int job){

	pthread_mutex_lock(&mutexTablaDeEStados);

	int i = 0;
	char* RL = "REDUCCION LOCAL";
	while(i < list_size(tabla_estados)){
		t_reg* registroACosultar = list_get(tabla_estados,i);
		if(registroACosultar->master == master && registroACosultar->job == job && registroACosultar->etapa == RL){
			t_reg* registroAModificar  = list_remove(tabla_estados,i);
			registroAModificar->estado = "OK";
			list_add_in_index(tabla_estados,i,registroAModificar);
		}else{

			i++;
		}
	}

	pthread_mutex_unlock(&mutexTablaDeEStados);

}

void crearEntradasReduccionGlobal(t_list* RRG,int master,int job){

	int i = 0;
	while(i<list_size(RRG)){
		RespuestaReduccionGlobal* respuesta = list_get(RRG,i);

		t_reg* registro = crearRegistroTablaGlobal(job,master,
				respuesta->nodo,0,"REDUCCION GLOBAL",respuesta->archivoReduccionGlobal,"EN  PROCESO");

		pthread_mutex_lock(&mutexTablaDeEStados);

		agregarReguistroATablaDeEstados(registro);

		pthread_mutex_unlock(&mutexTablaDeEStados);

		i++;
	}
}

void actualizarTablaDeEstadosFinReduccionGlobal(int job,int master){

	pthread_mutex_lock(&mutexTablaDeEStados);

	int i = 0;
	char* enProceso = "EN PROCESO";
	char* RG = "REDUCCION GLOBAL";
	while(i< list_size(tabla_estados)){

		t_reg* registroAAnalizar = list_get(tabla_estados,i);

		if(registroAAnalizar->master == master && registroAAnalizar->job == job &&
				registroAAnalizar->etapa == RG && registroAAnalizar->estado == enProceso){

			t_reg* registroAmodificar = list_remove(tabla_estados,i);
			registroAmodificar->estado = "OK";
			list_add_in_index(tabla_estados,i,registroAmodificar);
		}

	}

	pthread_mutex_unlock(&mutexTablaDeEStados);

}


void crearEntradasAlmacenamientoFinal(respuestaAlmacenadoFinal* RAF,finTransformacion* ft,int master){


	t_reg* registro = crearRegistroTablaGlobal(ft->numeroDeJob,master,
			RAF->nodo,0,"ALMACENADO FINAL",RAF->archivoDeReduccionGlobal,"EN  PROCESO");

	pthread_mutex_lock(&mutexTablaDeEStados);

	list_add(tabla_estados,registro);

	pthread_mutex_unlock(&mutexTablaDeEStados);


}


void actualizarNodosCaidosReplanificacion(Replanificacion* replanif, int master){
	int i = 0;

	pthread_mutex_lock(&mutexTablaDeEStados);

	while(i < list_size(tabla_estados)){

		t_reg* registro = list_get(tabla_estados,i);
		if(registro->job == replanif->numeroDeJOb && registro->master == master && registro->nodo == replanif->nodoCaido){

			t_reg* registroAModificar = list_remove(tabla_estados,i);
			registroAModificar->estado = "Error nodo caido";
			list_add_in_index(tabla_estados,i,registroAModificar);
			i++;
		}else{

			i++;
		}
	}

	pthread_mutex_unlock(&mutexTablaDeEStados);

}

void insertarNodosNuevosPlanificados(t_list* respuestaNuevaPlanificacion,int Master,int numeroDeJOB){


	pthread_mutex_lock(&mutexTablaDeEStados);

	int i = 0;
	while( i < list_size(respuestaNuevaPlanificacion)){
		RespuestaTransformacionYAMA * respuestaTY = list_get(respuestaNuevaPlanificacion,i);
		if(existeTransformacionEnTablaGlobal(respuestaTY)){

			i++;
		}else{

			t_reg* registroNuevo = crearRegistroTablaGlobal(numeroDeJOB,Master,respuestaTY->nodo,respuestaTY->bloque,"TRANSFORMACION",
					respuestaTY->archivoTemporal,"EN PROCESO");

			list_add(tabla_estados,registroNuevo);
			i++;
		}
	}

	pthread_mutex_unlock(&mutexTablaDeEStados);

}


bool existeTransformacionEnTablaGlobal(RespuestaTransformacionYAMA* respuesta){

	int i = 0;
	char* transformacion = "TRANSFORMACION";

	while(i < list_size(tabla_estados)){

		t_reg* registroDeLaTabla = list_get(tabla_estados,i);
		if(respuesta->nodo == registroDeLaTabla->nodo && respuesta->bloque == registroDeLaTabla->bloque &&
				respuesta->archivoTemporal == registroDeLaTabla->arch_temp && registroDeLaTabla->etapa == transformacion){

			return true;
		}else{

			i++;
		}
	}

	return false;
}

void actualizarAlmacenadoFinalOK(numeroDeJob,master){

	int i = 0;
	char* af = "ALMACENADO FINAL";
	char* ep = "EN PROCESO";

	pthread_mutex_lock(&mutexTablaDeEStados);

	while(i< list_size(tabla_estados)){
		t_reg* registro = list_get(tabla_estados,i);
		if(registro->job == numeroDeJob && registro->master == master && registro->etapa == af && registro->estado == ep){
			t_reg* registroM = list_remove(tabla_estados,i);

			registro->estado = "OK";

			list_add_in_index(tabla_estados,i,registroM);
			i++;
		}else{
			i++;
		}

	}
	pthread_mutex_unlock(&mutexTablaDeEStados);

}


void mostrarTabla(){

	pthread_mutex_lock(&mutexTablaDeEStados);
	int i = 0;
	while(i < list_size(tabla_estados)){
		t_reg* registro = list_get(tabla_estados,i);
		printf("----------------------------------------------------------------\n");
		printf("JOB: %i \n",registro->job);
		printf("MASTER: %i \n",registro->master);
		printf("NODO: %i \n",registro->nodo);
		printf("BLOQUE: %i \n",registro->bloque);
		printf("ETAPA: %s \n",registro->etapa);
		printf("ARCHIVO TEMPORAL: %s \n", registro->arch_temp);
		printf("ESTADO %s \n",registro->estado);

		i++;
	}

	pthread_mutex_unlock(&mutexTablaDeEStados);

}
