/*
 * consola.c
 *
 *  Created on: 14/9/2017
 *      Author: utnso
 */

#include "Headers/consola.h"
#include "Headers/FileSystem.h"
#include <stdio.h>

char** getComandos(){
	char* buffer = getStdinString();
	char** commands = string_split(buffer, " ");
	free(buffer);
	return commands;
}


void liberarComandos(char** array) {
	int i = 0;
	while (array[i] != NULL) {
		free(array[i++]);
	}
	free(array);
}

char* getStdinString() {

	unsigned int maxlen = 16, size = 16;
	char* buffer2 = malloc(maxlen);
	int ch = EOF;
	int pos = 0;

	/* Read input one character at a time, resizing the buffer as necessary */
	while ((ch = getchar()) != '\n' && ch != EOF) {
		buffer2[pos++] = ch;
		/* Next character to be inserted needs more memory */
		if (pos == size) {
			size = pos + maxlen;
			buffer2 = (char*) realloc(buffer2, size);
		}
	}
	buffer2[pos] = '\0'; /* Null-terminate the completed string */

	return buffer2;
}

void consolaFileSystem(){
		bool compararComando;

		while(true){
			printf("Ingrese un comando: \n");
			char** comandos = getComandos();
			compararComando=true;

			//comienzan comandos consola FS

			if(*comandos!=NULL && compararComando){
				if(string_equals_ignore_case(comandos[0], LS)){
					compararComando=false;

					if (string_length(comandos[1])>0){

						DIR *d;
						struct dirent *dir;
						d = opendir(comandos[1]);
						if(d==NULL){
							logInfo("No se puede abrir el Directorio.");
						}
						if(d)
						{
						while ((dir = readdir(d)) != NULL){

						 if (dir->d_type == DT_REG)
						  {
							 printf("%s\n", dir->d_name);
						  }
						}

						closedir(d);
						}
					}

					else mostrarTablaDeDirectorios();


				}
	        }

			if(*comandos!=NULL && compararComando){
				if(string_equals_ignore_case(comandos[0], FORMAT)){
					compararComando=false;
					printf(" \n");

					cargarDirectorios();

				}
			}

			if(*comandos!=NULL && compararComando){
				if(string_equals_ignore_case(comandos[0], RM)){
					compararComando=false;

					//empieza comando

					//directorio

					if(string_equals_ignore_case(comandos[1], DIRE)){

						int directorio;
						int status;

						directorio = pathToIndex(comandos[2]);
						status = eliminarDirectorio(directorio);
						if (status == 1){
							logInfo("Directorio eliminado correctamente.");
						}
						if (status == -1){
							logInfo("Directorio contiene archivos dentro o no existe. No pudo ser eliminado");
						}

					}

					//archivo

					if(string_equals_ignore_case(comandos[1], ARCH)){

						int status;

						status = eliminarArchivo(comandos[2]);
						if (status == 1){
							logInfo("Archivo eliminado correctamente.");
						}
						if (status == -1){
							logInfo("Archivo no existe. No pudo ser eliminado");
						}

					}

					//nodo

					if(string_equals_ignore_case(comandos[1], BLOQ)){

					}

				}

			if(*comandos!=NULL && compararComando){
				if(string_equals_ignore_case(comandos[0], RENAME)){
					compararComando=false;

					//empieza comando

					//directorio

					if(string_equals_ignore_case(comandos[1], DIRE)){

						int status;
						int index;

						index = pathToIndex(comandos[2]);
						status = cambiarNombreDirectorio(index, comandos[3]);
						if(status == 1) {
							logInfo("Nombre de directorio cambiado correctamente.");
						}
						if(status == -1){
							logInfo("Directorio no existe. No pudo ser cambiado.");
						}

					}

					//archivo

					if(string_equals_ignore_case(comandos[1], ARCH)){

						int status;

						status = cambiarNombreArchivo(comandos[2], comandos[3]);
						if(status == 1) {
							logInfo("Nombre de archivo cambiado correctamente.");
						}
						if(status == -1){
							logInfo("Archivo no existe. No pudo ser cambiado.");
						}

					}

				}
			}

			if(*comandos!=NULL && compararComando){
				if(string_equals_ignore_case(comandos[0], MV)){
					compararComando=false;
					//hacer el comando

					//directorio

					if(string_equals_ignore_case(comandos[1], DIRE)){

						int status, index, padreNew;

						index = pathToIndex(comandos[2]);
						padreNew = pathToIndex(comandos[3]);

						status = moverDirectorio(index, padreNew);
						if(status == 1){
							logInfo("Directorio movido correctamente.");
						}
						if(status == -1){
							logInfo("Directorio no pudo ser movido.");
						}

					}

					if(string_equals_ignore_case(comandos[1], ARCH)){

						int status;

						status = moverArchivo(comandos[2], comandos[3]);
						if(status == 1) {
							logInfo("Archivo movido cambiado correctamente.");
						}
						if(status == -1){
							logInfo("Archivo no existe. No pudo ser movido.");
						}

					}
				}
			}

			if(*comandos!=NULL && compararComando){
				if(string_equals_ignore_case(comandos[0], CAT)){
					compararComando=false;

					//empieza comando

					int caracter;
					int status;

					status = 0;
					FILE * fp = fopen(comandos[1],"r");
					if (!fp) {
					  perror("Error al abrir el Archivo de directorios");
					  status = -1;
					}
					if (status == 0) {
						while ((caracter = getc(fp)) != EOF)
							putchar(caracter);
						fclose(fp);
					}
				}
			}

			if(*comandos!=NULL && compararComando){
				if(string_equals_ignore_case(comandos[0], MKDIR)){
					compararComando=false;

					int status;
					int padre;

					padre = atoi(comandos[3]);
					status = crearDirectorio(comandos[2],padre);
					if(status == 1){
						logInfo("Directorio creado correctamente.");
					}
					if(status == -1){
						logInfo("Directorio ya existe o cantidad maxima de directorios alcanzada. El directorio no fue creado.");
					}
				}
			}

			if(*comandos!=NULL && compararComando){
				if(string_equals_ignore_case(comandos[0], CPFROM)){
					compararComando=false;
					//hacer el comando

				}
			}

			if(*comandos!=NULL && compararComando){
				if(string_equals_ignore_case(comandos[0], CPTO)){
					compararComando=false;
					//hacer el comando
				}
			}

			if(*comandos!=NULL && compararComando){
				if(string_equals_ignore_case(comandos[0], CPBLOCK)){
					compararComando=false;
					//hacer el comando
				}
			}

			if(*comandos!=NULL && compararComando){
				if(string_equals_ignore_case(comandos[0], MD5)){
					compararComando=false;
					//hacer el comando
				}
			}

			if(*comandos!=NULL && compararComando){
				if(string_equals_ignore_case(comandos[0], INFO)){
					compararComando=false;

					int status;

					status = mostrarArchivo(comandos[1]);
					if(status == -1){
						logInfo("Archivo no existe o no pudo ser abierto.");
					}
				}
			 }

            if(*comandos!=NULL && compararComando){
				if(string_equals_ignore_case(comandos[0], HELP)){
					compararComando=false;
					printf(
							"format - Formatear el Filesystem. \n"
							"rm [path_archivo] ó rm -d [path_directorio] ó rm -b [path_archivo] [nro_bloque] [nro_copia]. \n"
							"rename [path_original] [nombre_final] - Renombra un Archivo o Directorio. \n"
							"mv [path_original] [path_final] - Mueve un Archivo o Directorio. \n"
							"cat [path_archivo] - Muestra el contenido del archivo como texto plano. \n"
							"mkdir [path_dir] - Crea un directorio. Si el directorio ya existe, el comando deberá informarlo. \n"
							"cpfrom [path_archivo_origen] [directorio_yamafs] - Copiar un archivo local al yamafs,siguiendo los lineamientos en la operaciòn Almacenar Archivo, de la Interfaz del FileSystem.\n"
							"cpto [path_archivo_yamafs] [directorio_filesystem] - Copiar un archivo local al yamafs. \n"
							"cpblock [path_archivo] [nro_bloque] [id_nodo] - Crea una copia de un bloque de un archivo en el nodo dado.\n"
							"md5 [path_archivo_yamafs] - Solicitar el MD5 de un archivo en yamafs. \n"
							"ls [path_directorio] - Lista los archivos de un directorio. \n"
							"info [path_archivo] - Muestra toda la información del archivo, incluyendo tamaño, bloques, ubicación de los bloques, etc. \n"
							"\n");

				}
			 }

			if (compararComando) {
				printf("«%s» no es un comando válido. Si necesita ayuda use: «%s» \n",
								comandos[0], HELP);
			}


liberarComandos(comandos);


		}

	}
}


