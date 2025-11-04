all: code

code: solution_2076.c
	gcc -o code solution_2076.c

clean:
	rm -f code
