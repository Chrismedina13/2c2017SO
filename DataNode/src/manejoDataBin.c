
#include "Headers/manejoDataBin.h"

/*	FILE* fd;
			char* nombreArchivo = string_new();
			char* numeroArchivo = string_itoa(numeroDeArchivo); //paso el numero de archivo de int a char*
			string_append(&nombreArchivo, "/home/utnso/tp-2017-2c-s1st3m4s_0p3r4t1v0s/FileSystem/ArchivosADividir/bloque");
			string_append(&nombreArchivo, numeroArchivo); //Le agrego el numero de archivo al nombre
			string_append(&nombreArchivo, ".txt"); //le agrego la extencion de archivo al nombre
			fd = fopen(nombreArchivo,"w");
			if (fd==NULL) {
				printf("Error al abrir el archivo.");
			}
			string_append(&contenidoDelBloque,"\n"); //le agrego el salto de linea final
			fputs(contenidoDelBloque,fd);
			list_add(lista,nombreArchivo);
			fclose(fd);
			}

			*/

const int MB = 1024*1024;


	char * get_bloque(int nro_bloque){


			const char * rutaDelArchivo= "/home/utnso/tp-2017-2c-s1st3m4s_0p3r4t1v0s/data.txt";
		//	int tamanioBloque = strlen (1024*1024);

			char* p = obtenerPuntero(rutaDelArchivo);



			int tamanio = strlen(p);
			char* bloque = string_substring(p, nro_bloque* MB, MB);



			 if (munmap (p, tamanio) == -1) {
			                printf("Error al cerrar la proyeccion \n");
			 }


			// FILE* fd;
			// fd = fopen("/home/utnso/tp-2017-2c-s1st3m4s_0p3r4t1v0s/nombre","w");
			// 		if (fd==NULL) {
			// 			printf("Error al abrir el archivo.");

			 //		}
			// fputs( bloque, fd );

            return(bloque);


			}

	int set_bloque(char * contenido, int nro_bloque){


		const char * rutaDelArchivo= "/home/utnso/tp-2017-2c-s1st3m4s_0p3r4t1v0s/data2.txt";
	//	int tamanioBloque = strlen (1024*1024);


		FILE * fp = fopen(rutaDelArchivo, "r+");
		if (!fp) {
		  perror("Error al abrir el Archivo");
           return(0);
		}else{


		if(fseek(fp, nro_bloque *MB, SEEK_SET)==0){
			logInfo("Me ubico en el bloque %i", nro_bloque);

	//	opcion 1
	//fputs(contenido, fp);

			//opcion 2
		  //  fwrite(contenido, 1, strlen(contenido), ftell(fp));

			//opcion 3
		//	 fwrite(contenido, sizeof(char), sizeof(contenido), fp);

		    logInfo("Escribo en el bloque %i", nro_bloque);
		    return(0);

		}else{
       logInfo("Fallo set bloque en el bloque %i", nro_bloque);
	    return(1);
		}
	}
	}

	//	char* arch = string_new();
	//	fputs( bloque, fp );




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


	int tamanioArchivo(int fp){

		int tam;
		fseek(fp, 0L, SEEK_END);
		tam = ftell(fp);
		return(tam);

	}