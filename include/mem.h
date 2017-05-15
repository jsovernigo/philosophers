/**
 *	Julian Sovernigo
 *	0948924
 *	CIS*2750_W17
 *	gsoverni
 *	gsoverni@mail.uoguelph.ca
 *
 */

#ifndef __GSOVERNIGO_MEM_H__
#define __GSOVERNIGO_MEM_H__


/**
 *	initMem
 *	initializes the memory, setting it to zero.
 *	IN:			len			- the length of the memory
 *	OUT:		returns the char pointer memory.
 *	POST:		the memory has been malloced and returned.
 *	ERROR:		malloc fails
 */
char* initMem(int len);

/**
 *	setMem
 *	sets the memory specified by the params to the specified character.
 *	IN:			arr			- the memory
 *				len			- the length of the memory.
 *				pid			- the character to set the memory to.
 *				start		- the starting position.
 *				setlen		- the ending position (confusing I know.)
 *	OUT:		0 on success.
 *	POST:		the section specified in mem has been set to pid.
 *	ERROR:		like... alot.  malloc fails or any items are invalid.
 */
int setMem(char* arr, int len, char pid, int start, int setlen);

/**
 *	clearMem
 *	clears the mem, all the way to len.
 *	IN:			arr			- the memory to be cleared.
 *				len			- the length of the memory.
 *	OUT:		returns 0 on success.
 *	POST:		the mem has been set to 0.
 *	ERROR:		the memory is invalid or len extends past the end of arr.
 */
int clearMem(char* arr, int len);

/**
 *	getFreeMem
 *	downloads more RAM.  Actually checks how much memory is free ahead of
 *	the current position pointed to by start.
 *	IN:			memory		- the memory to be checked.
 *				len			- the length of the memory
 *				start		- the start of the memory to be checked.
 *	OUT:		returns the amount of free memory found past start.
 *	POST:		integer returned.
 *	ERROR:		memory is invalid.
 */
int getFreeMem(char* memory, int len, int start);

/**
 *	swapProc
 *	swaps out the process specified by pid.
 *	IN:			arr			- the memory
 *				len			- the length of that said memory.
 *				pid			- the process id we are looking for to remove.
 *	OUT:		returns the character that was swapped out.
 * 	POST:		arr has been cleared for all of pid.
 * 	ERROR:		arr is not in the memory.
 */
char swapProc(char* arr, int len, char pid);

/**
 *	manageProcs
 *	manages all the processes.
 *	IN:			memory		- the memory that we are loading.
 *				len			- the length of the memory.
 *				fname		- the file to be read.
 *	OUT:		0 on success.
 *	POST:		lots of stuff gets printed to the screen.
 *	ERROR:		none.
 */
int manageProcs(char* memory, int len, char* fname);


#endif
