/**
 *	Julian Sovernigo
 *	0948924
 *	CIS*2750_W17
 *	gsoverni
 *	gsoverni@mail.uoguelph.ca
 *
 */

#include "proc.h"

#include <stdlib.h>


struct process* makeProcess(char pid, int size)
{
	struct process* newProc;

	newProc = malloc(sizeof(struct process));

	newProc->pid = pid;
	newProc->size = size;
	newProc->inTime = -1;
	newProc->swaps = 0;

	return newProc;
}
