#ifndef _TCP_SOCKET_SERVER_HEADER_

#include <sys/socket.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>
#include <arpa/inet.h>


class SocektServer {

public :
    SocektServer();
    ~SocektServer();


private : 
    int server_socket_;
    
    int creatSocket();

public :

int getSocket();
int bindSocket(int server_socket,struct sockaddr_in server_address);
int listenSocket(int server_socket);

};


#endif