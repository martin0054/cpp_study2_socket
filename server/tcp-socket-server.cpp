#include "tcp-socket-server.h"
#include <iostream>

SocektServer::SocektServer(){

   std::cout << "socket 생성 " <<std::endl;
     server_socket_ = creatSocket();
    
}

SocektServer::~SocektServer()
{
     std::cout << "socket 해제 " <<std::endl;
    close(server_socket_);

}


int SocektServer::creatSocket()
{
    // IPv4 인터넷 프로토콜 , 체계에서 동작하는 연결지향형 데이터 전송 소켓(TCP)
    return socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
}

int SocektServer::getSocket(){
    return server_socket_;
}

int SocektServer::listenSocket(int server_socket){
    return listen(server_socket, 10); // 10번 기다림
}
int SocektServer::bindSocket(int server_socket,struct sockaddr_in server_address){
   return bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address));

}

