CPP=g++
CPPFLAGS=-O3 

all : tp2

tp2 : tp2.o
		$(CPP) $(CPPFLAGS) -o tp2 tp2.o -I/usr/X11R6/include -L/usr/X11R6/lib -lglut -lGLU -lGL -lX11  -lXt -lXi -lXmu -lXext -lX11 -lm -ggdb3

tp2.o : tp2.cpp
		$(CPP) $(CPPFLAGS) -o tp2.o -c tp2.cpp

clean :
		rm -f *.o
		rm -f tp2
