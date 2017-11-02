/*
 * recuperacionFS.c
 *
 *  Created on: 22/10/2017
 *      Author: utnso
 */
#include "Headers/recuperacionFS.h"
void recuperacionFS(){
	loginfo("LEVANTANDO FILESYSTEM");


	loginfo("LEVANTANDO TABLA DE DIRECTORIOS..");
	recuperarTablaDeDirectorios();

	loginfo("LEVANTANDO TABLA DE ARCHIVOS");
	recuperarTablasDeArchivos();


	loginfo("LEVANTANDO TABLA DE NODOS");
	recuperarTablaNodos();
}
