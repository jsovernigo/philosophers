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

/**
 *
 */
int main(int argc, char** argv)
{
	int philosophers;
	int numEats;

	if(argc < 3)
	{
		puts("Too few arguments to invoke the program.  Usage:\n\t./dine <philosophers> <eating times>");
		return 1;
	}
	else
	{
		philosophers = atoi(argv[1]);
		numEats = atoi(argv[2]);

		if(philosophers < 2 || numEats < 1 || numEats > 1000)
		{
			puts("Invalid argument. Usage:\n\t./dine <philosophers[2..)> <eatingTimes [1..1000]>");
			return 1;
		}
	}

	init(philosophers, numEats);

	clearTable();

	return 0;
}
