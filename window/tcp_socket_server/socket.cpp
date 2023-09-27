//
//#include <winsock2.h>
//#include <ws2tcpip.h>
//#include <stdlib.h>
//#include <stdio.h>
//
////#pragma comment (lib, "Ws2_32.lib")
//
//#define DEFAULT_BUFLEN 512
//#define DEFAULT_PORT "27015"
//
//
//
//int main(int argc, char* argv[]) {
//	{
//		WSADATA wsaData;
//
//		int iResult;
//
//		SOCKET ListenSocket = INVALID_SOCKET;
//		SOCKET ClientSocket = INVALID_SOCKET;
//
//
//		/*
//		result : addrinfo ������ ���� �������Դϴ�.addrinfo �������� �ּ� �迭, IP �ּ� �� ��Ʈ ��ȣ�� ���� ��Ʈ��ũ �ּҿ� ���� ������ ���ԵǾ� �ֽ��ϴ�.
//		ptr : addrinfo ������ ���� �� �ٸ� �������Դϴ�.
//		hints : addrinfo ������ �����Դϴ�.addrinfo ������ getaddrinfo() �Լ��� ȣ���� �� ���ϴ� �ּ� ������ �����ϴ� �� ���˴ϴ�.
//		result �� ptr ������ NULL�� �ʱ�ȭ�˴ϴ�.�̴� ��ȿ�� addrinfo ������ ����Ű�� ������ �ǹ��մϴ�.��Ʈ ������ �⺻������ �ʱ�ȭ�˴ϴ�.
//		�� �ڵ�� �Ϲ������� getaddrinfo() �Լ� ȣ�� ����� �����ϴ� �� ���˴ϴ�.getaddrinfo() �Լ��� ȣ��Ʈ �̸� �Ǵ� IP �ּҿ� ���� �̸� �Ǵ� ��Ʈ ��ȣ�� ���ϰ� ��ġ�ϴ� �ּҿ� ���� ������ ���Ե� addrinfo ������ ���� ����� ��ȯ�մϴ�.
//		*/
//		//struct addrinfo* result = NULL, * prt = NULL, hints;
//		struct addrinfo* result = NULL;
//		struct addrinfo hints;
//
//
//		int iSendResult;
//		char recvbuf[DEFAULT_BUFLEN];
//		int recvbuflen = DEFAULT_BUFLEN;
//
//		// Initialize Winsock
//		iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
//		if (iResult != 0) {
//			printf("WSAStartup failed: %d\n", iResult);
//			return 1;
//		}
//
//
//		ZeroMemory(&hints, sizeof(hints));
//		hints.ai_family = AF_INET;
//		hints.ai_socktype = SOCK_STREAM;
//		hints.ai_protocol = IPPROTO_TCP;
//		hints.ai_flags = AI_PASSIVE; //AI_PASSIVE: ���� �ּҴ� ���ε� �Լ� ȣ�⿡ ���˴ϴ�.
//
//		// Resolve the server address and port
//		iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
//		if (iResult != 0) {
//			printf("getaddrinfo failed with error: %d\n", iResult);
//			WSACleanup();
//			return 1;
//		}
//
//		// client ���� listen �� ���� ���� ���� ���� 
//		ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
//		if (ListenSocket == INVALID_SOCKET) {
//			printf("socket failed with error: %ld\n", WSAGetLastError());
//			freeaddrinfo(result);//�޸� ����
//			WSACleanup();
//			return 1;
//		}
//
//		// ���� ���ε� (���� ���Ͽ� �ּ� �Ҵ�)
//		iResult = bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);
//		if (iResult == SOCKET_ERROR) {
//			printf("bind failed with error: %d\n", WSAGetLastError());
//			freeaddrinfo(result);
//			closesocket(ListenSocket);
//			WSACleanup();
//			return 1;
//
//		}
//		freeaddrinfo(result);
//
//		//���� listen
//		iResult = listen(ListenSocket, SOMAXCONN);
//		if (iResult == SOCKET_ERROR) {
//			printf("listen failed with error: %d\n", WSAGetLastError());
//			closesocket(ListenSocket);
//			WSACleanup();
//			return 1;
//		}
//
//		// Accept a client socket
//		ClientSocket = accept(ListenSocket, NULL, NULL);
//		if (ClientSocket == INVALID_SOCKET) {
//			printf("accept failed with error: %d\n", WSAGetLastError());
//			closesocket(ListenSocket);
//			WSACleanup();
//			return 1;
//		}
//
//		// No longer need server socket
//		closesocket(ListenSocket);
//
//
//
//		// Receive until the peer shuts down the connection
//		do {
//
//			iResult = recv(ClientSocket, recvbuf, recvbuflen, 0);
//			if (iResult > 0) {
//				printf("Bytes received: %d\n", iResult);
//
//				// Echo the buffer back to the sender
//				iSendResult = send(ClientSocket, recvbuf, iResult, 0);
//				if (iSendResult == SOCKET_ERROR) {
//					printf("send failed with error: %d\n", WSAGetLastError());
//					closesocket(ClientSocket);
//					WSACleanup();
//					return 1;
//				}
//				printf("Bytes sent: %d\n", iSendResult);
//			}
//			else if (iResult == 0)
//				printf("Connection closing...\n");
//			else {
//				printf("recv failed with error: %d\n", WSAGetLastError());
//				closesocket(ClientSocket);
//				WSACleanup();
//				return 1;
//			}
//
//		} while (iResult > 0);
//
//		// shutdown the connection since we're done
//		iResult = shutdown(ClientSocket, SD_SEND);
//		if (iResult == SOCKET_ERROR) {
//			printf("shutdown failed with error: %d\n", WSAGetLastError());
//			closesocket(ClientSocket);
//			WSACleanup();
//			return 1;
//		}
//
//		// cleanup
//		closesocket(ClientSocket);
//		WSACleanup();
//
//		return 0;
//	}
//}