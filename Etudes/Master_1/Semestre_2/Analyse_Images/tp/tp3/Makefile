CPP=g++
CPPFLAGS=-O3 

all : tp3

tp3 : tp3.o
		$(CPP) $(CPPFLAGS) -o tp3 tp3.o -I/usr/X11R6/include -L/usr/X11R6/lib -lglut -lGLU -lGL -lX11  -lXt -lXi -lXmu -lXext -lX11 -lm -ggdb3

tp3.o : tp3.cpp
		$(CPP) $(CPPFLAGS) -o tp3.o -c tp3.cpp

clean :
		rm -f *.o
		rm -f tp3
