#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <Windows.h>
#include <WinSock2.h>
#include <iostream>
#include "Server.h"

#pragma comment (lib, "Ws2_32.lib")


using namespace std;

WSADATA wsaData;
Server server;

int main()
{
	int iRes = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iRes)
	{
		cerr << "ERROR: WSAStartup failed: " << iRes << endl;
		return 1;
	}

	if (server.init("192.168.0.9", 1488))
		return 1;
	cout << "Server initialized successfully.\n";

	if (server.start())
		return 1;
}
