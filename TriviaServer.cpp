#include "TriviaServer.h"

void TriviaServer::bindAndListen() throw()
{
	_socket = new Socket(8820);
	int result;
	result = _socket->socketBind();
	if (result == SOCKET_ERROR)
	{
		exception * e = new exception("Binding the socket has failed !");
		delete _socket;
		throw(e);
	}
	result = _socket->socketListen(1);
	if (result == SOCKET_ERROR)
	{
		exception * e = new exception("Listening to the socket has failed !");
		delete _socket;
		throw(e);
	}
}

void TriviaServer::accept()
{
	SOCKET soc = _socket->socketAccept();
	if (soc == SOCKET_ERROR)
	{
		exception * e = new exception("Accepting connection has failed !");
		delete _socket;
		throw(e);
	}
	//Add the user to _connectedUsers
}

void TriviaServer::clientHandler(SOCKET client)
{

}

void TriviaServer::safeDeleteUser(ReceivedMessage * msg)
{

}

User * TriviaServer::handleSignin(ReceivedMessage * msg)
{

}

bool TriviaServer::handleSignup(ReceivedMessage * msg)
{

}

void TriviaServer::handleSignout(ReceivedMessage * msg)
{

}

void TriviaServer::handleLeaveGame(ReceivedMessage * msg)
{

}

void TriviaServer::handleStartGame(ReceivedMessage * msg)
{

}

void TriviaServer::handlePlayerAnswer(ReceivedMessage * msg)
{

}

bool TriviaServer::handleCreateRoom(ReceivedMessage * msg)
{

}

bool TriviaServer::handleCloseRoom(ReceivedMessage * msg)
{

}

bool TriviaServer::handleJoinRoom(ReceivedMessage * msg)
{

}

bool TriviaServer::handleLeaveRoom(ReceivedMessage * msg)
{

}

void TriviaServer::handleGetUsersInRoom(ReceivedMessage * msg)
{

}

void TriviaServer::handleGetRooms(ReceivedMessage * msg)
{

}

void TriviaServer::handleGetBestScores(ReceivedMessage * msg)
{

}

void TriviaServer::handleGetPersonalStatus(ReceivedMessage * msg)
{

}

void TriviaServer::handleReceivedMessages()
{

}

void TriviaServer::addReceivedMessage(ReceivedMessage * msg)
{

}

ReceivedMessage * TriviaServer::buildReceiveMessage(SOCKET client_sock, int msgCode)
{

}

User * TriviaServer::getUserByName(string name)
{

}

User * TriviaServer::getUserBySocket(SOCKET sock)
{

}

Room * TriviaServer::getRoomById(int id)
{

}