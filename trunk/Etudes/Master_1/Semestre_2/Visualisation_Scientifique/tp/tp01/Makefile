CPP=g++
CPPFLAGS=-O3 

all : tp01

tp01 : tp01.o Point2D.o Polygone.o 
	$(CPP) $(CPPFLAGS) -o tp01 tp01.o Point2D.o Polygone.o -I/usr/X11R6/include -L/usr/X11R6/lib -lglut -lGLU -lGL -lX11  -lXt -lXi -lXmu -lXext -lX11 -lm 

tp01.o : tp01.cpp 
	$(CPP) $(CPPFLAGS) -o tp01.o -c tp01.cpp

Point2D.o : Point2D.cpp
	$(CPP) $(CPPFLAGS) -o Point2D.o -c Point2D.cpp
	
Polygone.o : Polygone.cpp
	$(CPP) $(CPPFLAGS) -o Polygone.o -c Polygone.cpp

clean :
	rm -f *.o
	rm -f tp01
