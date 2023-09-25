#include "tcp-socket-client.h"
#include <iostream>

SocketClient::SocketClient(){

   std::cout << "client socket 생성 " <<std::endl;
     client_socket_ = creatSocket();
    
}

SocketClient::~SocketClient()
{
     std::cout << "client socket 해제 " <<std::endl;
    close(client_socket_);

}


int SocketClient::creatSocket()
{
    // IPv4 인터넷 프로토콜 , 체계에서 동작하는 연결지향형 데이터 전송 소켓(TCP)
    return socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
}

int SocketClient::getSocket(){
    return client_socket_;
}


int SocketClient::connectSocket(int client_socket,struct sockaddr_in client_address){
   return connect(client_socket,
                         (sockaddr*) &client_address, sizeof(client_address));

}

