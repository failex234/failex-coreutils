#TODO prettify Makefile
CFLAGS=-Wall -O2
all: mkbuild echo true false yes dd ls cat

mkbuild:
	mkdir -p build
clean:
	rm -rf build/
echo:
	gcc -o build/echo src/echo.c
true:
	gcc -o build/true src/true.c
false:
	gcc -o build/false src/false.c
yes:
	gcc -o build/yes src/yes.c
dd:
	gcc $(CFLAGS) -o build/dd src/dd.c
ls:
	gcc $(CFLAGS) -o build/ls src/ls.c
cat:
	gcc $(CFLAGS) -o build/cat src/cat.c

