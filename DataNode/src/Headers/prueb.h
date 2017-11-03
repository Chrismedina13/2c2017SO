/*
 * prueb.h
 *
 *  Created on: 25/10/2017
 *      Author: utnso
 */

#ifndef HEADERS_PRUEB_H_
#define HEADERS_PRUEB_H_

#include <stdio.h>
#include <commons/collections/list.h>


typedef struct t_person {
    char *name;
    unsigned int age;
} t_person;

void funcion();
static t_person *persona_create(char *name, unsigned int age);
static void persona_destroy(t_person *self);


#endif /* HEADERS_PRUEB_H_ */
