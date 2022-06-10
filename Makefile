server : main.o
	g++ -o main main.o

main.o : main.cc
	g++ -c main.cc


clean : 
	rm *.o main 