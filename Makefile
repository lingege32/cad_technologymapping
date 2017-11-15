all: main
	
OFLAGS = -pedantic -Wall -fomit-frame-pointer -funroll-all-loops -O3 -g

main: parser.o main.o tm.o
	g++ $(OFLAGS)  main.o parser.o tm.o -o main
parser.o: parser.cpp parser.h
	g++ $(OFLAGS) parser.cpp -c
tm.o: tm.cpp tm.h
	g++ $(OFLAGS) tm.cpp -c
main.o: main.cpp parser.h tm.h
	g++ $(OFLAGS) main.cpp -c

clean: 
	rm -rf *.o *.gch *.txt main
