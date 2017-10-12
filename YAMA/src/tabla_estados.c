/*voy a crear una lista por cada master que se conecte*/
/*voy a tener que saber cuantos nodos hay en fs, tabla de nodos del fs*/
#include "Headers/tabla_estados.h"

void crear_tabla_estados(int master, int mensaje) {
	/*Recibirá del Filesystem la lista de bloques que lo componen,
	con la correspondiente ubicación de sus dos copias y espacio ocupado en el bloque
	e iniciará la etapa de Transformación.*/

	tabla_estados = list_create();
	t_reg *registro = malloc(sizeof(t_reg));
	registro->job = rand(); //cm:genero un numero aleatorio. es necesario si solo tengo un job or master?
	registro->master = master;
	registro->arch_temp = mensaje;


}
