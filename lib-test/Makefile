.PHONY:all staticlib main clean

all: staticlib main

staticlib:
	gcc -c staticlib.c -o staticlib.o
	ar -r staticlib.a staticlib.o

main:
	gcc main.c staticlib.a

clean:
	rm -f *.a *.o a.out
