/*
 * consola.c
 *
 *  Created on: 14/9/2017
 *      Author: utnso
 */
#include "Headers/consola.h"
#include "commons/string.h"
#include <stdio.h>

char** getComandos(){
	char* buffer = getStdinString();
	char** commands = string_split(buffer, " ");
	free(buffer);
	return commands;
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
			printf("Ingrese un comando: ");
			char** comandos = getComandos();
			compararComando=true;


			if(*comandos!=NULL && compararComando){
				if(string_equals_ignore_case(comandos[0], LS)){
					compararComando=false;
					printf("listar directorio \n");
					//hacer que liste un directorio

				}
	        }

			if(*comandos!=NULL && compararComando){
						if(string_equals_ignore_case(comandos[0], FORMAT)){
							compararComando=false;
							//hacer el comando
						}
			        }
			if(*comandos!=NULL && compararComando){
								if(string_equals_ignore_case(comandos[0], RM)){
									compararComando=false;
									//hacer el comando
								}
					        }
			if(*comandos!=NULL && compararComando){
								if(string_equals_ignore_case(comandos[0], RENAME)){
									compararComando=false;
									//hacer el comando
								}
					        }

			if(*comandos!=NULL && compararComando){
								if(string_equals_ignore_case(comandos[0], MV)){
									compararComando=false;
									//hacer el comando
								}
					        }

			if(*comandos!=NULL && compararComando){
								if(string_equals_ignore_case(comandos[0], CAT)){
									compararComando=false;
									//hacer el comando
								}
					        }

			if(*comandos!=NULL && compararComando){
								if(string_equals_ignore_case(comandos[0], MKDIR)){
									compararComando=false;
									//hacer el comando
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
												//hacer el comando
											}
			                             }

		}
	}

