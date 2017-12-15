/*
 * manejoDeArchivos.c
 *
 *  Created on: 23/10/2017
 *      Author: utnso
 */
#include "Headers/manejoDeArchivos.h"
#include "Headers/FileSystem.h"


/*FUNCION PRINCIPAL*/

t_list* obtenerBloquesTextoBinario(const char* rutaDelArchivo){
	t_list* bloques = list_create();
	int indiceArchivo = pathToIndiceArchivo(rutaDelArchivo);
	if(string_ends_with(rutaDelArchivo,".txt")){
		bloques = obtenerBloquesTexto(rutaDelArchivo, indiceArchivo);
	}
	else if(string_ends_with(rutaDelArchivo,".bin")){
		bloques = obtenerBloquesBinarios(rutaDelArchivo);
	}
	return bloques;
}

t_list* obtenerBloquesBinarios(const char * rutaDelArchivo){


		char* p = obtenerPuntero(rutaDelArchivo);
		int tamanio = tamanioArchivoCerrado(rutaDelArchivo);

        //OBTENEMOS LA CANTIDAD DE BLOQUES DE 1MB QUE NECESITA
        int cantidadBloques = cantidadDeBloques(tamanio);

        //divido el archivo en archivos de 1MB
        t_list* lista = dividirArchivoBinario(p,cantidadBloques);

        //LIBERAMOS EL ESPACIO DE MEMORIA QUE OCUPABA EL ARCHIVO
        if (munmap (p, tamanio) == -1) {
                printf("Error al cerrar la proyeccion \n");
        }

        //DEBERIAMOS BORRAR EL ARCHIVO PRINCIPAL

        t_list* bloques = listaDeContenidos(lista);

        return bloques;

}

/*FUNCION PRINCIPAL*/

void * obtenerBloquesTexto(const char * rutaDelArchivo, int indiceArchivo){

		/*Recibe la ruta de un archivo en fs
		 *Devuleve una lista de char* con bloques de texto que partio
		 */

		int desplazamiento=0;

		char* p = obtenerPuntero(rutaDelArchivo);

		FILE * fp = fopen(rutaDelArchivo, "r");
		if (!fp) {
		  perror("Error al abrir el Archivo");

		}

		int tamanio = tamanioArchivo(fp);

		fclose(fp);

		tabla_de_archivos[indiceArchivo].bloques = list_create();
		char* bloque = malloc(MB);

		while (desplazamiento < tamanio) {

		bloque = dividirArchivoTextoMemoria(p,&desplazamiento,indiceArchivo);
		list_add(tabla_de_archivos[indiceArchivo].bloques, bloque);

		}

        //LIBERAMOS EL ESPACIO DE MEMORIA QUE OCUPABA EL ARCHIVO
        if (munmap (p, tamanio) == -1) {
                printf("Error al cerrar la proyeccion \n");
        }

        //DEBERIAMOS BORRAR EL ARCHIVO PRINCIPAL**********

        //t_list* bloques = listaDeContenidos(lista);


}


