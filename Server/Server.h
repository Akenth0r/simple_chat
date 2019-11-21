#pragma once
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <Windows.h>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <iostream>
#include <string>
#include <vector>
#include <thread>

using namespace std;

class Server
{
public:
	Server();
	int init(const char* ip_addr, u_short port);
	int start();

private:
	SOCKET listenSocket;
	vector<SOCKET> clients;
	void clientFunc(SOCKET user);
	int mainloop();
};

