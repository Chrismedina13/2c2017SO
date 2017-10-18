/*
 * prubeas.c
 *
 *  Created on: 11/10/2017
 *      Author: utnso
 */

#include "Headers/pruebas.h"

int pasarAMemoria (const char* rutaDelArchivo, char* p){

        struct stat sb;
        off_t len;

        int fd;

        fd = open (rutaDelArchivo, O_RDONLY);
        if (fd == -1) {
                printf("Error al abrir archivo\n");
                return 1;
        }

        if (fstat (fd, &sb) == -1) {
                printf("Error al hacer stat\n");
                return 1;
        }

        if (!S_ISREG (sb.st_mode)) {
                printf ("No es un archivo regular\n");
                return 1;
        }

        p = (char *)mmap (0, sb.st_size, PROT_READ, MAP_SHARED, fd, 0);
        if (p == MAP_FAILED) {
                printf("Fallo el mapeo\n");
                return 1;
        }

        if (close (fd) == -1) {
                printf("Error al cerrar el archivo\n");
                return 1;
        }
	/* mostramos el archivo completo */
        printf ("%s\n", p);

        if (munmap (p, sb.st_size) == -1) {
                printf("Error al cerrar la proyeccion \n");
                return 1;
        }

        return 0;

}
