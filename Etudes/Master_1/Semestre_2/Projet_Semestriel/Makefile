CPP=g++
CPPFLAGS=-O3 
SRC=source/

all : main

main : main.o OutilsPGM.o 
	$(CPP) $(CPPFLAGS) -o projet main.o $(SRC)OutilsPGM.o -I/usr/X11R6/include -L/usr/X11R6/lib -lglut -lGLU -lGL -lX11  -lXt -lXi -lXmu -lXext -lX11 -lm 

main.o : main.cpp 
	$(CPP) $(CPPFLAGS) -o main.o -c main.cpp

OutilsPGM.o : $(SRC)OutilsPGM.c
	$(CC) $(CFLAGS) -o $(SRC)OutilsPGM.o -c $(SRC)OutilsPGM.c

clean :
	rm -f *.o
	rm -f source/*.o
	rm -f main
