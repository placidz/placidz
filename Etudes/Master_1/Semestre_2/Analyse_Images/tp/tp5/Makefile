CPP=g++
CPPFLAGS=-O3 

all : tp5

tp5 : tp5.o
		$(CPP) $(CPPFLAGS) -o tp5 tp5.o -I/usr/X11R6/include -L/usr/X11R6/lib -lglut -lGLU -lGL -lX11  -lXt -lXi -lXmu -lXext -lX11 -lm -ggdb3

tp5.o : tp5.cpp
		$(CPP) $(CPPFLAGS) -o tp5.o -c tp5.cpp

clean :
		rm -f *.o
		rm -f tp5
