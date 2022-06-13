VPATH = tools:http
objects = main.o wrapSock.o rio.o http.o

server : $(objects)
	g++ -o server $(objects)

main.o wrapSock.o http.o : tools.h

http.o : http.h


clean : 
	rm server $(objects)