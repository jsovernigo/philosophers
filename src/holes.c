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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char** argv)
{
	char* memory;
	char* fname;

	if(argc < 2)
	{
		puts("Invalid invokation. Usage:\n\t./holes <filename>");
		return 1;
	}
	else
	{
		fname = malloc(sizeof(char) * (strlen(argv[1]) + 1));
		strcpy(fname, argv[1]);
	}

	memory = initMem(128);

	manageProcs(memory, 128, fname);

	return 0;
}
