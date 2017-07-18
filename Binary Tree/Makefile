all:
	gcc -Wall -pedantic -ansi -g -Iinclude   -c -o lab9.o src/mainTest.c
	gcc -Wall -pedantic -ansi -g -Iinclude   -c -o restaurant.o src/examples.c
	gcc -Wall -pedantic -ansi -g -Iinclude   -c -o list.o src/LinkedList.c
	gcc -Llib  lab9.o restaurant.o list.o -lbintree -o lab9runnable
