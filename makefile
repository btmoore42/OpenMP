#

p3:	p3.c
	cc -c  -Ofast -fopenmp p3.c 
	cc -fopenmp -o p3 p3.o evalfunc.o

clean:
	rm -f p3
	rm -f *.o
