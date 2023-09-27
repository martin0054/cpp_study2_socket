#include <WinSock2.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <thread>


#pragma comment (lib, "Ws2_32.lib")

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 9000
#define BUFSIZE 1024


void error_handling(const char* message)
{
	std::cout << "[Error] " << message << std::endl;
	exit(1);
}


int main(int argc, char* argv[]) {

	WSADATA wsaData;
	SOCKET client_socket = INVALID_SOCKET;
    int retval;
    int iResult;
    
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);

	if (iResult != 0) {
		printf("WSAStartup failed with error: %d\n", iResult);
		return 1;
	}

	struct hostent* server = gethostbyname(SERVER_IP);

	if (server == NULL) {
		printf("gethostbyname failed with error: %ld\n", WSAGetLastError());
	}


	SOCKADDR_IN server_addr;
	ZeroMemory(&server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr =
		inet_addr(inet_ntoa(*(struct in_addr*)*server->h_addr_list));
	server_addr.sin_port = htons(SERVER_PORT);
	

	client_socket = socket(AF_INET, SOCK_STREAM, 0);

	if (client_socket == INVALID_SOCKET) {
		printf("socket failed with error: %ld\n", WSAGetLastError());
		WSACleanup();
		return 1;
	}
	std::cout << "Creat socket" << std::endl;

	iResult = connect(client_socket,(sockaddr*)&server_addr, sizeof(server_addr));
	
	if (iResult == SOCKET_ERROR) {
		printf("connect failed with error: %d\n", WSAGetLastError());
		closesocket(client_socket);
		WSACleanup();
		return 1;
	}
	std::cout << "Connect socket server" << std::endl;
        
    // thread  -.> receve �ؼ� �б⸸

    // receive thead ����
    std::thread recvThread([&]()
        {
            fd_set read_fds;
            FD_ZERO(&read_fds);               // reads fd_set �ʱ�ȭ
            FD_SET(client_socket, &read_fds); // ���� ���� fd�� read_fds fd_set�����
            TIMEVAL timeout;
            timeout.tv_sec = 5;     // timeout �� ����
            timeout.tv_usec = 5000; // timeout ����ũ���� ����
			char buffer[BUFSIZE + 1];
            while (true)
            {
                fd_set cpy_reads = read_fds; // select�� ���� �ѼյǹǷ� �����ؼ� ���
                int selectStatus = select(FD_SETSIZE, &cpy_reads, NULL, NULL, &timeout);

                if (selectStatus == -1)
                {
                    printf("select failed with error: %d\n", WSAGetLastError());
                    break;
                }

                if (FD_ISSET(client_socket, &cpy_reads))
                { 

                  
                     retval = recv(client_socket, buffer, sizeof(buffer), 0);
                    if (retval < 0) // server���� ���� data����
                    {
                        printf("recv failed with error: %d\n", WSAGetLastError());
                        continue;
                    }
                    if (retval == 0)
                    { // data�� ����  server�� ���ᵵ ����
                        break;
                    }

					buffer[retval] = '\0';
					printf("[TCP Ŭ���̾�Ʈ] %d����Ʈ�� �޾ҽ��ϴ�.\n", retval);
					printf("[���� ������] %s\n", buffer);
                }
            } });


    while (true)
    {
		printf("\n[���� ������] ");
		std::string data;
		getline(std::cin, data);

		if (data == "exit")
		{
			send(client_socket, data.c_str(), data.length(), 0);
			break;
		}

		retval = send(client_socket, data.c_str(), data.length(), 0);

		//printf("[TCP Ŭ���̾�Ʈ] %d����Ʈ�� ���½��ϴ�.\n", retval);
    }
    
	    // recvThread ����
    recvThread.join();
    closesocket(client_socket);
	std::cout << "Connection closed" << std::endl;


    WSACleanup();
    return 0;
}

//
//	char buf[BUFSIZE + 1];
//	int len;
//
//	while (1) {
		/*printf("\n[���� ������] ");
		if (fgets(buf, BUFSIZE + 1, stdin) == NULL) break;

		len = strlen(buf);
		if (buf[len - 1] == '\n') buf[len - 1] = '\0';

		if (strlen(buf) == 0) break;

		retval = send(sock, buf, strlen(buf), 0);
		printf("[TCP Ŭ���̾�Ʈ] %d����Ʈ�� ���½��ϴ�.\n", retval);

		retval = recv(sock, buf, retval, 0);
		if (retval == 0) break;

		buf[retval] = '\0';
		printf("[TCP Ŭ���̾�Ʈ] %d����Ʈ�� �޾ҽ��ϴ�.\n", retval);
		printf("[���� ������] %s\n", buf);*/
//	}
//
//	closesocket(sock);
//
//	WSACleanup();
//	return 0;
//}