#include "Socket.h"

/**
	*This function constructs a Socket object, initialized the WSADATA, socket
	 configs and attaching port and IP to listen to.
	*Input: Port to listen to.
	*Output: None.
**/
Socket::Socket(int port)
{
	WSADATA wsa;

	cout << "\nInitialising Winsock..." << endl;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		cout << "Failed. Error Code : " << WSAGetLastError() << endl;
		return;
	}

	cout << "Initialised.\n" << endl;

	//Create a socket
	_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (_socket == INVALID_SOCKET)
	{
		cout << "Could not create socket : " <<  WSAGetLastError() << endl;
	}

	cout << "Socket created.\n" << endl;

	//Prepare the sockaddr_in structure
	_server.sin_family = AF_INET;
	_server.sin_addr.s_addr = INADDR_ANY;
	_server.sin_port = htons(port);
}

/**
	*This function will bind an ip and port.
	*Input: None.
	*Output: The returned number from bind function(a code).
**/
int Socket::socketBind()
{
	int bindRes;
	bindRes = bind(_socket, (struct sockaddr *)&_server, sizeof(_server));
	if (bindRes == SOCKET_ERROR)
	{
		cout << "Bind failed with error code : " << WSAGetLastError() << endl;
	}
	return bindRes;
}

/**
	*This function will config the listening.
	*Input: How many connections to listen to.
	*Output: The returned number from listen function(a code).
**/
int Socket::socketListen(int connections)
{
	return listen(_socket, connections);
}

/**
	*This function accepts a connection from a client.
	*Input: None.
	*Output: The created socket number.
**/
SOCKET Socket::socketAccept()
{
	cout << "Waiting for incoming connections..." << endl;

	int c = sizeof(struct sockaddr_in);
	SOCKET Socket;
	Socket = accept(_socket, (struct sockaddr *)&_client, &c);
	if (Socket == INVALID_SOCKET)
	{
		cout << "Accept failed with error code : " << WSAGetLastError() << endl;
	}

	cout << "Connection accepted" << endl;
	return Socket;
}

/**
	*This function sends the data it gets to the socket it gets.
	*Input: Socket number, data.
	*Output: The returned number from send function(number of bytes sent).
**/
int Socket::socketSend(SOCKET sockNum, string buffer)
{
	return send(sockNum, buffer.c_str(), buffer.size(), 0);
}

/**
	*This function receives the data in the buffer with the given socket.
	*Input: Socket number.
	*Output: The data that was stored in the buffer.
**/
string Socket::socketRecv(SOCKET sockNum)
{
	char server_reply[2000];
	int recv_size;
	if ((recv_size = recv(_socket, server_reply, 2000, 0)) == SOCKET_ERROR)
	{
		cout << "recv failed" << endl;
		return "";
	}
	string reply(server_reply);
	cout << "Reply received\n" << endl;
	return reply;
}

string Socket::socketRecv()
{
	char reply[2000];
	int result = recv(_socket, reply, 2000, 0);

	if (result == SOCKET_ERROR)
	{
		cout << "recv failed" << endl;
		return "";
	}
	string data(reply);
	return data;
}

/**
	*This function closes the socket - calls the distructor.
	*Input: None.
	*Output: None.
**/
void Socket::socketClose()
{
	Socket::~Socket();
}

/**
	*This function distructs the Socket object, closing the socket itself
	 and cleaning the WSA config.
	*Input: None.
	*Output: None.
**/
Socket::~Socket()
{
	closesocket(_socket);
	WSACleanup();
}