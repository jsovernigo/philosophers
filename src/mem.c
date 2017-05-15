/**
 *	Julian Sovernigo
 *	0948924
 *	CIS*2750_W17
 *	gsoverni
 *	gsoverni@mail.uoguelph.ca
 *
 */


#include "mem.h"
#include "proc.h"
#include "vector.h"

#include <stdio.h>
#include <stdlib.h>

char* initMem(int len)
{
	int i;
	char* memory;

	memory = malloc(sizeof(char) * len);

	/* pre clear the memory. */
	for(i = 0; i < len; i++)
	{
		memory[i] = '0';
	}

	return memory;
}


/* helps to print memory to the screen. */
int printmem(char* mem, int len)
{
	int i;

	for(i = 0; i < len; i++)
	{
		if(mem[i] != '0')
		{
			putc(mem[i], stdout);
		}
		else
		{
			putc('.', stdout);
		}
	}

	putc('\n', stdout);

	return 0;
}


int setMem(char* arr, int len, char pid, int start, int setlen)
{
	int i;

	if(arr == NULL)
	{
		return -1;
	}

	/* set the array to the appropriate pid. */
	for(i = start; i < setlen && i < len; i++)
	{
		arr[i] = pid;
	}

	return 0;
}


int clearMem(char* arr, int len)
{
	int result;
	result = setMem(arr, len, '0', 0, len);

	return result;
}


int getFreeMem(char* memory, int len, int start)
{
	int i;
	int freeSpace;

	/* if start is out of the bounds. */
	if(start > len || start < 0)
	{
		return -1;
	}

	/* if it isnt free memory in the first place... */
	if(memory[start] != '0')
	{
		return 0;
	}

	freeSpace = 0;
	
	/* count how many actual free cells exist post start. */
	for(i = start; i < len && memory[i] == '0'; i++)
	{
		freeSpace ++;
	}

	return freeSpace;
}


char swapProc(char* arr, int len, char pid)
{
	int i;
	int start;

	/* finds the first cell containing the pid we are looking for. */
	for(i = 0; i < len; i++)
	{
		if(arr[i] == pid)
		{
			start = i;
			break;
		}
	}

	/* clear the cells we are reading - the whole processes */
	for(i = start; i < len && arr[i] == pid; i++)
	{
		arr[i] = '0';
	}

	return start;
} 


struct vector* loadProcs(char* fname)
{
	char buffer[16];
	struct vector* procs;
	FILE* infile;

	infile = fopen(fname, "r");
	if(infile == NULL)
	{
		return NULL;
	}

	procs = makeVector(10);

	/* read to the end of the file */
	while(fgets(buffer, 15, infile) != NULL)
	{
		char c;
		int s;
		struct process* nproc;

		sscanf(buffer, "%c %d", &c, &s);
		nproc = makeProcess(c, s);

		append(procs, nproc);
	}

	return procs;
}


double usedMem(char* memory, int length)
{
	int i;
	int used;

	used = 0;

	/* counts how many cells are used. */
	for(i = 0; i < length; i++)
	{
		if(memory[i] != '0')
		{
			used++;
		}
	}

	/* cast to percent. */
	return (((double) used) / ((double) length)) * 100.00;
}


int getNumHoles(char* mem, int len)
{
	int i;
	int holes;

	holes = 0;

	/* count the number of transitions. */
	for(i = 1; i < len; i++)
	{
		if((mem[i - 1] == '0' && mem[i] != '0') || (mem[i] == '0' && i == len - 1))
		{
			holes ++;
		}
	}

	return holes;
}


struct process* getOldest(struct vector* procs)
{
	int i;
	int pos;
	struct process* oldest;

	pos = 0;

	/* itterate the vector. */
	for(i = 0; i < procs->length; i++)
	{
		/* -1 is not a placed process. */
		if(((struct process*)procs->contents[i])->inTime != -1)
		{
			/* check if we have found an older process. */
			if(((struct process*)procs->contents[i])->inTime < ((struct process*)procs->contents[pos])->inTime || ((struct process*)procs->contents[pos])->inTime == -1)
			{
				pos = i;
			}
		}
	}

	oldest = (struct process*) removeAt(procs, pos);

	return oldest;
}


