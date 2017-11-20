/*voy a crear una lista por cada master que se conecte*/
/*voy a tener que saber cuantos nodos hay en fs, tabla de nodos del fs*/
#include "Headers/tabla_estados.h"

void agregarReguistroATablaDeEstados(t_reg* reg){

	list_add(tabla_estados,reg);

}

// si es 1 aModificar es la etapa, si es 2 aModificar es el estado

void actualizarTablaDeEstados(int job,int master,int nodo,int aModificar,char* modificado){

	int a = 0;
	while( a < list_size(tabla_estados)){

		t_reg* registro = list_remove(tabla_estados,a);
		if(registro->job == job && registro->master == master && registro->nodo == nodo){

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
}

t_reg* crearRegistroTablaGlobal(int job,int master,int nodo,int bloque,char* etapa,char* arch_temp,char* estado){

	t_reg* registro = malloc((sizeof(int)*4)+strlen(etapa)+strlen(arch_temp)+strlen(estado));
	registro->job= job;
	registro->master = master;
	registro->job = nodo;
	registro->bloque = bloque;
	registro->etapa = arch_temp;
	registro->estado = estado;

	return registro;

}


void ingresarDatosATablaGlobal(JOBCompleto* jobCompleto){

	int i = 0;
	while(i<list_size(jobCompleto->respuestaDePlanificacion)){

		RespuestaTransformacionYAMA* respuesta = list_get(jobCompleto->respuestaDePlanificacion,i);

		t_reg* registro = crearRegistroTablaGlobal(jobCompleto->job->identificadorJob,jobCompleto->job->master,
				respuesta->nodo,respuesta->bloque,"TRANSFORMACION",respuesta->archivoTemporal,"EN PROCESO");

		agregarReguistroATablaDeEstados(registro);
		i++;

	}

}

// si bloque es 0, Para la entradas de Reduccion Local no tiene bloque
void agregarEntradasReduccionLocal(finTransformacion* ft,RespuestaReduccionLocal* RRL,int numeroMaster){

	t_reg* registro = crearRegistroTablaGlobal(ft->numeroDeJob,numeroMaster,
			ft->nodo,0,"REDUCCION LOCAL",RRL->archivoReduccionLocal,"EN  PROCESO");

	agregarReguistroATablaDeEstados(registro);
}
