#include "TriviaServer.h"

int TriviaServer::_roomIdSequence = 0;

void TriviaServer::server()
{
	bindAndListen();
	SOCKET soc;
	thread handleMessages(&handleReceivedMessages);
	handleMessages.detach();

	while (true)
	{
		soc = acceptConnection();
		thread t(&clientHandler, soc);
		t.detach();
	}
}

void TriviaServer::bindAndListen() throw()
{
	Socket s(8820);
	int result;
	result = s.socketBind();
	if (result == SOCKET_ERROR)
	{
		exception * e = new exception("Binding the socket has failed !");
		throw(e);
	}
	result = s.socketListen(1);
	if (result == SOCKET_ERROR)
	{
		exception * e = new exception("Listening to the socket has failed !");
		throw(e);
	}
	sockaddr_in server;
	int bindRes;
	_socket = s.getSocket();
}

SOCKET TriviaServer::acceptConnection() throw()
{
	int len = sizeof(struct sockaddr_in);
	SOCKET soc = accept(_socket, (struct sockaddr *)&_client, &len);
	if (soc == SOCKET_ERROR)
	{
		exception * e = new exception("Accepting connection has failed !");
		throw(e);
	}
	return soc;
}

void TriviaServer::clientHandler(SOCKET client)
{
	int msgCode;
	ReceivedMessage * msg;
	while (true)
	{
		msgCode = Helper::getMessageTypeCode(client);
		if (msgCode != 0 && msgCode != LEAVE_GAME_REQ && msgCode != CLOSING_GAME_REQ)
		msg = buildReceiveMessage(client, msgCode);

		switch (msgCode)
		{
			case LOGIN_CODE:
				handleSignin(msg);
				break;

			case SIGNOUT_CODE:
				handleSignout(msg);
				break;

			case SIGNUP_CODE:
				handleSignup(msg);
				break;

			case ROOM_LIST_REQ:
				handleGetRooms(msg);
				break;

			case ROOM_USER_REQ:
				handleGetUsersInRoom(msg);
				break;

			case ROOM_JOIN_REQ:
				handleJoinRoom(msg);
				break;

			case ROOM_LEAVE_REQ:
				handleLeaveRoom(msg);
				break;

			case CREATE_ROOM_REQ:
				handleCreateRoom(msg);
				break;

			case CLOSE_ROOM_REQ:
				handleCloseRoom(msg);
				break;

			case START_GAME_REQ:
				handleStartGame(msg);
				break;

			case ANSWER_CODE:
				handlePlayerAnswer(msg);
				break;

			case HIGHSCORE_REQ:
				handleGetBestScores(msg);
				break;

			case INFO_REQ:
				handleGetPersonalStatus(msg);
				break;

			case LEAVE_GAME_REQ:
				handleLeaveGame(msg);
				break;

			case CLOSING_GAME_REQ:

				break;
		}
	}
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
	ReceivedMessage * msg;
	while (true)
	{
		while (_queRcvMessages.empty());
		msg = _queRcvMessages.front();

	}
}

void TriviaServer::addReceivedMessage(ReceivedMessage * msg)
{

}

ReceivedMessage * TriviaServer::buildReceiveMessage(SOCKET client, int msgCode)
{
	ReceivedMessage * msg;
	msg = new ReceivedMessage(client, msgCode);
	vector<string> values = msg->getValues();

	int len;
	string value;

	switch (msgCode)
	{
		case LOGIN_CODE:
			len = Helper::getIntPartFromSocket(client, 2);
			value = Helper::getStringPartFromSocket(client, len);
			values.push_back(value);

			len = Helper::getIntPartFromSocket(client, 2);
			value = Helper::getStringPartFromSocket(client, len);
			values.push_back(value);
			break;

		case SIGNOUT_CODE:
			
			break;

		case SIGNUP_CODE:
			len = Helper::getIntPartFromSocket(client, 2);
			value = Helper::getStringPartFromSocket(client, len);
			values.push_back(value);

			len = Helper::getIntPartFromSocket(client, 2);
			value = Helper::getStringPartFromSocket(client, len);
			values.push_back(value);

			len = Helper::getIntPartFromSocket(client, 2);
			value = Helper::getStringPartFromSocket(client, len);
			values.push_back(value);
			break;

		case ROOM_LIST_REQ:
			handleGetRooms(msg);
			break;

		case ROOM_USER_REQ:
			value = Helper::getStringPartFromSocket(client, 4);
			values.push_back(value);
			break;

		case ROOM_JOIN_REQ:
			value = Helper::getStringPartFromSocket(client, 4);
			values.push_back(value);
			break;

		case ROOM_LEAVE_REQ:
			
			break;

		case CREATE_ROOM_REQ:
			len = Helper::getIntPartFromSocket(client, 2);
			value = Helper::getStringPartFromSocket(client, len);
			values.push_back(value);

			value = Helper::getStringPartFromSocket(client, 1);
			values.push_back(value);

			len = Helper::getIntPartFromSocket(client, 2);
			value = Helper::getStringPartFromSocket(client, len);
			values.push_back(value);

			len = Helper::getIntPartFromSocket(client, 2);
			value = Helper::getStringPartFromSocket(client, len);
			values.push_back(value);
			break;

		case CLOSE_ROOM_REQ:
			
			break;

		case START_GAME_REQ:
			
			break;

		case ANSWER_CODE:
			value = Helper::getStringPartFromSocket(client, 1);
			values.push_back(value);

			len = Helper::getIntPartFromSocket(client, 2);
			value = Helper::getStringPartFromSocket(client, len);
			values.push_back(value);
			break;

		case HIGHSCORE_REQ:
			
			break;

		case INFO_REQ:
			
			break;

		case LEAVE_GAME_REQ:
			
			break;

		case CLOSING_GAME_REQ:

			break;
	}
	return msg;
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