int firstFit(char* fname, char* memory, int length)
{
	int time;
	int procs;
	int holes;
	int loads;
	int numProcs;
	int totalprocs;
	double cumulativeMem;
	
	struct vector* ready;
	struct vector* inmem;

	ready = loadProcs(fname);
	inmem = makeVector(ready->length);

	time = 0;
	procs = 0;
	holes = 0;
	loads = 0;
	numProcs = ready->length;
	totalprocs = 0;
	cumulativeMem = 0;

	/* while there is still stuff to be loaded. */
	while(ready->length > 0)
	{
		int i;
		int placed;
		struct process* inproc;

		inproc = (struct process*) pop(ready);

		placed = 0;
		while(!placed)
		{
			/* check for all possible holes */
			for(i = 0; i < length; i++)
			{
				if(getFreeMem(memory, length, i) >= inproc->size)
				{
					double memUse;

					setMem(memory, length, inproc->pid, i, i + inproc->size);
					inproc->inTime = time;
					loads ++;
					procs ++;
					totalprocs ++;
					placed = 1;
					holes += getNumHoles(memory, length);

					memUse = usedMem(memory, length);
					cumulativeMem *= ((double) loads - 1) / ((double) loads);
					cumulativeMem += memUse / ((double) loads);

					printf("%c loaded. Processes: %d, holes: %d, occupied Mem: %lf, cumulative mem: %lf\n", inproc->pid, procs, getNumHoles(memory, length), memUse, cumulativeMem);

					/* add to the list of items currently in memory. */
					append(inmem, inproc);
					break;
				}
			}

			/* we need to swap out a process */
			if(!placed)
			{
				struct process* oldest;

				oldest = getOldest(inmem);
				swapProc(memory, length, oldest->pid);
				oldest->swaps ++;
				procs --;

				if(oldest->swaps >= 3)
				{
					/* we don't need this process anymore; it is done. */
					free(oldest);
				}
				else
				{
					/* they are ready again. */
					append(ready, oldest);
				}
			}
		}
		time ++;
	}

	printf("Total loads: %d, average processes: %d, average holes: %d, cumulative mem: %lf\n\n", loads, totalprocs/numProcs, holes/loads, cumulativeMem);

	destroyVector(ready);
	destroyVector(inmem);

	clearMem(memory, length);

	return 0;
}


int bestFit(char* fname, char* memory, int length)
{
	int time;
	int procs;
	int holes;
	int loads;
	int numProcs;
	int totalprocs;
	double cumulativeMem;
	
	struct vector* ready;
	struct vector* inmem;

	ready = loadProcs(fname);
	inmem = makeVector(ready->length);

	time = 0;
	procs = 0;
	holes = 0;
	loads = 0;
	numProcs = ready->length;
	totalprocs = 0;
	cumulativeMem = 0;

	while(ready->length > 0)
	{
		int i;
		int placed;
		struct process* inproc;

		inproc = (struct process*) pop(ready);

		placed = 0;
		while(!placed)
		{
			int smallestPos;
			smallestPos = -1;
			i = 0;

			while(i < length)
			{
				int fmem;

				fmem = getFreeMem(memory, length, i);

				if((smallestPos == -1 || fmem < getFreeMem(memory, length, smallestPos)) && fmem >= inproc->size)
				{
					smallestPos = i;
					i += fmem;
				}
				else
				{
					i++;
				}	
			}

			if(getFreeMem(memory, length, smallestPos) < inproc->size)
			{
				struct process* oldest;

				oldest = getOldest(inmem);
				swapProc(memory, length, oldest->pid);
				oldest->swaps ++;
				procs --;

				if(oldest->swaps >= 3)
				{
					/* we don't need this process anymore; it is done. */
					free(oldest);
				}
				else
				{
					/* they are ready again. */
					append(ready, oldest);
				}
			}
			else
			{
				double memUse;

				setMem(memory, length, inproc->pid, smallestPos, smallestPos + inproc->size);
				inproc->inTime = time;
				loads ++;
				procs ++;
				totalprocs ++;
				placed = 1;

				holes += getNumHoles(memory, length);

				memUse = usedMem(memory, length);
				cumulativeMem *= ((double) loads - 1) / ((double) loads);
				cumulativeMem += memUse / ((double) loads);

				printf("%c loaded. Processes: %d, holes: %d, occupied Mem: %lf, cumulative mem: %lf\n", inproc->pid, procs, getNumHoles(memory, length), memUse, cumulativeMem);

				/* add to the list of items currently in memory. */
				append(inmem, inproc);
				break;
			}
		}
	}

	printf("Total loads: %d, average processes: %d, average holes: %d, cumulative mem: %lf\n\n", loads, totalprocs/numProcs, holes/loads, cumulativeMem);

	destroyVector(ready);
	destroyVector(inmem);

	clearMem(memory, length);

	return 0;
}


