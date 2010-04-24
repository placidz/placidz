CPP=g++
CPPFLAGS=-O3 

all : tp1

tp1 : tp1.o
		$(CPP) $(CPPFLAGS) -o tp1 tp1.o -I/usr/X11R6/include -L/usr/X11R6/lib -lglut -lGLU -lGL -lX11  -lXt -lXi -lXmu -lXext -lX11 -lm -ggdb3

tp1.o : tp1.cpp
		$(CPP) $(CPPFLAGS) -o tp1.o -c tp1.cpp

clean :
		rm -f *.o
		rm -f tp1
