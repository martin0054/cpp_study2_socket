all: client

client: main.cpp tcp-socket-client.cpp tcp-socket-client.h
	g++ -g -o client -lpthread main.cpp tcp-socket-client.cpp tcp-socket-client.h

clean:
	rm -f client
 