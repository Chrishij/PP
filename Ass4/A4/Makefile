#CFLAGS=-Wall -O0 -fopt-info-vec-all
CFLAGS=-Wall -O3 -fopt-info-vec
INCLUDES=-I/opt/X11/include
LDFLAGS=-L/opt/X11/lib -lX11 -lm

galsim: galsim.o graphics.o
	gcc -o galsim galsim.o graphics.o $(LDFLAGS)

galsim.o: galsim.c graphics.h
	gcc $(CFLAGS) $(INCLUDES) -c galsim.c

graphics.o: graphics.c graphics.h
	gcc $(CFLAGS) $(INCLUDES) -c graphics.c

clean:
	rm -f ./galsim *.o