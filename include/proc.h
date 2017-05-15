/**
 *	Julian Sovernigo
 *	0948924
 *	CIS*2750_W17
 *	gsoverni
 *	gsoverni@mail.uoguelph.ca
 *
 */

#ifndef __GSOVERNIGO_PROC_H__
#define __GSOVERNIGO_PROC_H__

struct process
{
	char pid;
	int size;
	int inTime;
	int swaps;
};


/**
 *	makeProcess
 *	creates a new process that can store process information.
 *	IN:			pid			- the pid that represents this process struct.
 *				size		- the size, in megabytes of the process.
 *	OUT:		a new process struct.
 *	POST:		returns a process.
 *	ERROR:		malloc failes.
 */
struct process* makeProcess(char pid, int size);

#endif
