/*
 * manejoDeArchivos.c
 *
 *  Created on: 23/10/2017
 *      Author: utnso
 */
#include "Headers/manejoDeArchivos.h"


/*FUNCION PRINCIPAL*/

t_list* obtenerBloquesTextoBinario(const char* rutaDelArchivo){
	t_list* bloques = list_create();
	if(string_ends_with(rutaDelArchivo,".txt")){
		bloques = obtenerBloquesTexto(rutaDelArchivo);
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

t_list* obtenerBloquesTexto(const char * rutaDelArchivo){

		/*Recibe la ruta de un archivo en fs
		 *Devuleve una lista de char* con bloques de texto que partio
		 */

		char* p = obtenerPuntero(rutaDelArchivo);
		char* archivoNombre = pathToFile(rutaDelArchivo);

		FILE * fp = fopen(rutaDelArchivo, "r");
		if (!fp) {
		  perror("Error al abrir el Archivo");

		}

		int tamanio = tamanioArchivo(fp);

		fclose(fp);

		t_list* lista = dividirArchivoTexto(p, archivoNombre);

        //LIBERAMOS EL ESPACIO DE MEMORIA QUE OCUPABA EL ARCHIVO
        if (munmap (p, tamanio) == -1) {
                printf("Error al cerrar la proyeccion \n");
        }

        //DEBERIAMOS BORRAR EL ARCHIVO PRINCIPAL**********

        t_list* bloques = listaDeContenidos(lista);

        return bloques;

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

t_list* dividirArchivoTexto(char* p, char* archivoNombre){

	/*la lista que devuleve es una lista con las rutas de los archivos en los que lo divide
	 * del estilio .../FileSystem/ArchivosADividir/Bloque(numeroArchivo).(tipoDeDato)
	 */

	char** parrafos =  string_split(p, "\n");
	char* contenidoDelBloque = string_new(); //SE VA A IR GUARDANDO LOS PARRAFOS EN EL BLOQUE HASTA QUE PASE EL ESPACIO REQUERIDO
	//int numeroDeArchivo = 0; //A LA HORA DE GUARDAR EN UN ARCHIVO SE INCREMENTA PARA EL SIGUIENTE ARCHIVO
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
		totalEspacioLlenado += longitudParrafo; //sirve para verificar que no sobrepase el MB
		if(totalEspacioLlenado<=MB && parrafos[k]!='\0'){ //verifico que no sobrepase el MB y que no sea el fin del archivo.
			string_append(&contenidoDelBloque, parrafo);//agrego el parrafo al bloque
			string_append(&contenidoDelBloque,"\n");
			i++;
		}
		else{
			if(parrafos[k]=='\0') {
				string_append(&contenidoDelBloque, parrafo); //agrego el parrafo faltante al bloque
				i++; //PARA QUE EL WHILE TERMINE
			}
			/*FILE* fd;
			char* nombreArchivo = string_new();
			char* numeroArchivo = string_itoa(numeroDeArchivo); //paso el numero de archivo de int a char*
			string_append(&nombreArchivo, "/home/utnso/tp-2017-2c-s1st3m4s_0p3r4t1v0s/FileSystem/ArchivosADividir/");
			string_append(&nombreArchivo, archivoNombre);
			string_append(&nombreArchivo, ".bloque");
			string_append(&nombreArchivo, numeroArchivo); //Le agrego el numero de archivo al nombre
			string_append(&nombreArchivo, ".csv"); //le agrego la extencion de archivo al nombre
			fd = fopen(nombreArchivo,"w");
			if (fd==NULL) {
				printf("Error al abrir el archivo.");
			}
			string_append(&contenidoDelBloque,"\n"); //le agrego el salto de linea final
			fputs(contenidoDelBloque,fd);
			list_add(lista,nombreArchivo);
			fclose(fd);

			*/

			contenidoDelBloque = '\0'; //vacio el contenido del bloque para poder volver a meterle parrafos
			totalEspacioLlenado=0; //reseteo el espacio que ocupan los parrafos
			//numeroDeArchivo++; //sirve para que me escriba otro archivo.
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
