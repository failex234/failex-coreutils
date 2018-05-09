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

FILE *input;
FILE *output;

char *getpathofarg( char *string );
void usage( char *prgname );

//TODO: free all string pointers, close files in all cases

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
			}
		}

		if (!inputgiven || !outputgiven) {
			usage(argv[0]);
			return 1;
		}

		//Get file size
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

	//Copy filepath into a new string without if= or of= 
	for (int i = 3; i < strlen(string); i++) {
		temp[j] = string[i];
		j++;
	}

	temp[strlen(string) - 3] = '\0';

	return temp;
}
