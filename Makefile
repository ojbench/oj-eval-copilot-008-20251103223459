all: code

code: solution.c
	gcc -o code solution.c

clean:
	rm -f code
