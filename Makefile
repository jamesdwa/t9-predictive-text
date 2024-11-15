all: t9 t9_demo

t9: t9_demo.o t9_lib.o
	gcc -Wall -g -std=c11 t9_demo.o t9_lib.o -o t9

t9_demo: t9_demo.o t9_lib.o
	gcc -Wall -g -std=c11 t9_demo.o t9_lib.o -o t9_demo

t9_demo.o: t9_demo.c t9_lib.h
	gcc -Wall -g -std=c11 -c t9_demo.c

t9_lib.o: t9_lib.c t9_lib.h
	gcc -Wall -g -std=c11 -c t9_lib.c

clean:
	rm -f *.o t9 t9_demo