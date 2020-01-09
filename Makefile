compile: socket

socket: client.o server.o
	g++ -pthread client.o -o client 
	g++ -pthread server.o -o server

client.o: client.cpp
	g++ -c client.cpp -pthread

server.o: server.cpp
	g++ -c server.cpp -pthread

clean:
	rm -rf *o client server
 
