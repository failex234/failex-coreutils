all: echo true false yes dd

clean:
	rm build/*
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

