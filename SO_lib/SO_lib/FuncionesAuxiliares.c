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
	nombreArchivo = "/home/utnso/tp-2017-2c-s1st3m4s_0p3r4t1v0s/Worker/Scripts";
	switch(codigo){
	case SCRIPT_REDUCCION:
		string_append(nombreArchivo,"Reduccion.py");
		break;
	case SCRIPT_TRANSFORMADOR:
		string_append(nombreArchivo,"Transformador.py");
		break;
	case SCRIPT_TRANSFORMADOR_ANUAL:
		string_append(nombreArchivo,"Transformador_Anual.py");
		break;
	case SCRIPT_TRANSFORMADOR_INICIAL:
		string_append(nombreArchivo,"Transformador_Inicial.py");
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

void ejecutarScript(char* rutaScript,char* rutaArchivo){
  int SIZE = 1024;
  int pipe_padreAHijo[2];
  int pipe_hijoAPadre[2];

  system("chmod +x script_transformacion.py");
  pipe(pipe_padreAHijo);
  pipe(pipe_hijoAPadre);
  pid_t pid;
  int status;
  char* buffer=malloc(SIZE);

  if ((pid=fork()) == 0 )
  {

  	dup2(pipe_padreAHijo[0],STDIN_FILENO);
  	dup2(pipe_hijoAPadre[1],STDOUT_FILENO);
   	close( pipe_padreAHijo[1] );
  	close( pipe_hijoAPadre[0] );
	close( pipe_hijoAPadre[1]);
	close( pipe_padreAHijo[0]);
      char *argv[] = {NULL};
      char *envp[] = {NULL};
      execve("./script_transformacion.py", argv, envp);
    	exit(1);
  }else{
	close( pipe_padreAHijo[0] ); //Lado de lectura de lo que el padre le pasa al hijo.
    	close( pipe_hijoAPadre[1] ); //Lado de escritura de lo que hijo le pasa al padre.

    	write( pipe_padreAHijo[1],"hola pepe",strlen("hola pepe"));

    	close( pipe_padreAHijo[1]);

    	waitpid(pid,&status,0);
  	read( pipe_hijoAPadre[0], buffer, SIZE );
    	close( pipe_hijoAPadre[0]);
  }
  FILE* fd = fopen("/home/utnso/tp-2017-2c-s1st3m4s_0p3r4t1v0s/SO_lib/SO_lib/resultado","w");
  fputs(buffer,fd);
  fclose(fd);

  free(buffer);

}
