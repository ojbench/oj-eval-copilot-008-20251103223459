all: code

code: solution_2079.c
	gcc -o code solution_2079.c

clean:
	rm -f code
