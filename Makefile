all: simple_test1 simple_test2 random_test

simple_test1: my_malloc.o simple_test1.o
	gcc -std=c99 -Wall -g -o simple_test1 my_malloc.o simple_test1.o

simple_test1.o: simple_test1.c my_malloc.h
	gcc -std=c99 -Wall -g -c simple_test1.c

simple_test2: my_malloc.o simple_test2.o
	gcc -std=c99 -Wall -g -o simple_test2 my_malloc.o simple_test2.o

simple_test2.o: simple_test2.c my_malloc.h
	gcc -std=c99 -Wall -g -c simple_test2.c

random_test: my_malloc.o random_test.o
	gcc -std=c99 -Wall -g -o random_test my_malloc.o random_test.o

random_test.o: random_test.c my_malloc.h
	gcc -std=c99 -Wall -g -c random_test.c

my_malloc.o: my_malloc.c my_malloc.h
	gcc -std=c99 -Wall -g -c my_malloc.c

clean:
	rm -rf simple_test1 simple_test1.o my_malloc.o simple_test2 simple_test2.o random_test random_test.o
