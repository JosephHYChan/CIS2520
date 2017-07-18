all:prog1 prog2

prog1:
	gcc -Wall -pedantic -std=c99 -g -c listio.c -Include 
	ar cr liblistio.a listio.o
	
prog2:
	gcc main.c liblistio.a
clean:
	rm *.o