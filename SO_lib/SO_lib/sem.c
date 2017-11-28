/*
 * sem.c
 *
 *  Created on: 27/11/2017
 *      Author: utnso
 */

#include "sem.h"
#include <semaphore.h>

Semaphore *make_semaphore(int value)
{
Semaphore *sem = malloc(sizeof(Semaphore));
int n = sem_init(sem, 0, value);
if (n != 0) perror("sem_init failed");
return sem;
}

void semaphore_wait(Semaphore *sem)
{
int n = sem_wait(sem);
if (n != 0) perror("sem_wait failed");
}

void semaphore_signal(Semaphore *sem)
{
int n = sem_post(sem);
if (n != 0) perror("sem_post failed");
}