int nextFit(char* fname, char* memory, int length)
{
	int time;
	int procs;
	int holes;
	int loads;
	int numProcs;
	int totalprocs;
	
	int lastIndex;
	double cumulativeMem;
	
	struct vector* ready;
	struct vector* inmem;

	ready = loadProcs(fname);
	inmem = makeVector(ready->length);

	time = 0;
	procs = 0;
	holes = 0;
	loads = 0;
	lastIndex = 0;
	numProcs = ready->length;
	totalprocs = 0;
	cumulativeMem = 0;

	while(ready->length > 0)
	{
		int i;
		int placed;
		struct process* inproc;

		inproc = (struct process*) pop(ready);

		placed = 0;
		while(!placed)
		{
			/* check for all possible holes */
			for(i = lastIndex; i < length; i++)
			{
				if(getFreeMem(memory, length, i) >= inproc->size)
				{
					double memUse;

					setMem(memory, length, inproc->pid, i, i + inproc->size);
					inproc->inTime = time;
					loads ++;
					procs ++;
					totalprocs ++;
					placed = 1;
					holes += getNumHoles(memory, length);

					lastIndex = i;

					memUse = usedMem(memory, length);
					cumulativeMem *= ((double) loads - 1) / ((double) loads);
					cumulativeMem += memUse / ((double) loads);

					printf("%c loaded. Processes: %d, holes: %d, occupied Mem: %lf, cumulative mem: %lf\n", inproc->pid, procs, getNumHoles(memory, length), memUse, cumulativeMem);

					/* add to the list of items currently in memory. */
					append(inmem, inproc);
					break;
				}


				if(i >= length - 1)
				{
					lastIndex = 0;
				}	
			}



			/* we need to swap out a process */
			if(!placed)
			{
				struct process* oldest;

				oldest = getOldest(inmem);
				swapProc(memory, length, oldest->pid);
				oldest->swaps ++;
				procs --;

				if(oldest->swaps >= 3)
				{
					/* we don't need this process anymore; it is done. */
					free(oldest);
				}
				else
				{
					/* they are ready again. */
					append(ready, oldest);
				}
			}
		}
		time ++;
	}

	printf("Total loads: %d, average processes: %d, average holes: %d, cumulative mem: %lf\n\n", loads, totalprocs/numProcs, holes/loads, cumulativeMem);

	destroyVector(ready);
	destroyVector(inmem);

	clearMem(memory, length);

	return 0;
}


int worstFit(char* fname, char* memory, int length)
{
	int time;
	int procs;
	int holes;
	int loads;
	int numProcs;
	int totalprocs;
	double cumulativeMem;

	struct vector* ready;
	struct vector* inmem;

	ready = loadProcs(fname);
	inmem = makeVector(ready->length);

	time = 0;
	procs = 0;
	holes = 0;
	loads = 0;
	numProcs = ready->length;
	totalprocs = 0;
	cumulativeMem = 0;

	while(ready->length > 0)
	{
		int i;
		int placed;
		struct process* inproc;

		inproc = (struct process*) pop(ready);

		placed = 0;
		while(!placed)
		{
			int largestPos;
			largestPos = -1;
			i = 0;

			while(i < length)
			{
				int fmem;

				fmem = getFreeMem(memory, length, i);

				if((largestPos == -1 || fmem > getFreeMem(memory, length, largestPos)) && fmem >= inproc->size)
				{
					largestPos = i;
					i += fmem;
				}
				else
				{
					i++;
				}	
			}

			/* if we have identified a correct location. */
			if(largestPos < 0 || getFreeMem(memory, length, largestPos) < inproc->size)
			{
				struct process* oldest;

				oldest = getOldest(inmem);
				swapProc(memory, length, oldest->pid);
				oldest->swaps ++;
				procs --;

				if(oldest->swaps >= 3)
				{
					/* we don't need this process anymore; it is done. */
					free(oldest);
				}
				else
				{
					/* they are ready again. */
					append(ready, oldest);
				}
			}
			else
			{
				double memUse;

				setMem(memory, length, inproc->pid, largestPos, largestPos + inproc->size);
				inproc->inTime = time;
				loads ++;
				procs ++;
				totalprocs ++;
				placed = 1;

				holes += getNumHoles(memory, length);

				memUse = usedMem(memory, length);
				cumulativeMem *= ((double) loads - 1) / ((double) loads);
				cumulativeMem += memUse / ((double) loads);

				printf("%c loaded. Processes: %d, holes: %d, occupied Mem: %lf, cumulative mem: %lf\n", inproc->pid, procs, getNumHoles(memory, length), memUse, cumulativeMem);

				/* add to the list of items currently in memory. */
				append(inmem, inproc);
				break;
			}
		}
	}

	printf("Total loads: %d, average processes: %d, average holes: %d, cumulative mem: %lf\n\n", loads, totalprocs/numProcs, holes/loads, cumulativeMem);

	destroyVector(ready);
	destroyVector(inmem);

	clearMem(memory, length);

	return 0;
}


int manageProcs(char* memory, int len, char* fname)
{
	firstFit(fname, memory, len);
	bestFit(fname, memory, len);
	worstFit(fname, memory, len);
	nextFit(fname, memory, len);

	return 0;
}

/*
 * DDDD.....AAAA.....CC......BBBBBBB...
 */
