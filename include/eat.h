/**
 *	Julian Sovernigo
 *	0948924
 *	CIS*2750_W17
 *	gsoverni
 *	gsoverni@mail.uoguelph.ca
 *
 */

#ifndef __GSOVERNIGO_EAT_H__
#define __GSOVERNIGO_EAT_H__

/**
 *	init
 *	sets the diners up.
 *	IN:			numDiner		- the number of diners that will be eating.
 *				numMeals		- the number of meals to be eaten.
 *	OUT:		returns an integer on success.
 *	POST:		sets up mutexes and threads.
 *	ERROR:		none
 */
int init(int numDiners, int numMeals);

/**
 *	clearTable
 *	lets all the diners eat.
 *	IN:			void.
 *	OUT:		an integer succes/fail.
 *	POST:		all the threads will exist, run, then join. 
 *	ERROR:		MANY
 */
int clearTable();

/**
 *	eat
 *	makes a single philosopher eat.
 *	IN:			n		- the specific philospher that should eat.
 *	OUT:		a void* (NULL)
 *	POST:		a single philospher has eaten.
 *	ERROR:		deadlock... maybe.
 */
void* eat(void* n);

#endif
