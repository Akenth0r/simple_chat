#include "Server.h"

Server::Server()
{
	listenSocket = INVALID_SOCKET;
}

int Server::init(const char* ip_addr, u_short port)
{
	sockaddr_in info;
	inet_pton(AF_INET, ip_addr, &info.sin_addr.s_addr);
	info.sin_family = AF_INET;
	info.sin_port = htons(port);

	// Create server socket
	listenSocket = socket(info.sin_family, SOCK_STREAM, IPPROTO_TCP);
	if (listenSocket == INVALID_SOCKET)
	{
		cerr << "ERROR socket(): " << WSAGetLastError() << endl;
		WSACleanup();
		return 1;
	}

	// Bind socket
	int bindRes = bind(listenSocket, ((SOCKADDR*)&info), sizeof(info));
	if (bindRes == SOCKET_ERROR)
	{
		cerr << "ERROR bind(): " << WSAGetLastError() << endl;
		closesocket(listenSocket);
		WSACleanup();
		return 1;
	}

	return 0;

}

int Server::start()
{
	// Start listening
	int lisRes = listen(listenSocket, SOMAXCONN);
	if (lisRes == SOCKET_ERROR)
	{
		cerr << "ERROR listen(): " << WSAGetLastError() << endl;
		closesocket(listenSocket);
		WSACleanup();
		return 1;
	}
	
	// Start main loop
	cout << "Start listening...\n";
	mainloop();

}

int Server::mainloop()
{
	const char* msg = "Hello, user!\n";
	char adrbuf[32];
	while (true)
	{
		sockaddr_in from;
		int fromlen = sizeof(from);
		SOCKET user = accept(listenSocket, (sockaddr*)&from, &fromlen);
		if (user != INVALID_SOCKET)
		{
			cout << "connect from " << inet_ntop(from.sin_family, &from.sin_addr, adrbuf, 32) << endl;
			send(user, msg, 14, 0);
			thread t(&Server::clientFunc, this, user);
			t.join();
		}
		//closesocket(user);
	}
}

void Server::clientFunc(SOCKET user)
{
	char buf[512];
	while (true)
	{
		int res;
		if ((res = recv(user, buf, 512, 0)) > 0)
		{
			//buf[res] = '\0';
			cout << buf << endl;
		}
		else if (res < 0)
			break;
	}
	closesocket(user);
}