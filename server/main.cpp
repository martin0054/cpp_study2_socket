#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <iostream>
#include <string>
#include <unistd.h>
#include <sys/types.h>
#include <arpa/inet.h>

// socket() 생성 > bind() 주소할당 > listen() > accept() > read() ,write() > close(;)

// int socket( int domain, int type, int protocol );
// 	--> 성공 시 파일디스크립터(이하 fd), 실패시 -1 반환.
//     ㄴ. domain   : 소켓이 사용할 프로토콜 체계(Protocol Family) 정보 전달
//     ㄴ. type     : 소켓의 데이터 전송방식에 대한 정보 전달
//     ㄴ. protocol : 두 컴퓨터간 통신에 사용되는 프로토콜 정보 전달

int creatSocket()
{
    // IPv4 인터넷 프로토콜 , 체계에서 동작하는 연결지향형 데이터 전송 소켓(TCP)
    return socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
}

void error_handling(const char *message);

void error_handling(const char *message)
{
    std::cout << "[Error] " << message << std::endl;
    // fputs(message, stderr); //문자열 , steam
    // fputc('\n', stderr); //char , stream
    exit(1);
}

int main(int argc, char *argv[])
{

    // 서버 소켓 파일디스크립터 변수 선언
    int serv_sock;

    // argc가 2가 아니면 1 => error
    if (argc != 2)
    {
        printf("Usage : %s <port>\n", argv[0]);
        exit(1);
    }

    // socket 생성
    serv_sock = creatSocket();

    if (serv_sock == -1)
        error_handling("socket() error");

    struct sockaddr_in server_address;

    int port = atoi(argv[1]); // atoi(argv[1]) 명령행에서 전달된 첫 번째 인수를 정수로 변환
                              // serv_sock에 bind 할  주소정보 초기화
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    server_address.sin_port = htons(port);

    // socket bind - 소켓에 주소 할당
    int bindStatus = bind(serv_sock, (struct sockaddr *)&server_address, sizeof(server_address));

    if (bindStatus < 0) // 성공 : 0 / 실패 : -1
        error_handling("bind() error!");
    // exit(0);

    // socket listen , client의 connect 대기
    std::cout << "Waiting for a client to connect..." << std::endl;
    int listenStatus = listen(serv_sock, 10); // 10번 기다림
    if (listenStatus < 0)
        error_handling("listen() error!");
    // exit(0);


-----------

    // accept()함수호출을 해서 실제 데이터를 보낼 수 있는 소켓을 생성
    socklen_t clnt_addr_size=sizeof(clnt_addr);
    int clnt_sock=accept(serv_sock, (struct sockaddr*)&clnt_addr,&clnt_addr_size);
    if(clnt_sock==-1)
        error_handling("accept() error");  
    
    // 메시지를 보낸다
    char message[]="Hello World!";
    write(clnt_sock, message, sizeof(message));
    // 서버, 클라이언트 소켓의 연결을 해제
    close(clnt_sock);	
    close(serv_sock);
    return 0;



    --------
sockaddr_in newSockAddr;
    socklen_t newSockAddrSize = sizeof(newSockAddr);
    //accept, create a new socket descriptor to 
    //handle the new connection with client
    int newSd = accept(serverSd, (sockaddr *)&newSockAddr, &newSockAddrSize);
    if(newSd < 0)
    {
        cerr << "Error accepting request from client!" << endl;
        exit(1);
    }
    cout << "Connected with client!" << endl;

    -------
      
    // accept 하기위한 클라이언트 구조체 크기 밑작업 및 함수호출
    int clnt_addr_size = sizeof(st_clnt_addr);
    int acceptret = accept(serv_sock,
                          (struct sockaddr*) &st_clnt_addr,
                           &clnt_addr_size );
    if(acceptret == -1) errhandle("accept() ERR!");
    
    // sendmsg 보내고, recvmsg에 수신된 string데이터 담기
    write(clnt_sock, sendmsg, sizeof(sendmsg) );
    int readstrlen = read(clnt_sock, recvmsg, sizeof(recvmsg)-1);
    
    //소켓은 파일이다! 닫아야 한다.
    close(clnt_sock);
    close(serv_sock);
}