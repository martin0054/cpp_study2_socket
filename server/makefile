all: server

server: main.cpp tcp-socket-server.cpp tcp-socket-server.h
	g++ -g -o  server main.cpp tcp-socket-server.cpp tcp-socket-server.h

clean:
	rm -f server
