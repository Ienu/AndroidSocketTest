#include <winsock2.h>
#include <windows.h>
#include <iostream>

#define BUFLEN 20

#pragma comment(lib, "ws2_32.lib")

using namespace std;

bool StartUp()
{
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;

	wVersionRequested = MAKEWORD(2, 2);

	err = WSAStartup(wVersionRequested, &wsaData);
	if(err != 0)
	{
		printf("WSAStartup failed with error: %d\n", err);  
        return false;
	}

	if (LOBYTE(wsaData.wVersion) != 2 
		|| HIBYTE(wsaData.wVersion) != 2) 
	{  
		//Tell the user that we could not find a usable WinSock DLL.  
		printf("Could not find a usable version of Winsock.dll\n");  
		WSACleanup();  
		return false;  
	}
	else
	{
		printf("The Winsock 2.2 dll was found okay\n");
	}
	return true;
}
bool CreateSocket(SOCKET & sockOut)  
{  
	int iFamily = AF_INET;  
	int iType = SOCK_STREAM; //tcp连接  
	int iProtocol = IPPROTO_TCP;  

	int iResult = 0;  

	sockOut = socket(iFamily, iType, iProtocol);  
	if(sockOut == INVALID_SOCKET)   
	{  
		printf("socket function failed with error = %d\n", WSAGetLastError() );  
		return false;  
	}  
	else 
	{  
		printf("socket function succeeded\n");   
	}  
	return true;  
}  
bool BinSocket(SOCKET sock)  
{  
	// The socket address to be passed to bind  
	sockaddr_in service;  
	service.sin_family = AF_INET;  
	service.sin_addr.s_addr = inet_addr("192.168.1.113"); //服务器IP  
	service.sin_port = htons(30000); //要使用的端口号  

	// Bind the socket.  
	int iResult = bind(sock, (SOCKADDR *) &service, sizeof (service));  
	if (iResult == SOCKET_ERROR) 
	{  
		wprintf(L"bind failed with error %u\n", WSAGetLastError());  
		closesocket(sock);  
		return false;  
	}  
	else  
	{
		wprintf(L"bind returned success\n");  
	}
	return true;  
}  
void Serve(SOCKET  clientSocket)  
{
	char msg[] = "rec\n";
	int iResult = 1;  
	do   
	{  
		send(clientSocket, msg, strlen(msg), 0);
		//Sleep(1);

		// success 14908
		/*int count = BUFLEN;
		char rec[BUFLEN];
		
		iResult = recv(clientSocket, rec, count, 0);

		cout << "recv = " << iResult << endl;

		cout << rec << endl;*/


	}while(iResult > 0);  
}

bool SetupServer()  
{  
	SOCKET sock = INVALID_SOCKET;  
	if(!CreateSocket(sock))
	{
		return false;  
	}
	if(!BinSocket(sock)) 
	{
		return false;  
	}

	if(listen(sock, SOMAXCONN) == SOCKET_ERROR)  
	{  
		printf("Listen failed with error: %ld\n", WSAGetLastError());  
		return false;  
	}

	SOCKET  clientSocket = INVALID_SOCKET;  

	// Accept a client socket  
	clientSocket = accept(sock, NULL, NULL);  
	if (clientSocket == INVALID_SOCKET)   
	{  
		printf("accept failed: %d\n", WSAGetLastError());  
		return false;  
	}  

	Serve(clientSocket);  
	closesocket(sock);  
	return true; 
}

int main()
{
	StartUp();
	SetupServer();
	return 0;
}