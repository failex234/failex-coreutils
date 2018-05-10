#TODO prettify Makefile
all: mkbuild echo true false yes dd

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
	gcc -o build/dd src/dd.c

