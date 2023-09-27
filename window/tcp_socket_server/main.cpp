#include <WinSock2.h>
#include <WS2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <string>

#pragma comment (lib, "Ws2_32.lib")

#ifndef INET_ADDRSTRLEN
#define INET_ADDRSTRLEN 16
#endif

#define SERVERPORT 9000
#define BUFSIZE 1024

struct Client
{
	SOCKET socket{};
	std::string name;
};


int main(int argc, char* argv[]) {


	WSADATA wsaData;
	SOCKET listen_socket = INVALID_SOCKET;
	SOCKET client_socket = INVALID_SOCKET;

	int iResult;

	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);

	if (iResult != 0) {
		printf("WSAStartup failed with error: %d\n", iResult);
		return 1;
	}

	//���ϻ���
	listen_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (listen_socket == INVALID_SOCKET) {
		printf("socket failed with error: %ld\n", WSAGetLastError());
		WSACleanup();
		return 1;
	}
	std::cout << "Creat socket" << std::endl;

	SOCKADDR_IN server_addr;
	ZeroMemory(&server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY); // �� IP�ּҸ� network���� order�� �����Ͽ� ���� 
	server_addr.sin_port = htons(SERVERPORT);

	//���� ���ε� /���Ͽ� �ּ��Ҵ�
	iResult = bind(listen_socket, (SOCKADDR*)&server_addr, sizeof(server_addr));
	if (iResult == SOCKET_ERROR) {
		printf("bind failed with error: %d\n", WSAGetLastError());
		closesocket(listen_socket);
		WSACleanup();
		return 1;
	}
	std::cout << "Bind socket" << std::endl;

	iResult = listen(listen_socket, SOMAXCONN); //�Ϻ� ���������� �ִ��� ť�� ũ��� ���ÿ� �޾Ƶ��� �� �ֵ����Ѵ�.

	if (iResult == SOCKET_ERROR) {
		printf("listen failed with error: %d\n", WSAGetLastError());
		closesocket(listen_socket);
		WSACleanup();
		return 1;
	}
	std::cout << "Listen socket" << std::endl;

	fd_set read_fds;
	FD_ZERO(&read_fds);               // reads fd_set �ʱ�ȭ
	FD_SET(listen_socket, &read_fds); // ���� ���� fd�� read_fds fd_set�����
	TIMEVAL timeout;
	timeout.tv_sec = 5;     // timeout �� ����
	timeout.tv_usec = 5000; // timeout ����ũ���� ����

	std::vector<Client> clients;

	SOCKADDR_IN client_address = { 0 };
	int client_address_len;
	char buffer[BUFSIZE+1], buffer1[BUFSIZE + 1];
	int retval;


	while (1) {
		fd_set cpy_reads = read_fds; // select�� ���� �ѼյǹǷ� �����ؼ� ���
		int selectStatus = select(FD_SETSIZE, &cpy_reads, NULL, NULL, &timeout);
		if (selectStatus == -1)
		{
			printf("select failed with error: %d\n", WSAGetLastError());
			break;
		}

		if (FD_ISSET(listen_socket, &cpy_reads))
		{

			client_address_len = sizeof(client_address);

			// accept client
			std::cout << "accept client " << std::endl;
			client_socket = accept(listen_socket, (sockaddr*)&client_address, &client_address_len);

			if (client_socket < 0)
			{
				printf("accept failed with error: %d\n", WSAGetLastError());
				continue;
				// exit(0);
			}

	
			FD_SET(client_socket, &read_fds); // ���� fd_set�� Ŭ���̾�Ʈ ���!!

			// vector<client> ��  client �߰�
			Client client;
			client.socket = client_socket;
			client.name = "c " + std::to_string(client_socket);
			clients.push_back(client);


		}
		// client sockets Ȯ��
		for (int i = 0; i < clients.size(); i++)
		{
			if (FD_ISSET(clients[i].socket, &cpy_reads))
			{
				// Receive data from the client

				retval = recv(clients[i].socket, buffer, sizeof(buffer)-1, 0);
				if (retval < 0)
				{
					printf("receive() client data error: %d\n", WSAGetLastError());
					return 1;
				}

				// recevie data�� ���ų� clinet ������ �������� ����͸����� ���� ����
				if (retval == 0)
				{
					FD_CLR(clients[i].socket, &read_fds);
					closesocket(clients[i].socket);
					clients.erase(clients.begin() + i);
					continue;
				}
				else
				{

					buffer[retval] = 0;
					std::cout << "[" << clients[i].name << "] [�����޼���] : " << buffer << std::endl;
					printf("\n[TCP %s : %d ] %s\n", inet_ntop(AF_INET, &client_address.sin_addr, buffer1, INET_ADDRSTRLEN),
						ntohs(client_address.sin_port), buffer1);

					// Echo the data back to the client
					retval = send(clients[i].socket, buffer, retval, 0);
				}
			}
		}
	}

	// Close the server socket
	for (auto client = clients.begin(); client != clients.end(); ++client)
		closesocket(client->socket);


	closesocket(listen_socket);
	WSACleanup();
	return 0;
}