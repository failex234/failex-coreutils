/*
 * Part of Felix' Coreutils
 * 
 * dd
 *
 * This is a private project so I'm not thinking about
 * licensing this software suite. Maybe one day it will
 * be but why? There are already many coreutils implementations.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#include "failex-coreutils.h"

FILE *input;
FILE *output;

char *getpathofarg( char *string );
void help( char *prgname );

//TODO: free all string pointers, close files in all cases

int main( int argc, char **argv ) {
    //Check if input and output are given
    //and input leads to an existing file
    int inputgiven = 0;
    int outputgiven = 0;

    for (int i = 1; i < argc; i++) {
        if (strstr(argv[i], "if=")) {
            if (!inputgiven) {
                inputgiven = 1;
                input = fopen(getpathofarg(argv[i]), "rb");

                //Check if user is permitted to read the input file
                if (input == NULL) {
                    if (errno == EACCES) {
                        fputs("Access to input file denied, please rerun command as a privileged user!\n", stderr);
                    } else {
                        fputs("Input file doesn't exist!\n", stderr);
                    }
                    return 1;
                }

            } else {
                fputs("Too many input files!", stderr);
                return 1;
            }
	    continue;
        }

        if (strstr(argv[i], "of=")) {
            if (!outputgiven) {
                outputgiven = 1;
                output = fopen(getpathofarg(argv[i]), "wb");

                //Check if user is permitted to write to the output file
                if (output == NULL) {
                    if (errno == EACCES) {
                        fputs("Access to output file denied, please rerun command as a privileged user!\n", stderr);
                        return 1;
                    }
                }
            } else {
                fputs("Only one output is possible!\n", stderr);
                return 1;
            }
	    continue;
        }

	if (!strcmp(argv[1], "--help")) {
		help(argv[0]);
		return 0;
	}

	if (!strcmp(argv[1], "--version")) {
		version(argv[0]);
		return 0;
	}

	fprintf(stderr, "%s: Unrecognized argument »%s«\n“%s --help“ is here to help.\n", argv[0], argv[i], argv[0]);
	return 1;
    }

    if (!inputgiven) {
        input = stdin;
    }

    if (!outputgiven) {
        output = stdout;
    }

    int c = 0;

    while ((c = fgetc(input)) != EOF) {
        //Check if in- and output are stdin and stdout and they're not piped
        if ((!isatty(STDIN_FILENO) || input != stdin) || (!isatty(STDOUT_FILENO) || output != stdout)) {
            fputc(c, output);
        }
    }

    fputs("Complete!\n", stdout);

    fclose(input);
    fclose(output);

    return 0;

}

char *getpathofarg( char *string ) {
    char *temp = malloc(sizeof(char) * (strlen(string) - 2));

    int j = 0;

    //Copy filepath into a new string without if= or of=
    for (int i = 3; i < strlen(string); i++) {
        temp[j] = string[i];
        j++;
    }

    temp[strlen(string) - 3] = '\0';

    return temp;
}

void help( char *prgname ) {
	printf("usage: %s [OPERAND]...\n", prgname);
	printf("\nCopy the contents of a file to another.\n");
	printf("cureently implemented operands:\n");
	printf("\nif=/path/to/file/       read from file instead of stdin\n");
	printf("of=/path/to/file/       write to file instead of stdout\n");
	printf("\ncurrently implemented arguments:\n");
	printf("\n--help       show this menu then exit\n");
	printf("--version    show version information then exit\n\n");
}
