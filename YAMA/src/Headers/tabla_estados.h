#include <commons/collections/list.h>
#include "SO_lib/estructuras.h"
#include "logYAMA.h"
/*  Definicion de estructuras */

typedef struct {
	int job;
	int master;
	int nodo;
	int bloque;
	char* etapa;
	char* arch_temp;
	char* estado;
} t_reg;

/*  Definicion de variables */
t_list *tabla_estados;
/*  Definicion de funciones */
void agregarReguistroATablaDeEstados(t_reg* reg);
void actualizarTablaDeEstados(int job,int master,int nodo,int aModificar,char* modificado);
t_reg* crearRegistroTablaGlobal(int job,int master,int nodo,int bloque,char* etapa,char* arch_temp,char* estado);
void ingresarDatosATablaGlobal(JOBCompleto* jobCompleto,t_list* listaRespuestaPlanificacionYama);
void agregarEntradasReduccionLocal(finTransformacion* ft,RespuestaReduccionLocal* RRL,int numeroMaster);
void actualizarTablaDeEstadosFinReduccionLocal(int master,int job);
void crearEntradasReduccionGlobal(t_list* RRG,int master,int job);
void actualizarTablaDeEstadosFinReduccionGlobal(int job,int master);
void crearEntradasAlmacenamientoFinal(respuestaAlmacenadoFinal* RAF,finTransformacion* ft,int master);
void actualizarNodosCaidosReplanificacion(Replanificacion* replanif, int master);
void insertarNodosNuevosPlanificados(t_list* respuestaNuevaPlanificacion,int Master,int numeroDeJOB);
bool existeTransformacionEnTablaGlobal(RespuestaTransformacionYAMA* respuesta);


