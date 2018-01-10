all: hw2
	
hw2: parser.o main.o tm.o
	g++ main.o parser.o tm.o -o hw2
parser.o: parser.cpp parser.h 
	g++ parser.cpp -c
tm.o: tm.cpp tm.h 
	g++ tm.cpp -c
main.o: main.cpp parser.h tm.h 
	g++ main.cpp -c

clean: 
	rm -rf *.o *.gch *.txt *.blif hw2
blif:
	rm -rf *.blif *.txt
read:
	vim -O *.txt
exc:
	./hw2 b_case1.bench cell.lib 1.blif 1.txt
	./hw2 b_case2.bench cell.lib 2.blif 2.txt
	./hw2 b_case3.bench cell.lib 3.blif 3.txt
	./hw2 b_case4.bench cell.lib 4.blif 4.txt
	./hw2 b_case5.bench cell.lib 5.blif 5.txt
	./hw2 b_case6.bench cell.lib 6.blif 6.txt
	./hw2 case1.bench cell.lib 11.blif 11.txt
	./hw2 case2.bench cell.lib 22.blif 22.txt
	./hw2 case3.bench cell.lib 33.blif 33.txt
	./hw2 case4.bench cell.lib 44.blif 44.txt
	./hw2 case5.bench cell.lib 55.blif 55.txt
	./hw2 case6.bench cell.lib 66.blif 66.txt
	./hw2 case7.bench cell.lib 77.blif 77.txt
