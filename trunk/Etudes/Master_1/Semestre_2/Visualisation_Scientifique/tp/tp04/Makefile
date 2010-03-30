CPP=g++
CPPFLAGS=-O3 -ggdb3

all : tp04

tp04 : tp04.o Point2D.o Polygone.o tools.o
	$(CPP) $(CPPFLAGS) -o tp04 tp04.o Point2D.o Polygone.o tools.o -I/usr/X11R6/include -L/usr/X11R6/lib -lglut -lGLU -lGL -lX11  -lXt -lXi -lXmu -lXext -lX11 -lm 

tp04.o : tp04.cpp 
	$(CPP) $(CPPFLAGS) -o tp04.o -c tp04.cpp
	
tools.o : tools.cpp
	$(CPP) $(CPPFLAGS) -o tools.o -c tools.cpp

Point2D.o : Point2D.cpp
	$(CPP) $(CPPFLAGS) -o Point2D.o -c Point2D.cpp
	
Polygone.o : Polygone.cpp
	$(CPP) $(CPPFLAGS) -o Polygone.o -c Polygone.cpp
	


clean :
	rm -f *.o
	rm -f tp04
