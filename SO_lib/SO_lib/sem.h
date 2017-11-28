/*
 * sem.h
 *
 *  Created on: 27/11/2017
 *      Author: utnso
 */

#ifndef SO_LIB_SEM_H_
#define SO_LIB_SEM_H_

#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

typedef sem_t Semaphore;
Semaphore *make_semaphore(int value);
void semaphore_wait(Semaphore *sem);
void semaphore_signal(Semaphore *sem);


#endif /* SO_LIB_SEM_H_ */
