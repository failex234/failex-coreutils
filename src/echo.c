/*
 * Part of Felix' Coreutils
 * 
 * echo
 *
 * This is a private project so I'm not thinking about
 * licensing this software suite. Maybe one day it will
 * be but why? There are already many coreutils implementations.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int nflag = 0;
int i = 1;

int main ( int argc, char **argv ) {


	if(argc > 1 && !strcmp("-n", argv[1])) {
		nflag = 1;
		i++;
	}

	while(i < argc) {
		printf("%s", argv[i]);

		if (i + 1 < argc) {
			putc(' ', stdout);
		}
		i++;
	}

	if (!nflag) {
		putc('\n', stdout);
	}

	return 0;
}

