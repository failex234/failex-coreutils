#ifndef FLX_COREUTILS_H
#define FLX_COREUTILS_H

const char VERSION[] = "0.0.1";
const char AUTHOR[] = "Felix Naumann";
const char URL[] = "https://github.com/failex234/failex-coreutils";

void version( char *prgname ) {
	fprintf(stdout, "%s (failex-coreutils) %s\n", prgname, VERSION);
	fprintf(stdout, "failex-coreutils is a small side-project that is not meant\n");
	fprintf(stdout, "to replace other coreutils, this project is for educational\n");
	fprintf(stdout, "purposes. This project may not be POSIX-compliant.\n\nWritten by %s.\nGitHub Repository: %s\n", AUTHOR, URL);
}
#endif
