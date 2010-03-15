CPP=gcc
CPPFLAGS=-O3 -ggdb3

all : tp6

tp6 : tp6.o OutilsPGM.o 
	$(CPP) $(CPPFLAGS) -o tp6 tp6.o OutilsPGM.o -I/usr/X11R6/include -L/usr/X11R6/lib -lglut -lGLU -lGL -lX11  -lXt -lXi -lXmu -lXext -lX11 -lm 

tp6.o : tp6.cpp
	$(CPP) $(CPPFLAGS) -o tp6.o -c tp6.cpp 

OutilsPGM.o : OutilsPGM.cpp
	$(CPP) $(CPPFLAGS) -o OutilsPGM.o -c OutilsPGM.cpp

clean :
	rm -f *.o
	rm -f tp6
