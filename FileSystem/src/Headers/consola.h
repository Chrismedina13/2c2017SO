/*
 * consola.h
 *
 *  Created on: 14/9/2017
 *      Author: utnso
 */

#ifndef HEADERS_CONSOLA_H_
#define HEADERS_CONSOLA_H_

#define LS "ls"
#define FORMAT "format"
#define RM "rm"
#define RENAME "rename"
#define MV "mv"
#define CAT "cat"
#define MKDIR "mkdir"
#define CPFROM "cpfrom"
#define CPTO "cpto"
#define CPBLOCK "cpblock"
#define MD5 "md5"
#define INFO "info"




void consolaFileSystem();
char** getComandos();
char* getStdinString();







#endif /* HEADERS_CONSOLA_H_ */
