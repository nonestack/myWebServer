VPATH = tools
objects = main.o wrapSock.o

server : $(objects)
	g++ -o server $(objects)

main.o wrapSock.o : tools.h


clean : 
	rm server $(objects)