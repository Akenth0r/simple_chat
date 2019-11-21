#include "Client.h"

Client::Client()
{
	connectSocket = INVALID_SOCKET;
}

int Client::connect(const char* ip, u_short port)
{
	connectSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (connectSocket == INVALID_SOCKET)
	{
		cerr << "ERROR socket(): " << WSAGetLastError() << endl;
		WSACleanup();
		return 1;
	}

	sockaddr_in inAddr;
	inAddr.sin_family = AF_INET;
	inAddr.sin_port = htons(1488);
	inet_pton(AF_INET, ip, &inAddr.sin_addr.s_addr);
	if (::connect(connectSocket, (sockaddr*)&inAddr, sizeof(sockaddr)))
	{
		cerr << "ERROR connect(): " << WSAGetLastError() << endl;
		WSACleanup();
		return 1;
	}
	cout << "Successfully connected to " << ip << endl;
	isConnected = true;
	return 0;
}

int Client::start()
{
	char userInputBuf[512];
	char servBuf[512];
	thread t(&Client::tListenServerFunc, this);
	Sleep(5);
	while (isConnected)
	{
		cout << '>';
		cin.getline(userInputBuf, 512);
		send(connectSocket, userInputBuf, 512, 0);
	}
	t.join();
	closesocket(connectSocket);
	WSACleanup();
	return 0;
}

void Client::tListenServerFunc()
{
	char buf[512];
	while (isConnected)
	{
		int res;
		if ((res = recv(connectSocket, buf, 512, 0)) > 0)
			cout << buf;
		else if (res < 0)
			isConnected = false;
	}
}