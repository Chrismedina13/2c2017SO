/*voy a crear una lista por cada master que se conecte*/
/*voy a tener que saber cuantos nodos hay en fs, tabla de nodos del fs*/
#include "Headers/tabla_estados.h"

void agregarReguistroATablaDeEstados(t_reg* reg){

	list_add(tabla_estados,reg);

}

// si es 1 aModificar es la etapa, si es 2 aModificar es el estado

void actualizarTablaDeEstados(int job,int master,int nodo,int bloque,int aModificar,char* modificado){

	int a = 0;
	while( a < list_size(tabla_estados)){

		t_reg* registro = list_remove(tabla_estados,a);
		if(registro->job == job && registro->master == master && registro->nodo == nodo && registro->bloque == bloque){

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
