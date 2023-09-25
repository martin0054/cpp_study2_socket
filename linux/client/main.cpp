#include "tcp-socket-client.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <iostream>

#include <netdb.h>
#include <sys/uio.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <fstream>
#include <thread>

// socket > conenct > read/write recv> close

void error_handling(const char *message);

void error_handling(const char *message)
{
    std::cout << "[Error] " << message << std::endl;
    // fputs(message, stderr); //문자열 , steam
    // fputc('\n', stderr); //char , stream
    exit(1);
}

// Client side
int main(int argc, char *argv[])
{
    SocketClient socketClient;

    int client_socket;

    if (argc != 3)
    {
        printf("Usage : %s <ip_address> <port>\n", argv[0]);
        exit(1);
    }

    // IP address , port
    char *serverIp = argv[1];
    int port = atoi(argv[2]);

    // message buffer 생성
    char msg[1500];
    struct hostent *server = gethostbyname(serverIp);

    if (server == NULL)
    {
        error_handling("no such host ");

        // exit(0);
    }

    struct sockaddr_in send_socket_address;

    memset(&send_socket_address, 0, sizeof(send_socket_address));
    send_socket_address.sin_family = AF_INET;
    send_socket_address.sin_port = htons(port);
    // server_address.sin_addr.s_addr = inet_addr("127.0.0.1"); // IP address of the server
    send_socket_address.sin_addr.s_addr =
        inet_addr(inet_ntoa(*(struct in_addr *)*server->h_addr_list)); //??

    client_socket = socketClient.getSocket();

    if (client_socket == -1)
        error_handling("socket() error");

    // try to connect...
    int connectStatus = socketClient.connectSocket(client_socket, send_socket_address);
    if (connectStatus < 0)
        error_handling("connect() error!");
    // exit(0);

    std::cout << "Connected to the server!" << std::endl;
    int bytesRead, bytesWritten = 0;
    struct timeval start1, end1;
    gettimeofday(&start1, NULL);

    // thread  -.> receve 해서 읽기만

    // receive thead 생성
    std::thread recvThread([&]()
                           {
        fd_set read_fds;
        FD_ZERO(&read_fds);               // reads fd_set 초기화
        FD_SET(client_socket, &read_fds); // 서버 소켓 fd를 read_fds fd_set에등록
        int fd_max = client_socket;
        struct timeval timeout;
        timeout.tv_sec = 5;     // timeout 초 정의
        timeout.tv_usec = 5000; // timeout 마이크로초 정의

        while (true)
        {
            fd_set cpy_reads = read_fds; // select시 원본 훼손되므로 복사해서 사용
            int selectStatus = select(fd_max + 1, &cpy_reads, NULL, NULL, &timeout);

            if (selectStatus == -1)
            {
                error_handling("select() error!");
                return;
            }

            if (FD_ISSET(client_socket, &cpy_reads))
            { // fd에 변화가 있나~ FD_ISSET으로 확인

                // server에서 data receive
                char buffer[1024];
                int recevStatus = recv(client_socket, buffer, sizeof(buffer), 0);
                if (recevStatus < 0) // server에서 받은 data없음
                {
                    error_handling("client recv() error!");
                    return;
                }
                if (recevStatus == 0)
                { // data도 없고  server랑 연결도 끊김
                    break;
                }
                std::cout << "Sever : " << buffer << std::endl;
            }
        } });

    while (true)
    {
        std::cout << ">";
        std::string data;
        getline(std::cin, data);
        std::cout << " Client : " << data << " ";
        // strcpy(msg, data.c_str());
        if (data == "exit")
        {
            send(client_socket, data.c_str(), data.length(), 0);
            break;
        }

        // server에 messege 보내기
        bytesWritten += send(client_socket, data.c_str(), data.length(), 0);
    }

    gettimeofday(&end1, NULL);

    std::cout << "********Session********" << std::endl;
    std::cout << "Bytes written: " << bytesWritten << " Bytes read: " << bytesRead << std::endl;
    std::cout << "Elapsed time: " << (end1.tv_sec - start1.tv_sec)
              << " secs" << std::endl;
    std::cout << "Connection closed" << std::endl;

    // recvThread 종료
    recvThread.join();
    return 0;
}