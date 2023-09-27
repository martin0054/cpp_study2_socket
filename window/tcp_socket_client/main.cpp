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
        
    // thread  -.> receve 해서 읽기만

    // receive thead 생성
    std::thread recvThread([&]()
        {
            fd_set read_fds;
            FD_ZERO(&read_fds);               // reads fd_set 초기화
            FD_SET(client_socket, &read_fds); // 서버 소켓 fd를 read_fds fd_set에등록
            TIMEVAL timeout;
            timeout.tv_sec = 5;     // timeout 초 정의
            timeout.tv_usec = 5000; // timeout 마이크로초 정의
			char buffer[BUFSIZE + 1];
            while (true)
            {
                fd_set cpy_reads = read_fds; // select시 원본 훼손되므로 복사해서 사용
                int selectStatus = select(FD_SETSIZE, &cpy_reads, NULL, NULL, &timeout);

                if (selectStatus == -1)
                {
                    printf("select failed with error: %d\n", WSAGetLastError());
                    break;
                }

                if (FD_ISSET(client_socket, &cpy_reads))
                { 

                  
                     retval = recv(client_socket, buffer, sizeof(buffer), 0);
                    if (retval < 0) // server에서 받은 data없음
                    {
                        printf("recv failed with error: %d\n", WSAGetLastError());
                        continue;
                    }
                    if (retval == 0)
                    { // data도 없고  server랑 연결도 끊김
                        break;
                    }

					buffer[retval] = '\0';
					printf("[TCP 클라이언트] %d바이트를 받았습니다.\n", retval);
					printf("[받은 데이터] %s\n", buffer);
                }
            } });


    while (true)
    {
		printf("\n[보낼 데이터] ");
		std::string data;
		getline(std::cin, data);

		if (data == "exit")
		{
			send(client_socket, data.c_str(), data.length(), 0);
			break;
		}

		retval = send(client_socket, data.c_str(), data.length(), 0);

		//printf("[TCP 클라이언트] %d바이트를 보냈습니다.\n", retval);
    }
    
	    // recvThread 종료
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
		/*printf("\n[보낼 데이터] ");
		if (fgets(buf, BUFSIZE + 1, stdin) == NULL) break;

		len = strlen(buf);
		if (buf[len - 1] == '\n') buf[len - 1] = '\0';

		if (strlen(buf) == 0) break;

		retval = send(sock, buf, strlen(buf), 0);
		printf("[TCP 클라이언트] %d바이트를 보냈습니다.\n", retval);

		retval = recv(sock, buf, retval, 0);
		if (retval == 0) break;

		buf[retval] = '\0';
		printf("[TCP 클라이언트] %d바이트를 받았습니다.\n", retval);
		printf("[받은 데이터] %s\n", buf);*/
//	}
//
//	closesocket(sock);
//
//	WSACleanup();
//	return 0;
//}