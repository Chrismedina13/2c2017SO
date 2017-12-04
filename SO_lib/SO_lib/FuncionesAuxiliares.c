/*
 * FuncionesAuxiliares.c
 *
 *  Created on: 3/11/2017
 *      Author: utnso
 */

#include "FuncionesAuxiliares.h"



int generarNumeroAleatorioNoRepetido(){

	srand(time(NULL));
	int test = rand() %100;
	printf ("El numero aleatorio del JOB vale %d\n", test);

	return test;

}

char* generarNombreArchivoTransformacion(int variableTransformacion){

	char *unaPalabra = string_new();
	string_append(&unaPalabra, "/tmp/Transf-");
	string_append(&unaPalabra, string_itoa(variableTransformacion));
	return unaPalabra;
}

char* generarNombreArchivoReduccionLocal(int variableRL){

	char *unaPalabra = string_new();
	string_append(&unaPalabra, "/tmp/RL-");
	string_append(&unaPalabra, string_itoa(variableRL));
	return unaPalabra;

}
char* generarNombreArchivoReduccionGlobal(int variableRG){


	char *unaPalabra = string_new();
	string_append(&unaPalabra, "/tmp/RG-");
	string_append(&unaPalabra, string_itoa(variableRG));
	return unaPalabra;
}


char* obtenerPuntero(const char* rutaArchivo){

	struct stat sb;
	off_t len;
	char* p;
	int fd;

	fd = open (rutaArchivo, O_RDONLY);
	//fd = fopen(rutaArchivo,"r");
	if (fd == -1) {
			printf("Error al abrir archivo\n");
			exit(-1);
	}

	if (fstat (fd, &sb) == -1) {
			printf("Error al hacer stat\n");
			exit(-1);
	}

	if (!S_ISREG (sb.st_mode)) {
			printf ("No es un archivo regular\n");
			exit(-1);
	}

	p = (char *)mmap (0, sb.st_size, PROT_READ, MAP_SHARED, fd, 0);

	if (p == MAP_FAILED) {
			printf("Fallo el mapeo\n");
			exit(-1);
	}

	if (close (fd) == -1) {
			printf("Error al cerrar el archivo\n");
			exit(-1);
	}

	return p;
}

int tamanioEstructuraInfoWorker(Info_Workers* info){
	int tamanio = sizeof(int) + strlen(info->ipWorker);
	return tamanio;
}
int tamanioEstructurasListaWorkers(t_list* listaWorkers){
	int tamanioWorker;
	int tamanioListaTotal=0;
	int i;
	for(i=0;i<list_size(listaWorkers);i++){
		tamanioWorker = tamanioEstructuraInfoWorker(list_get(listaWorkers,i));
		tamanioListaTotal = tamanioListaTotal+tamanioWorker;
	}
	return tamanioListaTotal;
}
void setInfoWorker(Info_Workers* info, int puerto,char* ipWorker){
	info = malloc(sizeof(int) + strlen(ipWorker) * sizeof(char));
	info->puerto = puerto;
	info->ipWorker = ipWorker;
}

int tamanioEstructuraUbicacionBloquesArchivo(UbicacionBloquesArchivo* info){
	int tamanio = sizeof(int)*6;
	return tamanio;
}
int tamanioEstructurasListaUbicacionBloquesArchivo(t_list* listaUbicaciones){
	int tamanioUbicacion;
	int tamanioListaTotal=0;
	int i;
	for(i=0;i<list_size(listaUbicaciones);i++){
		tamanioUbicacion = tamanioEstructuraUbicacionBloquesArchivo(list_get(listaUbicaciones,i));
		tamanioListaTotal = tamanioListaTotal+tamanioUbicacion;
	}
	return tamanioListaTotal;
}

script* setScript(char* rutaScript){
	char* punteroAlContenidoDelScript = obtenerPuntero(rutaScript);
	script* script1 = malloc(strlen(rutaScript)+strlen(punteroAlContenidoDelScript));
	script1->nombre = rutaScript;
	script1->contenido = punteroAlContenidoDelScript;

	return script1;
}

void rearmar_script(script* script,int codigo){
	FILE* fd;
	char* nombreArchivo = string_new();
	char* contenidoDelBloque = string_new();
	string_append(&nombreArchivo,"/home/utnso/tp-2017-2c-s1st3m4s_0p3r4t1v0s/Worker/Scripts/");

	switch(codigo){
	case SCRIPT_REDUCCION:
		string_append(&nombreArchivo,"Reduccion.py");
		rutaScriptReduccion = nombreArchivo; //variable global
		break;
	case SCRIPT_TRANSFORMADOR:
		string_append(&nombreArchivo,"Transformador.py");
		rutaScriptTransformador = nombreArchivo; //variable global
		break;
	case SCRIPT_TRANSFORMADOR_ANUAL:
		string_append(&nombreArchivo,"Transformador_Anual.py");
		rutaScriptTransformadorAnual = nombreArchivo; //variable global
		break;
	case SCRIPT_TRANSFORMADOR_INICIAL:
		string_append(&nombreArchivo,"Transformador_Inicial.py");
		rutaScriptTransformadorInicial = nombreArchivo; //variable global
		break;
	default:
		logInfo("Error codigo incorrecto");
		break;
	}
	fd = fopen(nombreArchivo,"w");
	if (fd==NULL) {
		printf("Error al abrir el archivo.");
	}
	contenidoDelBloque = script->contenido;
	fputs(contenidoDelBloque,fd);
	fclose(fd);
}

void ejecutarScript(char* rutaScript,char* rutaArchivoAEjecutar,char* rutaArchivoAGuardar){

	char* command = string_new();
	//malloc(strlen(rutaScript)+strlen(rutaArchivoAEjecutar)+strlen(rutaArchivoAGuardar)+sizeof(char)*15);
	string_append(&command,"/bin/cat ");
	string_append(&command, rutaArchivoAEjecutar);
	string_append(&command, " | ");
	string_append(&command, rutaScript);
	string_append(&command, " > ");
	string_append(&command, rutaArchivoAGuardar);

    system(command);

    /*

    pid_t pid;
    int status;

     if ( (pid=fork()) == 0 )
    { //hijo

    	char *args[] = { "chmod","+x", "/home/utnso/ejemplo_correr-script_so/script_transformacion.py",0 };
    	execv("/bin/chmod", args);

    	// EJEMPLO DE EJECV
    	char *args[] = { "cat", "/home/utnso/SO-Nombres-Dataset/nombres.csv" ,"|", "/home/utnso/SO-Nombres-Dataset/transformador.py > /home/utnso/SO-Nombres-Dataset/CantidadPersonas.txt", NULL };
    	execv("/bin/cat", args);
    	    printf("Se ha producido un error al ejecutar execv.\n");

    	printf("Soy el hijo (%d, hijo de %d)\n", getpid(), getppid());
    }
    else
    { // padre
    	waitpid(pid, &status, 0);
        printf("Soy el padre (%d, hijo de %d)\n", getpid(),getppid());
    }
    */
}
