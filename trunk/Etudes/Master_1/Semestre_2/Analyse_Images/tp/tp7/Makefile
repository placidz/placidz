CPP=gcc
CPPFLAGS=-O3 -ggdb3

all : tp7

tp7 : tp7.o OutilsPGM.o 
	$(CPP) $(CPPFLAGS) -o tp7 tp7.o OutilsPGM.o -I/usr/X11R6/include -L/usr/X11R6/lib -lglut -lGLU -lGL -lX11  -lXt -lXi -lXmu -lXext -lX11 -lm 

tp7.o : tp7.cpp
	$(CPP) $(CPPFLAGS) -o tp7.o -c tp7.cpp 

OutilsPGM.o : OutilsPGM.cpp
	$(CPP) $(CPPFLAGS) -o OutilsPGM.o -c OutilsPGM.cpp

clean :
	rm -f *.o
	rm -f tp7
