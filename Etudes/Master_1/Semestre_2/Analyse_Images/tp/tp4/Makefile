CPP=g++
CPPFLAGS=-O3 

all : tp4

tp4 : tp4.o
		$(CPP) $(CPPFLAGS) -o tp4 tp4.o -I/usr/X11R6/include -L/usr/X11R6/lib -lglut -lGLU -lGL -lX11  -lXt -lXi -lXmu -lXext -lX11 -lm -ggdb3

tp4.o : tp4.cpp
		$(CPP) $(CPPFLAGS) -o tp4.o -c tp4.cpp

clean :
		rm -f *.o
		rm -f tp4