int cantidadDeBloques(int tamanio){
	int cantidad;
	if(tamanio%MB == 0) cantidad = tamanio/MB;
	else cantidad = (tamanio/MB)+1;
	return cantidad;
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

int tamanioArchivoCerrado(const char* rutaArchivo){
	FILE* fd;
	fd = fopen(rutaArchivo,"r");
	fseek(fd, 0L, SEEK_END);
	int tamanio = ftell(fd);
	fclose(fd);

	return tamanio;
}

t_list* dividirArchivoBinario(char*puntero,int cantidadDeBloques){
	int i =0;
	t_list* lista = list_create();
	for(i=0;i<cantidadDeBloques;i++){
		char *nombreArchivo = string_new();
		FILE* fd;
		char* numeroDeArchivo = string_itoa(i);
		string_append(&nombreArchivo, "/home/utnso/tp-2017-2c-s1st3m4s_0p3r4t1v0s/FileSystem/ArchivosADividir/bloque");
		string_append(&nombreArchivo, numeroDeArchivo); //Le agrego el numero de archivo al nombre
		string_append(&nombreArchivo, ".txt"); //le agrego la extencion de archivo al nombre
		fd = fopen(nombreArchivo,"w");
		if (fd==NULL) {
			printf("Error al abrir el archivo.");

		}
		char* bloque = string_substring(puntero, i*MB, MB);
		fputs( bloque, fd );
		list_add(lista,nombreArchivo);
		fclose(fd);
	}
	return lista;
}

char * dividirArchivoTextoMemoria(char* texto, int * desplazamiento, int indiceArchivo) {

	int tamArchivo = strlen(texto);

	int tamRestante = tamArchivo - *desplazamiento;

	char * buffer = string_new();

	int tamBuffer = 0;

	int count = 0;

	//Genero el bloque

	char * bloqueAGurdar = armarBloqueTamRestante(texto + *desplazamiento,
			tamRestante);
	int tamBloqueAGuardar = strlen(bloqueAGurdar);

	//Pregunto si es el ultimo bloque
	if ((tamRestante - tamBloqueAGuardar) != 0) {
		tamBloqueAGuardar++;
	}

	while (tamRestante > 0 && (tamBuffer + tamBloqueAGuardar) < MB) {
		//Guardo el bloque al buffer
		string_append(&buffer, bloqueAGurdar);

		//Pregunto si es el ultimo bloque
		if ((tamRestante - tamBloqueAGuardar) != 0) {
			string_append(&buffer, "\n\0");
		}

		tamRestante -= tamBloqueAGuardar;

		*desplazamiento = *desplazamiento + tamBloqueAGuardar;

		tamBuffer += tamBloqueAGuardar;

		free(bloqueAGurdar);

		//Genero el bloque
		bloqueAGurdar = armarBloqueTamRestante(texto + *desplazamiento, tamRestante);
		tamBloqueAGuardar = strlen(bloqueAGurdar);

		//Pregunto si es el ultimo bloque
		if ((tamRestante - tamBloqueAGuardar) != 0) {
			tamBloqueAGuardar++;
		}

	}
	free(bloqueAGurdar);

	return buffer;
}

char * armarBloqueTamRestante(void * texto, int tamRestante) {

	int desplazamiento = 0;
	int tamALeer = 0;

	if (string_starts_with(texto, "\n"))
		desplazamiento++;

	void * prueba = strchr(texto + desplazamiento, '\n');

	if (prueba == NULL) {
		tamALeer += tamRestante;
		desplazamiento = 0;
	} else {
		tamALeer += prueba - texto;
	}

	char * bloque = malloc(tamALeer + 1);
	char * finBloque = "\0";
	memcpy(bloque, texto + desplazamiento, tamALeer);
	memcpy(bloque + tamALeer, finBloque, 1);

	return bloque;
}

t_list* dividirArchivoTexto(char* p, int indiceDeArchivo){

	/*la lista que devuleve es una lista con las rutas de los archivos en los que lo divide
	 * del estilio .../FileSystem/ArchivosADividir/Bloque(numeroArchivo).(tipoDeDato)
	 */

	char** parrafos =  string_split(p, "\n");
	char* contenidoDelBloque = string_new(); //SE VA A IR GUARDANDO LOS PARRAFOS EN EL BLOQUE HASTA QUE PASE EL ESPACIO REQUERIDO
	int numeroDeArchivo = 0; //A LA HORA DE GUARDAR EN UN ARCHIVO SE INCREMENTA PARA EL SIGUIENTE ARCHIVO
	int i = 0; //NUMERO DE PARRAFO
	int k; // NUMERO DE PARRAFO SIGUIENTE (k = i+1);
	int longitudParrafo; //PARA VER EL TAMAÑO DEL PARRAFO
	int totalEspacioLlenado = 0; //PARA VERIFICAR QUE AL AGREGAR LOS PARRAFOS NO SUPEREN EL ESPACIO REQUERIDO
	t_list* lista = list_create();
	char* parrafo;

	while(parrafos[i]!='\0'){

		k=i+1;
		parrafo = parrafos[i];
		longitudParrafo = string_length(parrafo);
		int cortoArchivo=0;

		if(totalEspacioLlenado+longitudParrafo>MB){
			cortoArchivo=1;
		}

		if(cortoArchivo==0 && parrafos[k]!='\0'){ //verifico que no sobrepase el MB y que no sea el fin del archivo.
			totalEspacioLlenado += longitudParrafo; //sirve para verificar que no sobrepase el MB
			string_append(&contenidoDelBloque, parrafo);//agrego el parrafo al bloque
			string_append(&contenidoDelBloque,"\n");
			i++;
		}

		else{
			if(parrafos[k]=='\0') {
				string_append(&contenidoDelBloque, parrafo); //agrego el parrafo faltante al bloque
				//string_append(&contenidoDelBloque,"\n"); //le agrego el salto de linea final
				i++; //PARA QUE EL WHILE TERMINE
			}
			FILE* fd;
			char* nombreArchivo = string_new();
			char* numeroArchivo = string_itoa(numeroDeArchivo); //paso el numero de archivo de int a char*
			char* indiceArchivo = string_itoa(indiceDeArchivo);
			string_append(&nombreArchivo, "/home/utnso/tp-2017-2c-s1st3m4s_0p3r4t1v0s/FileSystem/ArchivosADividir/indiceArchivo");
			string_append(&nombreArchivo, indiceArchivo);
			string_append(&nombreArchivo, "-bloque");
			string_append(&nombreArchivo, numeroArchivo); //Le agrego el numero de archivo al nombre
			string_append(&nombreArchivo, ".txt"); //le agrego la extencion de archivo al nombre
			fd = fopen(nombreArchivo,"w");
			if (fd==NULL) {
				printf("Error al abrir el archivo.");
			}

			fputs(contenidoDelBloque,fd);
			list_add(lista,nombreArchivo);
			fclose(fd);



			contenidoDelBloque = '\0'; //vacio el contenido del bloque para poder volver a meterle parrafos
			totalEspacioLlenado=0; //reseteo el espacio que ocupan los parrafos
			numeroDeArchivo++; //sirve para que me escriba otro archivo.
			contenidoDelBloque = string_new();
		}
	}

    return lista;
}

char* obtenerContenidoDelArchivo(const char* rutaArchivo){

	/*Obtengo el contenido de un archivo, pasandolo a memoria y retornando un puntero a esa direccion
	 *Devuelve un puntero a el contenido del bloque
	 */

	char* contenido = obtenerPuntero(rutaArchivo);
	return contenido;
}

t_list* listaDeContenidos(t_list* rutasDeArchivos){
	t_list* listaContenidos = list_create();
	listaContenidos = list_map(rutasDeArchivos,obtenerContenidoDelArchivo);
	return listaContenidos;
}

void mostrarBloque(t_list* listaDeBloques,int index){
	char* contenido = list_get(listaDeBloques,index);
	printf("El contenido del bloque %d es:\n\n\n%s",index,contenido);
}

/*
int cpfrom(char* ruta, char* rutaLocal){


						int status=0;

						int indiceArchivo = newArchivo();

						//reviso si hay lugar en mis nodos

						FILE * fp = fopen(comandos[1], "r");
						if (!fp) {
						  perror("Error al abrir el Archivo");
						  status = -1;
						}

						int desplazamiento = 0;
						int tamArchivo = tamanioArchivo(fp);
						int cantBloques = 0;

						int bloquesNecesarios = 10;//hacer bien, no importa mucho ahora porque hay lugar

						int bloquesLibres = tabla_de_nodos.bloqueslibres;
						//logInfo("%d", tabla_de_nodos.bloqueslibres);

						if(bloquesNecesarios>bloquesLibres){//necesita guardar una copia por cada bloque
							logInfo("No hay lugar suficiente para almacenar el archivo y sus copias");
							status = -1;
						}

						while (desplazamiento < tamArchivo  && status == 0) {

							//saco el bloque del archivo

							char* bloque = obtenerBloqueDeTexto(fp, indiceArchivo, desplazamiento);

							cantBloques++;

							//elijo los nodos para guardarlo

							distribuirBloque(bloque, indiceArchivo);

							int count = 0;
							UbicacionBloquesArchivo2* ubicacion;

							ubicacion = list_get(tabla_de_archivos[indiceArchivo].ubicaciones,count);
							int tamanioSetBloque= (strlen(bloque) + sizeof(int)*3);


							//envio las copias a los nodos

							//copia 1

							char* contenidoSerializado = serializarContenidoDelBloque(bloque);
							char* desplazamiento = serializeInt(ubicacion->desplazamiento1);

							int fileDescriptor1=nodoToFD(ubicacion->nodo1);

							logInfo("voy a mandar a este FileDescriptor %d un mensaje de tamaño %d", fileDescriptor1,tamanioSetBloque);

							mensajesEnviadosADataNode(SET_BLOQUE, fileDescriptor1, contenidoSerializado,sizeof(int)+strlen(bloque));

							send(fileDescriptor1,desplazamiento,4,0);

							logInfo("Copia1 del bloque %d, esta en dataNode%d:desplazamiento%d", count, ubicacion->nodo1, ubicacion->desplazamiento1);


							//copia 2

							char* contenidoSerializado2 = serializarContenidoDelBloque(bloque);
							char* desplazamiento2 = serializeInt(ubicacion->desplazamiento1);

							int fileDescriptor2=nodoToFD(ubicacion->nodo2);

							logInfo("voy a mandar a este FileDescriptor %d un mensaje de tamaño %d", fileDescriptor2,tamanioSetBloque);

							mensajesEnviadosADataNode(SET_BLOQUE, fileDescriptor2, contenidoSerializado2, sizeof(int)+strlen(bloque));

							send(fileDescriptor1,desplazamiento2,4,0);

							logInfo("Copia2 del bloque %d, esta en dataNode%d:desplazamiento%d", count, ubicacion->nodo2, ubicacion->desplazamiento2);

							logInfo("Envio los bloques a sus respectivos nodos y desplazamiento.");

						}

						logInfo("Parti el archivo en %d bloques.", cantBloques);

						//crea registro del archivo en YAMAFS

						status = crearRegistroArchivo(comandos[1],comandos[2], indiceArchivo);
						if(status==1){
							logInfo("Registro de archivo creado correctamente.");

							if(cantidad_archivos==cantArchivos){
								semaphore_signal(SEMAFOROYAMA);
							}
						}
						if(status==-1){
							logInfo("Registro de archivo no pudo ser creado.");
						}

						fclose(fp);

					}


}
*/
char* obtenerBloqueDeTexto(int fp, int indiceArchivo, int desplazamiento){

	char* bloque = malloc(MB);

	bloque = dividirArchivoTextoMemoria(fp,&desplazamiento,indiceArchivo);
	return(bloque);

}
/*
char * dividirArchivoTextoMemoria2(int fp, int * desplazamiento, int indiceArchivo, int tamArchivo) {

	int tamRestante = tamArchivo - *desplazamiento;

	char * buffer = string_new();

	int tamBuffer = 0;

	//Genero el bloque

	char * bloqueAGurdar = armarBloqueTamRestante(texto + *desplazamiento,
			tamRestante);
	int tamBloqueAGuardar = strlen(bloqueAGurdar);

	//Pregunto si es el ultimo bloque
	if ((tamRestante - tamBloqueAGuardar) != 0) {
		tamBloqueAGuardar++;
	}

	while (tamRestante > 0 && (tamBuffer + tamBloqueAGuardar) < MB) {
		//Guardo el bloque al buffer
		string_append(&buffer, bloqueAGurdar);

		//Pregunto si es el ultimo bloque
		if ((tamRestante - tamBloqueAGuardar) != 0) {
			string_append(&buffer, "\n");
		}

		tamRestante -= tamBloqueAGuardar;

		*desplazamiento = *desplazamiento + tamBloqueAGuardar;

		tamBuffer += tamBloqueAGuardar;

		free(bloqueAGurdar);

		//Genero el bloque
		bloqueAGurdar = armarBloqueTamRestante(texto + *desplazamiento, tamRestante);
		tamBloqueAGuardar = strlen(bloqueAGurdar);

		//Pregunto si es el ultimo bloque
		if ((tamRestante - tamBloqueAGuardar) != 0) {
			tamBloqueAGuardar++;
		}

	}

	free(bloqueAGurdar);

	return buffer;
}
*/
