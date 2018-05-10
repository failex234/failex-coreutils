/*
 * Part of Felix' Coreutils
 * 
 * sh
 *
 * This is a private project so I'm not thinking about
 * licensing this software suite. Maybe one day it will
 * be but why? There are already many coreutils implementations.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "failex-coreutils.h"

#define BUFSIZE 2048

void help( char *prgname );
char *getprgname( char *allargs );
char *getprgpath( char *prgname );

char *prgpath;
char *args;

int main( int argc, char **argv ) {

	while(1) {
		if (fgets(args, BUFSIZE, stdin) == -1) {
			fputs("Error reading input.\n", stderr);
			return 1;
		}
		//later: go through input and check for pipes or redirections
		//even later: check for logical ors and ands
		//get program name, search for it in PATH
		//assemble full path
		//fork
		//execv
		prgpath = getprgpath(getprgname(args));

		//getargs, fork...
		execv(prgpath, args);
	}
}


char *getprgname( char *allargs ) {
	int c;
	int i;
	while (c = allargs[i] != ' ') {
		i++;
	}

	
