//
// Created by felix on 20.05.18.
//
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include "failex-coreutils.h"

#define MAXFILES 64

void addfile( const FILE* );
void mallocnext( void );
void freelast( void );
void freeall( void );
char *getpermissions( FILE* );
char *getowner( FILE* );
char *getgroup( FILE* );
char *getsize( FILE* );
char *getlastmodified( FILE* );

int longoutput = 0; /* -l flag*/
int showall = 0; /* -a flag */
int currfiles = 0;
FILE *allfiles;

int main( int argc, char **argv ) {
    for (int i = 1; i < argv; i++) {
        if (!strcmp(argv[i], "-l")) {
            longoutput = 1;
        } else if (!strcmp(argv[i], "-a")) {
            showall = 1;
        } else if (!strcmp(argv[i], "--help")) {
            help(argv[0]);
        } else if (!strcmp(argv[i], "--version")) {
            version(argv[0]);
        } else {
            //Check if program argument is an actual argument or a file
            if (argv[i][0] == '-') {
                fprintf(stderr, "Unrecognized argument %s\n", argv[i]);
                return 1;
            } else {
                mallocnext();
                FILE *temp = fopen(argv[i], "r");

                if (temp == NULL) {
                    fprintf(stderr, "No such file or directory %s\n", argv[i]);
                    return 1;
                }

                addfile(temp);
                currfiles++;

            }
        }
    }

    //Check if any files where given as an argument
    if (currfiles == 0) {
        DIR *dir;
        struct dirent *ep;
        dir = opendir("./");

        if (dir == NULL) {
            fputs("Unable to open directory!", stderr);
            return 1;
        }

        while (ep = readdir(dir)) {
            if (longoutput) {
                printf("%s %s %s %s %s %s\n", getpermissions(NULL), getowner(NULL), getgroup(NULL), getsize(NULL), getlastmodified(NULL), ep->d_name);
            } else {
                printf("%s   ", ep->d_name);
            }
        }
        closedir(dir);
    } else {
        int currfile = 0;
        if (longoutput) {
            printf("%s %s %s %s %s %s\n", getpermissions(allfiles[currfile]), getowner(allfiles[currfile]), getgroup(allfiles[currfile]), getsize(allfiles[currfile]), getlastmodified(allfiles[currfile]), getfilename(allfiles[currfile]);
        } else {
	    printf("%s   ", getfilename(allfiles[currfile]))M
        }
    }
}

void addfile( const FILE *file ) {
        allfiles[currfiles] = file;
}

void mallocnext( void ) {
    if (currfiles == 0) {
        allfiles = malloc(sizeof(FILE));
    } else {
        allfiles = realloc(allfiles, sizeof(FILE) * (currfiles + 1));
    }
}

void freelast( void ) {
    if (currfiles != 0) {
        free(allfiles[currfiles - 1]);
        currfiles--;
    }
}

void freeall( void ) {
    while(currfile != 0) {
        freelast();
    }
}

char *getpermissions( FILE *file ) {
    //stub
    return "-rwxr-xr-x";
}

char *getowner( FILE *file ) {
    //stub
    return "leet";
}

char *getgroup( FILE *file ) {
    //stub
    return "leet";
}

/*
 * Give back file size in most appropriate unit
 */
char *getsize( FILE *file ) {

}

char *getlastmodified( FILE *file ) {
    //stub
    return "May 21st 10:15";
}

char *getfilename( FILE *file ) {
	//stub
	return "test.c"
}
