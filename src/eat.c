/**
 *	Julian Sovernigo
 *	0948924
 *	CIS*2750_W17
 *	gsoverni
 *	gsoverni@mail.uoguelph.ca
 *
 */

#include "eat.h"

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>


/**
 *
 */
int dinerc;

pthread_t* diners;
pthread_mutex_t* forks;
int* meals;


int init(int numDiners, int numMeals)
{
	int i;

	dinerc = numDiners;
	
	diners = malloc(sizeof(pthread_t) * numDiners);
	if(diners == NULL)
	{
		return -1;
	}

	/* create the forks array. */
	forks = malloc(sizeof(pthread_mutex_t) * numDiners);
	if(forks == NULL)
	{
		free(diners);
		return -1;
	}

	/* initialize all the mutexes so they can be (un)?locked later. */
	for(i = 0; i < dinerc; i++)
	{
		pthread_mutex_init(&forks[i], NULL);
	}

	/* set all the associated meal values to the max. */
	meals = malloc(sizeof(int) * dinerc);
	for(i = 0; i < dinerc; i++)
	{
		meals[i] = numMeals;
	}

	return 0;	
}


int clearTable()
{
	int i;
	
	/* create all the threads */
	for(i = 0; i < dinerc; i++)
	{
		int* num;
		num = malloc(sizeof(int));
		*num = i;

		/* spawns a new thread */
		pthread_create(&diners[i], NULL, &eat, (void*) num);
	}

	/* rejoin all the threads, after they have finished. */
	for(i = 0; i < dinerc; i++)
	{
		pthread_join(diners[i], NULL);
	}
	
	for(i = 0; i < dinerc; i++)
	{
		pthread_mutex_destroy(&forks[i]);
	}

	return 0;
}


void* eat(void* num)
{
	int i;
	int n;

	n = *((int*) num);

	i = 0;

	printf("Philosopher %d is thinking.\n", n);

	/* lock the critical sections. */
	pthread_mutex_lock(&forks[n]);
	pthread_mutex_lock(&forks[(n + 1) % dinerc]);

	printf("Philosopher %d is eating meal %d.\n", n, meals[n]);
	meals[n] --;

	/* unlock the critical section. */
	pthread_mutex_unlock(&forks[n]);
	pthread_mutex_unlock(&forks[(n + 1) % dinerc]);

	/* if we still have meals left to eat. */
	if(meals[n] > 0)
	{
		while(1)
		{
			int isEqual;

			isEqual = 1;

			for(i = 0; i < dinerc; i++)
			{
				if(i != n && meals[n] < meals[i])
				{
					isEqual = 0;
					break;
				}
			}

			if(isEqual == 1)
			{
				break;
			}
		}

		/* eat the next meal. */
		eat(num);
	}
	else
	{
		free(num);
		printf("Philosopher %d is finished eating.\n", n);
	}

	return NULL;
}
