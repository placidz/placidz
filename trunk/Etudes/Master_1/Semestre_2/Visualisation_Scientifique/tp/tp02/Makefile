CPP=g++
CPPFLAGS=-O3 -ggdb3 

all : tp02

tp02 : tp02.o Point2D.o Polygone.o 
	$(CPP) $(CPPFLAGS) -o tp02 tp02.o Point2D.o Polygone.o -I/usr/X11R6/include -L/usr/X11R6/lib -lglut -lGLU -lGL -lX11  -lXt -lXi -lXmu -lXext -lX11 -lm 

tp02.o : tp02.cpp 
	$(CPP) $(CPPFLAGS) -o tp02.o -c tp02.cpp

Point2D.o : Point2D.cpp
	$(CPP) $(CPPFLAGS) -o Point2D.o -c Point2D.cpp
	
Polygone.o : Polygone.cpp
	$(CPP) $(CPPFLAGS) -o Polygone.o -c Polygone.cpp

clean :
	rm -f *.o
	rm -f tp02
