/*
 * Part of Felix' Coreutils
 * 
 * yes
 *
 * This is a private project so I'm not thinking about
 * licensing this software suite. Maybe one day it will
 * be but why? There are already many coreutils implementations.
 */
#include <stdio.h>
#include <stdlib.h>

int main( int argc, char **argv ) {
	if(argc > 1) {
		while(1) {
			for (int i = 1; i < argc; i++) {
				printf("%s", argv[i]);

				if (i + 1 < argc) {
					putc(' ', stdout);
				} else {
					putc('\n', stdout);
				}
			}
		}
	} else {
		while(1) {
			puts("y");
		}
	}
}
