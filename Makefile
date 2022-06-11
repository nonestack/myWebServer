VPATH = tools
objects = main.o wrapSock.o rio.o

server : $(objects)
	g++ -o server $(objects)

main.o wrapSock.o : tools.h


clean : 
	rm server $(objects)