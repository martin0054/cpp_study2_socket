#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <iostream>
#include <string>
#include <unistd.h>
#include <sys/types.h>

// socket() 생성 > bind() 주소할당 > listen() > accept() > read() ,write() > close(;)

// int socket( int domain, int type, int protocol );
// 	--> 성공 시 파일디스크립터(이하 fd), 실패시 -1 반환.
//     ㄴ. domain   : 소켓이 사용할 프로토콜 체계(Protocol Family) 정보 전달
//     ㄴ. type     : 소켓의 데이터 전송방식에 대한 정보 전달
//     ㄴ. protocol : 두 컴퓨터간 통신에 사용되는 프로토콜 정보 전달

int creatSocket()
{
    return socket(AF_INET, SOCK_STREAM, 0);
}

int main()
{

    int serv_sock;
    serv_sock = creatSocket();
}