/*
 * prueb.c
 *
 *  Created on: 25/10/2017
 *      Author: utnso
 */

#include "Headers/prueb.h"

void prueb(){

	t_list * list = list_create();

	list_add(list, persona_create("Matias"   , 24));
	list_add(list, persona_create("Ariel"   , 22));
	list_add(list, persona_create("l"   , 10));

	t_person* p1 = list_get(list,0);

	printf("El nombre de la persona es %s y su edad es %d\n\n", p1->name, p1->age);

	persona_destroy(p1);

	printf("El nombre de la persona es %s y su edad es %d\n\n", p1->name, p1->age);
}

static t_person *persona_create(char *name, unsigned int age) {
    t_person *new = malloc(sizeof(t_person));
    new->name = strdup(name);
    new->age = age;
    return new;
}

static void persona_destroy(t_person *self) {
    free(self->name);
    free(self);
}


