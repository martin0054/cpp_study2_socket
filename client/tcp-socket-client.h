#ifndef _TCP_SOCKET_CLIETN_HEADER_

#include <sys/socket.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
class SocketClient {

public :
    SocketClient();
    ~SocketClient();


private : 
    int client_socket_;
    
   
 int creatSocket();

public :

int getSocket();

int connectSocket(int client_socket_,struct sockaddr_in client_address);

};

#endif