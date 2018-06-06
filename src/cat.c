/*
 * Part of Felix' Coreutils
 * 
 * cat
 *
 * This is a private project so I'm not thinking about
 * licensing this software suite. Maybe one day it will
 * be but why? There are already many coreutils implementations.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

#include "failex-coreutils.h"

FILE **files;
int inputfiles = 0;

void addtofilelist( const char *filepath );
void clearfilelist( void );
void help ( char *prgname );
void run_cat( void );

int main( int argc, char **argv ) {

    signal(SIGINT, sighandler);

    for (int i = 1; i < argc; i++) {

	if (!strcmp(argv[1], "--help")) {
		help(argv[0]);
		return 0;
	}

	if (!strcmp(argv[1], "--version")) {
		version(argv[0]);
		return 0;
	}

	if (strlen(argv[i]) > 3 && argv[i][0] == '-') {
		fprintf(stderr, "%s: Unrecognized argument »%s«\n“%s --help“ is here to help.\n", argv[0], argv[i], argv[0]);
		return 1;
	}

	addtofilelist(argv[i]);
    }

    if (inputfiles == 0) {
	files = malloc(sizeof(FILE));
	files[0] = stdin;
	inputfiles = 1;
    }
    run_cat();
    return 0;

}
void help( char *prgname ) {
	printf("usage: %s [OPTION]... [FILE]...\n", prgname);
	printf("concatenates all input files to the standard output\n");
	printf("read standard input on no file or \"-\"\n");
	printf("\npossible arguments:\n");
	printf("\n--help         - Shows this menu then exit\n");
	printf("--version      - Show version information then exit\n");
}

void sighandler( int sig ) {
	exit(0);
}

void addtofilelist( const char *filepath ) {
	if (!strcmp(filepath, "-")) {
		if(inputfiles == 0) {
		       FILE *temp = fopen(filepath, "r");

		       if (temp == NULL) {
			       fprintf(stderr, "no such file or directory: %s\n", filepath);
			       exit(1);
		       }
					       
		       files = malloc(sizeof(FILE));
		       files[0] = temp;

		       inputfiles++;
		} else {
			FILE *temp = fopen(filepath, "r");

			if (temp == NULL) {
				fprintf(stderr, "no such file or directory: %s\n", filepath);
				clearfilelist();
				exit(0);
			}

			files = realloc(files, sizeof(FILE) * (inputfiles + 1));
			files[inputfiles] = temp;
		}
	}
}

void clearfilelist( void ) {
	for (int i = 0; i < inputfiles; i++) {
		fclose(files[i]);
	}

	free(files);
}

void run_cat( void ) {

	clearfilelist();
}
