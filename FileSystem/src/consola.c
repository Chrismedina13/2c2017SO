/*
 * consola.c
 *
 *  Created on: 14/9/2017
 *      Author: utnso
 */

#include "Headers/consola.h"
#include "Headers/FileSystem.h"
#include "SO_lib/estructuras.h"
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

int checkInput(char* input){

	/*checkea si input esta vacio o no
	 * devuelve 1 si esta vacio, -1 si tiene algo.
	 */

	if(string_is_empty(input)==true){
		printf("Ingrese un parametro.\n");
		return(1);
	}
	return(-1);

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

	Configuracion *config = leerArchivoDeConfiguracion(ARCHIVO_CONFIGURACION);
	int cantidad_archivos = config->cant_archivos;


		semaphore_wait(SEMAFORODN);

		bool compararComando;

		while(true){
			printf("Ingrese un comando: \n");
			char** comandos = getComandos();
			compararComando=true;

			//comienzan comandos consola FS

			if(*comandos!=NULL && compararComando){
				if(string_equals_ignore_case(comandos[0], LS)){
					compararComando=false;

					/*
					int status = checkInput(comandos[0]);
					if (status==1){
						printf("No fue especificado ningun directorio para mostrar archivos. Se van a mostrar los directorios disponibles");
					}
					*/

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

					inciarTablaDeArchivos();
					//cantArchivos = 0;
					cargarDirectorios();

					int resp = crearRegistroArchivoNodos(tabla_de_nodos); //escribe el registro de nodos nodos.bin (para recuperar fs y nodos anteriores)
						if(resp==0) printf("\nRegistro de Nodo cargado correctamente.\n");
						else printf("\nRegistro de Nodo no pudo ser cargado.\n");

				}
			}

			if(*comandos!=NULL && compararComando){
				if(string_equals_ignore_case(comandos[0], RM)){
					compararComando=false;

					/*rm [path_archivo] ó rm -d [path_directorio] ó rm -b [path_archivo] [nro_bloque] [nro_copia] -
					  Eliminar un Archivo/Directorio/Bloque. Si un directorio a eliminar no se encuentra vacío, la operación debe fallar.
					  Además, si el bloque a eliminar fuera la última copia del mismo, se deberá abortar la operación informando lo sucedido.
					 */

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

					if(string_equals_ignore_case(comandos[1], ARCH)){ //falta borrar los bloques de los dataNode



						//indicar al dataNode que borre tal bloque

						//list_remove(tabla_de_nodos.listaCapacidadNodos,indice);

						int status;
						status = eliminarArchivo(comandos[2]);
						if (status == 1){

							char* nombre = pathToFile(comandos[2]);
							int indice = pathToIndiceArchivo(comandos[2]);
							int count = 0;
							int cantidadBloques = list_size(tabla_de_archivos[indice].bloques);

							while(count<cantidadBloques){

								UbicacionBloquesArchivo* ubicacion = list_get(tabla_de_archivos[indice].ubicaciones,0);

								liberarBloqueBitMap(ubicacion->ubicacionCopia1.nodo,ubicacion->ubicacionCopia1.desplazamiento);
								liberarBloqueBitMap(ubicacion->ubicacionCopia2.nodo,ubicacion->ubicacionCopia2.desplazamiento);

								list_remove(tabla_de_archivos[indice].ubicaciones,0);
								count++;
							}

							strcpy(tabla_de_archivos[indice].nombre,"deleted");
							strcpy(tabla_de_archivos[indice].tipo,"deleted");


							cantArchivos--;

							logInfo("Registro de archivo eliminado correctamente.");
						}
						else{
							logInfo("Archivo no existe. No pudo ser eliminado");
						}


					}

					//bloque

					if(string_equals_ignore_case(comandos[1], BLOQ)){

						int count = 0;
						int status;
						int bloqueArchivo = atoi(comandos[3]);
						int numeroCopia = atoi(comandos[4]);
						int indice = pathToIndiceArchivo(comandos[2]);
						int cantBloques = list_size(tabla_de_archivos[indice].bloques);
						UbicacionBloquesArchivo* bloques;

						while(count<cantBloques){
							bloques = list_get(tabla_de_archivos[indice].ubicaciones,count);
							if(bloques->parteDelArchivo == bloqueArchivo){
								break;
							}
							count++;
						}

						status = ultimaCopia(indice,bloqueArchivo);
						if(status==1){
							if(numeroCopia == 1){
								liberarBloqueBitMap(bloques->ubicacionCopia1.nodo,bloques->ubicacionCopia1.desplazamiento);
								bloques->ubicacionCopia1.nodo = -1;
								bloques->ubicacionCopia1.desplazamiento = -1;
							}
							if(numeroCopia == 2){
								liberarBloqueBitMap(bloques->ubicacionCopia2.nodo,bloques->ubicacionCopia2.desplazamiento);
								bloques->ubicacionCopia2.nodo = -1;
								bloques->ubicacionCopia2.desplazamiento = -1;
							}
							logInfo("El bloque fue eliminado correctamente.");
						}
						if(status==-1){
							logInfo("El bloque no puede ser eliminado. No existe una copia en otro dataNode conectado.");
						}


						//eliminarBloque(nodo, bloque);

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
					  perror("Error al abrir el Archivo");
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

					int status=0;

					int indiceArchivo = newArchivo();

					//reviso si hay lugar en mis nodos

					FILE * fp = fopen(comandos[1], "r");
					if (!fp) {
					  perror("Error al abrir el Archivo");
					  status = -1;
					}


					//parte el archivo en bloques

					t_list* bloquesDeTexto = obtenerBloquesTexto(comandos[1], indiceArchivo); //quedan cargados en bloques

					int cantBloques = list_size(bloquesDeTexto);
					logInfo("Parti el archivo en %d bloques.", cantBloques);

					int bloquesNecesarios = cantBloques *2;

					int bloquesLibres = tabla_de_nodos.bloqueslibres;
					//logInfo("%d", tabla_de_nodos.bloqueslibres);

					if(bloquesNecesarios>bloquesLibres){//necesita guardar una copia por cada bloque
						logInfo("No hay lugar suficiente para almacenar el archivo y sus copias");
						status = -1;
					}

					//if(lugarEnNodos(bloquesArchivo)==-1){
					//	logInfo("La distribucion de los bloques en Nodos no permite almacenar el archivo");
					//	status = -1;
					//}



					if(status==0){

						//le pasa los bloques a los nodos

						distribuirBloques(bloquesDeTexto, indiceArchivo);
						logInfo("Distribui administrativamente los bloques en los Nodos conectados.");

						int count = 0;
						UbicacionBloquesArchivo2* ubicacion;
						char* bloque;

						while(count<cantBloques){


							bloque= list_get(bloquesDeTexto,count);
							ubicacion = list_get(tabla_de_archivos[indiceArchivo].ubicaciones,count);
							int tamanioSetBloque= (strlen(bloque) + sizeof(int)*3);


							//copia 1

						//	char* contenidoSerializado = serializarContenidoDelBloque(bloque);
							char* desplazamiento = serializeInt(ubicacion->desplazamiento1);

							int fileDescriptor1=nodoToFD(ubicacion->nodo1);

							logInfo("voy a mandar a este FileDescriptor %d un mensaje de tamaño %d", fileDescriptor1,tamanioSetBloque);

							mensajesEnviadosADataNode(SET_BLOQUE, fileDescriptor1, bloque,strlen(bloque));

							send(fileDescriptor1,desplazamiento,4,0);

							logInfo("Copia1 del bloque %d, esta en dataNode%d:desplazamiento%d", count, ubicacion->nodo1, ubicacion->desplazamiento1);



							//copia 2

						  //char* contenidoSerializado2 = serializarContenidoDelBloque(bloque);
							char* desplazamiento2 = serializeInt(ubicacion->desplazamiento1);

							int fileDescriptor2=nodoToFD(ubicacion->nodo2);

							logInfo("voy a mandar a este FileDescriptor %d un mensaje de tamaño %d", fileDescriptor2,tamanioSetBloque);

							mensajesEnviadosADataNode(SET_BLOQUE, fileDescriptor2, bloque, strlen(bloque));

							send(fileDescriptor1,desplazamiento2,4,0);

							logInfo("Copia2 del bloque %d, esta en dataNode%d:desplazamiento%d", count, ubicacion->nodo2, ubicacion->desplazamiento2);



							count++;

							//free(mensaje);
							//free(mensaje2);

						}



						logInfo("Envio los bloques a sus respectivos nodos y desplazamiento.");

						//crea registro del archivo en YAMAFS

						status = crearRegistroArchivo(comandos[1],comandos[2], tabla_de_archivos[indiceArchivo].ubicaciones, indiceArchivo);
						if(status==1){
							logInfo("Registro de archivo creado correctamente.");

							if(cantidad_archivos==cantArchivos){
								semaphore_signal(SEMAFOROYAMA);
							}
						}
						if(status==1){
							logInfo("Registro de archivo no pudo ser creado.");
						}


					}

					fclose(fp);
				}

			}

			if(*comandos!=NULL && compararComando){
				if(string_equals_ignore_case(comandos[0], CPTO)){
					compararComando=false;
					//hacer el comando

					//cpto [path_archivo_yamafs] [directorio_filesystem] - Copiar un archivo local desde el yamafs

					//buscar todas las partes

					//va pidiendo a los DN los bloques

					//va poniendo los char* por orden en un buffer

					//une todos los char*

					char* rutaYAMA = comandos[1];
					char* rutaLocal = comandos[2];
					int indexArchivo = pathToIndiceArchivo(rutaYAMA);

					char* bloque = malloc(1024*1024);
					UbicacionBloquesArchivo* ubicacion;

					int count = 0;
					int cantBloques = list_size(tabla_de_archivos[indexArchivo].bloques);

					FILE * fp = fopen(rutaLocal, "w+");
					if (!fp) {
					  perror("Error al crear el Archivo");
					}

					while(count<cantBloques){

						bloque = list_get(tabla_de_archivos[indexArchivo].bloques,count);
						fputs("%s", bloque);
						count++;
					}

					free(bloque);


				}
			}

			if(*comandos!=NULL && compararComando){
				if(string_equals_ignore_case(comandos[0], CPBLOCK)){
					compararComando=false;

					//empieza el comando

					int count = 0;
					int status;
					int indiceArchivo = pathToIndiceArchivo(comandos[1]);
					int parteArchivo = atoi(comandos[2]); //parte del archivo que voy a copiar
					int nodoACopiar = atoi(comandos[3]); //nodo donde lo voy a copiar
					int cantBloques = list_size(tabla_de_archivos[indiceArchivo].bloques);
					UbicacionBloquesArchivo* bloques;
					bloques_nodo* nodos;

					if(ultimaCopia(indiceArchivo, parteArchivo)==-1){

						while(count<cantBloques){
							bloques = list_get(tabla_de_archivos[indiceArchivo].ubicaciones,count);
							if(bloques->parteDelArchivo == parteArchivo){
								break;
							}
							count++;
						}


						int cantNodos = list_size(tabla_de_nodos.listaNodos);
						int desplazamiento;

						count = 0;

						while(count<cantNodos){
							nodos = list_get(tabla_de_nodos.listaCapacidadNodos,count);
							if(nodos->idNodo == nodoACopiar){
								desplazamiento = buscarBloqueVacio(nodos);
								break;
							}
							count++;
						}

						if(bloques->ubicacionCopia1.nodo==-1){
							bloques->ubicacionCopia1.nodo = nodoACopiar;
							bloques->ubicacionCopia1.desplazamiento = desplazamiento;
						}
						if(bloques->ubicacionCopia2.nodo==-1){
							bloques->ubicacionCopia2.nodo = nodoACopiar;
							bloques->ubicacionCopia2.desplazamiento = desplazamiento;
						}
						else{
							logInfo("La parte del Archivo ya posee 2 copias en distintos DN.");
						}

						char* bloque = list_get(tabla_de_archivos[indiceArchivo].bloques,count);

						SetBloque *setbloque= malloc(sizeof(char)*1024+sizeof(int));
						setbloque->nrobloque= desplazamiento;
						setbloque->contenidoBloque=bloque;
						char* mensaje= malloc(sizeof(int)+(sizeof(char)+strlen(setbloque->contenidoBloque)));
						mensaje = serializarBloque(setbloque->nrobloque,setbloque->contenidoBloque);
						int tamanioSetBloque= sizeof(int)+(sizeof(char)+strlen(setbloque->contenidoBloque));
						mensajesEnviadosADataNode(SET_BLOQUE, nodoACopiar, mensaje, tamanioSetBloque);

						logInfo("El bloque fue copiado con exito.");


					}
					if(ultimaCopia(indiceArchivo, parteArchivo)==1){
						logInfo("El bloque ya tiene 2 copias en dataNode distintos.");
					}
					else{
						logInfo("El bloque no pudo ser encontrado.");
					}
				}
			}

			if(*comandos!=NULL && compararComando){
				if(string_equals_ignore_case(comandos[0], MD5)){
					compararComando=false;

                   char* comand = string_new();
                  string_append(&comand, comandos[0]);
                  string_append(&comand," ");
                  string_append(&comand,comandos[1]);

                    system(comand);



					}



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

			//pruebas de comandos

			if(*comandos!=NULL && compararComando){
					if(string_equals_ignore_case(comandos[0], "prueba")){
					compararComando=false;

					int fileDescriptor1=nodoToFD(1);
					int fileDescriptor2=nodoToFD(2);

					logInfo("EL FD DEL NODO 1 es %d, EL FD DEL NODO 2 es %d",fileDescriptor1, fileDescriptor2);

				}
			 }


liberarComandos(comandos);


		}

	}



