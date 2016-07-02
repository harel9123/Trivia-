#pragma once

#include <iostream>
#include <string>

#include "SocketsInclude.h"

using namespace std;

class Socket
{
	public:
		Socket(int port);
		Socket(SOCKET sock, int flag);
		~Socket();
		int socketBind();
		int socketListen(int connections);
		SOCKET socketAccept();
		int socketSend(SOCKET sockNum, string buffer);
		int socketSend(string buffer);
		string socketRecv(SOCKET sockNum);
		string socketRecv();
		void socketClose();

		SOCKET getSocket() { return _socket; };
	private:
		sockaddr_in _server;
		SOCKET _socket;
};