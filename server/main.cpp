
#include "tcp-socket-server.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <iostream>
#include <vector>

void error_handling(const char *message);

void error_handling(const char *message)
{
    std::cout << "[Error] " << message << std::endl;
    // fputs(message, stderr); //문자열 , steam
    // fputc('\n', stderr); //char , stream
    exit(1);
}

struct Client
{
    int socket;
    std::string name;
};

int main(int argc, char *argv[])
{

    SocektServer socketServer;
    // 서버 소켓 파일디스크립터 변수 선언
    int server_socket;

    // argc가 2가 아니면 1 => error
    if (argc != 2)
    {
        printf("Usage : %s <port>\n", argv[0]);
        exit(1);
    }

    // socket 생성
    server_socket = socketServer.getSocket();

    int reuse_address = 1;
    // SO_REUSEADDR	BOOL	이미 사용된 주소를 재사용하도록 함
    setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &reuse_address, sizeof(reuse_address));

    if (server_socket == -1)
        error_handling("socket() error");

    struct sockaddr_in server_address;

    int port = atoi(argv[1]); // atoi(argv[1]) 명령행에서 전달된 첫 번째 인수를 정수로 변환
                              // server_socket에 bind 할  주소정보 초기화
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    // server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    server_address.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_address.sin_port = htons(port);

    // socket bind - 소켓에 주소 할당
    int bindStatus = socketServer.bindSocket(server_socket, server_address);

    if (bindStatus < 0) // 성공 : 0 / 실패 : -1
        error_handling("bind() error!");
    // exit(0);

    // socket listen , client의 connect 대기
    int listenStatus = socketServer.listenSocket(server_socket);
    if (listenStatus < 0)
        error_handling("listen() error!");
    // exit(0);

    fd_set read_fds;
    struct timeval tv;
    FD_ZERO(&read_fds);               // reads fd_set 초기화
    FD_SET(server_socket, &read_fds); // 서버 소켓 fd를 read_fds fd_set에등록
    struct timeval timeout;
    timeout.tv_sec = 5;     // timeout 초 정의
    timeout.tv_usec = 5000; // timeout 마이크로초 정의

    std::vector<Client> clients;

    while (true)
    {

        fd_set cpy_reads = read_fds; // select시 원본 훼손되므로 복사해서 사용
        int selectStatus = select(FD_SETSIZE, &cpy_reads, NULL, NULL, &timeout);

        if (selectStatus == -1)
        {
            error_handling("select() error!");
            break;
        }
        // else if (selectStatus == 0)
        // {
        //     std::cout << "Waiting for a client to connect" << std::endl;
        //     continue;
        // }

        // fd에 변화여부 FD_ISSET으로 확인
        if (FD_ISSET(server_socket, &cpy_reads))
        {
            sockaddr_in client_address;
            socklen_t client_address_len = sizeof(client_address);

            // accept client
            std::cout << "accept client " << std::endl;
            int client_socket = accept(server_socket, (sockaddr *)&client_address, &client_address_len);

            if (client_socket < 0)
            {
                error_handling("client accept() error!");
                continue;
                // exit(0);
            }

            FD_SET(client_socket, &read_fds); // 원본 fd_set에 클라이언트 등록!!

            // vector<client> 에  client 추가
            Client client;
            client.socket = client_socket;
            client.name = "c " + std::to_string(client_socket);
            clients.push_back(client);

            // if (fd_max < client_socket)
            //     fd_max = client_socket; // 최대 fd길이 늘려줌 (추가되었으니)
            // std::cout << "client Connect : " << client_socket << std::endl;
        }

        // client sockets 확인
        for (int i = 0; i < clients.size(); i++)
        {
            if (FD_ISSET(clients[i].socket, &cpy_reads))
            {
                // Receive data from the client
                char buffer[1024];
                int bytes_received = recv(clients[i].socket, buffer, sizeof(buffer), 0);
                if (bytes_received < 0)
                {
                    error_handling("receive() client data error!");
                    return 1;
                }

                // recevie data가 없거나 clinet 연결이 끊겼으면 모니터링중인 소켓 해제
                if (bytes_received == 0)
                {

                    FD_CLR(clients[i].socket, &read_fds);
                    close(clients[i].socket);
                    clients.erase(clients.begin() + i);
                    continue;
                }
                else
                {
                    buffer[bytes_received] = 0;
                    std::cout << "[" << clients[i].name << "] buffer : " << buffer << std::endl;
                    // Echo the data back to the client
                    send(clients[i].socket, buffer, bytes_received, 0);
                }
            }
        }
    }

    // Close the server socket
    for (auto client = clients.begin(); client != clients.end(); ++client)
        close(client->socket);

    return 0;
}
