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
#include <stdint.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <sys/time.h>

#include "failex-coreutils.h"

FILE *input;
FILE *output;

long copied;
long b_copied;
int curr_blocks;
long blocks_copied;
int block_size = 512;

struct timeval start;
struct timeval end;
double runtime;

double avg_rate;

int suffixedvaltoval( char *val );
char *getvalofarg( char *string, int start );
char *converttobiggerunit( double size );
void help( char *prgname );
void conclusion( void );
void run_dd( void );

int main( int argc, char **argv ) {

    signal(SIGINT, sighandler);
    //Check if input and output are given
    //and input leads to an existing file
    int inputgiven = 0;
    int outputgiven = 0;
    int bsgiven = 0;

    for (int i = 1; i < argc; i++) {
        if (strstr(argv[i], "if=")) {
            if (!inputgiven) {
                inputgiven = 1;
                input = fopen(getvalofarg(argv[i], 3), "rb");

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
                output = fopen(getvalofarg(argv[i], 3), "wb");

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

	if(strstr(argv[i], "bs=")) {
		if(!bsgiven) {
			block_size = suffixedvaltoval(getvalofarg(argv[i], 3));
			bsgiven = 1;
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

    run_dd();
    return 0;

}

char *getvalofarg( char *string, int start ) {
    char *temp = malloc(sizeof(char) * (strlen(string) - 2));


    int j = 0;

    //Copy filepath into a new string without if= or of= etc.
    for (int i = start; i < strlen(string); i++) {
        temp[j] = string[i];
        j++;
    }

    temp[strlen(string) - 3] = '\0';

    return temp;
}

void help( char *prgname ) {
	printf("usage: %s [OPERAND]...\n", prgname);
	printf("\nCopy the contents of a file to another.\n");
	printf("currently implemented operands:\n");
	printf("\nif=/path/to/file/       read from file instead of stdin\n");
	printf("of=/path/to/file/       write to file instead of stdout\n");
	printf("bs=SIZE                 set BLOCKSIZE to SIZE\n");
	printf("\ncurrently implemented arguments:\n");
	printf("\n--help       show this menu then exit\n");
	printf("--version    show version information then exit\n\n");
}

void run_dd( void ) {
	gettimeofday(&start, NULL);
	int c = 0;

	while((c = fgetc(input)) != EOF) {
		if ((!isatty(STDIN_FILENO) || input != stdin) || (!isatty(STDOUT_FILENO) || output != stdout)) {
			fputc(c, output);
			copied++;
			curr_blocks += sizeof(char);

			if(curr_blocks >= block_size) {
				curr_blocks = 0;
				blocks_copied++;
			}
		}
	}

	fputs("Complete!\n", stdout);
	conclusion();
}

void conclusion( void ) {
	b_copied = (double) copied * sizeof(char);
	gettimeofday(&end, NULL);
	runtime = (double) (end.tv_usec - start.tv_usec) / 1000000 + (double) (end.tv_sec - start.tv_sec);

	avg_rate =  (b_copied / 1000) / runtime;

	if(b_copied < 1000000) {
		printf("%ld bytes copied, %f s, %.02f kB/s\n", b_copied, runtime, avg_rate);
	} else {
		printf("%ld bytes copied (%d MB, %d MiB), %f s, %s/s\n", b_copied, (int) b_copied / 1000000, (int) b_copied / 1048576, runtime, converttobiggerunit(avg_rate));
	}
	printf("%ld blocks copied\n", blocks_copied);

	fclose(input);
	fclose(output);
}

void sighandler( int sig ) {
	conclusion();
	exit(0);
}


int suffixedvaltoval( char *val ) {
	char suffix = 'Z';
	char *valcpy = val;
	int suffixfound = 0;

	for (int i = 0; i < strlen(valcpy); i++) {
		//Check string until we find a upper or lower case character
		//Replace suffix with null character so that we can convert
		//the string into a number
		if ((valcpy[i] > 65 && valcpy[i] < 90) || (valcpy[i] > 97 && valcpy[i] < 122)) {
			suffix = valcpy[i];
			valcpy[i] = '\0';
			suffixfound = 1;
			break;
		}
	}

	int tempval = atoi(valcpy);

	if(!suffixfound) {
		return tempval;
	}

	switch(suffix) {
		case 'K':
		case 'k':
			return tempval * 1000;
		case 'M':
		case 'm':
			return tempval * 1000000;
		case 'G':
		case 'g':
			return tempval * 1000000000;
		default:
			return -1;
	}

}

char *converttobiggerunit( double size ) {
	char *sizewithunit = malloc(sizeof(char) * 8);
	double newsize;
	//Use GB as unit when size is larger than 900MB
	if (size >= 900000000.0) {
		newsize = size / 1000000000.0;
		sprintf(sizewithunit, "%.02f GB", newsize);
	} else if (size >= 900000.0) {
		newsize = size / 1000000.0;
		sprintf(sizewithunit, "%.02f MB", newsize);
	} else if (size >= 900.0) {
		newsize = size / 1000.0;
		sprintf(sizewithunit, "%.02f KB", newsize);
	} else {
		sprintf(sizewithunit, "%.02f B", size);
	}

	return sizewithunit;
}
