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
//		result : addrinfo 구조에 대한 포인터입니다.addrinfo 구조에는 주소 계열, IP 주소 및 포트 번호와 같은 네트워크 주소에 대한 정보가 포함되어 있습니다.
//		ptr : addrinfo 구조에 대한 또 다른 포인터입니다.
//		hints : addrinfo 유형의 변수입니다.addrinfo 변수는 getaddrinfo() 함수를 호출할 때 원하는 주소 정보를 지정하는 데 사용됩니다.
//		result 및 ptr 변수는 NULL로 초기화됩니다.이는 유효한 addrinfo 구조를 가리키지 않음을 의미합니다.힌트 변수는 기본값으로 초기화됩니다.
//		이 코드는 일반적으로 getaddrinfo() 함수 호출 결과를 저장하는 데 사용됩니다.getaddrinfo() 함수는 호스트 이름 또는 IP 주소와 서비스 이름 또는 포트 번호를 취하고 일치하는 주소에 대한 정보가 포함된 addrinfo 구조의 연결 목록을 반환합니다.
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
//		hints.ai_flags = AI_PASSIVE; //AI_PASSIVE: 소켓 주소는 바인딩 함수 호출에 사용됩니다.
//
//		// Resolve the server address and port
//		iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
//		if (iResult != 0) {
//			printf("getaddrinfo failed with error: %d\n", iResult);
//			WSACleanup();
//			return 1;
//		}
//
//		// client 연결 listen 을 위한 서버 소켓 생성 
//		ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
//		if (ListenSocket == INVALID_SOCKET) {
//			printf("socket failed with error: %ld\n", WSAGetLastError());
//			freeaddrinfo(result);//메모리 해제
//			WSACleanup();
//			return 1;
//		}
//
//		// 소켓 바인딩 (서버 소켓에 주소 할당)
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
//		//소켓 listen
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