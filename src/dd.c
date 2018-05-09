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

FILE *input;
FILE *output;

char *getpathofarg( char *string );
void usage( char *prgname );

int main( int argc, char **argv ) {
	if (argc < 3) {
		usage(argv[0]);
		return 1;
	} else {
		//Check if input and output are given
		//and input leads to an existing file
		int inputgiven = 0;
		int outputgiven = 0;

		for (int i = 1; i < argc; i++) {
			if(strstr(argv[i], "if=")) {
				if (!inputgiven) {
					inputgiven = 1;
					input = fopen(getpathofarg(argv[i]), "rb");
				} else {
					fputs("Too many input files!", stderr);
					return 1;
				}
			}

			if (strstr(argv[i], "of=")) {
				if (!outputgiven) {
					outputgiven = 1;
					output = fopen(getpathofarg(argv[i]), "wb");
				} else {
					fputs("Only one output is possible!", stderr);
					return 1;
				}
			}
		}
		if (!inputgiven || !outputgiven) {
			usage();
			return 1;
		}

		if (input == NULL) {
			fputs("Input file doesn't exist!", stderr);
			return 1;
		}

		fseek(input, 0L, SEEK_END);
		int filesize = ftell(input);
		fseek(input, 0L, SEEK_SET);

		int c;
		for (int i = 0; i < filesize; i++) {
			c = fgetc(input);
			fputc(c, output);
		}

		fclose(input);
		fclose(output);

		puts("Complete!");

		return 0;

	}
}

void usage( char *prgname ) {
	printf("usage: %s if=/path/to/file of=/path/to/file\n", prgname);
}

char *getpathofarg( char *string ) {
	char *temp = malloc(sizeof(char) * (strlen(string) - 2));

	int j = 0;
	for (int i = 3; i < strlen(string); i++) {
		temp[j] = string[i];
		j++;
	}

	temp[strlen(string) - 3] = '\0';

	return temp;
}
