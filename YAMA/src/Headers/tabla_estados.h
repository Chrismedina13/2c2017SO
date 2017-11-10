#include <commons/collections/list.h>
/*  Definicion de estructuras */

typedef struct {
	int job;
	int master;
	int nodo;
	int bloque;
	char etapa;
	char arch_temp;
	char estado;
} t_reg;

/*  Definicion de variables */
t_list *tabla_estados;
/*  Definicion de funciones */
void agregarReguistroATablaDeEstados(t_reg* reg);
void actualizarTablaDeEstados(int job,int master,int nodo,int bloque,int aModificar,char* modificado);


