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

void TriviaServer::bindAndListen()
{
	Socket s(8820);
	int result;
	result = s.socketBind();
	if (result == SOCKET_ERROR)
	{
		exception e("Binding the socket has failed !");
		throw(e);
	}
	result = s.socketListen(1);
	if (result == SOCKET_ERROR)
	{
		exception e("Listening to the socket has failed !");
		throw(e);
	}
	sockaddr_in server;
	int bindRes;
	_socket = s.getSocket();
}

SOCKET TriviaServer::acceptConnection()
{
	int len = sizeof(struct sockaddr_in);
	SOCKET soc = accept(_socket, (struct sockaddr *)&_client, &len);
	if (soc == SOCKET_ERROR)
	{
		exception e ("Accepting connection has failed !");
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

		
	}
}

void TriviaServer::safeDeleteUser(ReceivedMessage * msg)
{
	try
	{
		SOCKET sock = msg->getSock();
		handleSignout(msg);
		closesocket(sock);
	}
	catch (exception e)
	{
		cout << e.what() << endl;
	}
}

User * TriviaServer::handleSignin(ReceivedMessage * msg)
{
	string message = to_string(SIGNIN_RESP);
	User * user = nullptr;
	vector<string> values = msg->getValues();
	string username = values[0];
	string password = values[1];
	bool res = _db.isUserAndPassMatch(username, password);
	if (res)
	{
		//Checking if the user is logged in.
		user = getUserByName(username);
		if (user != nullptr)
		{
			user = new User(username, msg->getSock());
			message += to_string(SIGNIN_SUCCESS);
		}
		else
		{
			message += to_string(SIGNIN_CONNECTED);
		}
	}
	else
	{
		message += to_string(SIGNIN_WRONG);
	}

	try
	{
		Helper::sendData(msg->getSock(), message);
	}
	catch (exception e)
	{
		throw(e);
	}

	return user;
}

bool TriviaServer::handleSignup(ReceivedMessage * msg)
{
	string message = to_string(SIGNUP_RESP);

	string username = msg->getValues()[0];
	string password = msg->getValues()[1];
	string email = msg->getValues()[2];

	bool flag = true;

	bool isValid = Validator::isUserNameValid(username);
	if (isValid)
	{
		isValid = Validator::isPasswordValid(password);
		if (isValid)
		{
			if (!_db.isUserExists(username))
			{
				_db.addNewUser(username, password, email);
				message += to_string(SIGNUP_SUCCESS);
			}
			else
			{
				message += to_string(USER_EXISTS);
				flag = false;
			}
		}
		else
		{
			message += to_string(PASS_ILLEGAL);
			flag = false;
		}
	}
	else
	{
		message += to_string(NAME_ILLEGAL);
		flag = false;
	}

	try
	{
		Helper::sendData(msg->getSock(), message);
	}
	catch (exception e)
	{
		throw(e);
	}

	return flag;
}

void TriviaServer::handleSignout(ReceivedMessage * msg)
{
	if (msg->getUser() != nullptr)
	{
		_connectedUsers.erase(msg->getSock());
		handleCloseRoom(msg);
		handleLeaveRoom(msg);
		handleLeaveGame(msg);
	}
}

void TriviaServer::handleLeaveGame(ReceivedMessage * msg)
{
	if (msg->getUser()->leaveGame())
	{
		delete msg->getUser()->getGame();
	}
}

void TriviaServer::handleStartGame(ReceivedMessage * msg)
{
	Game * game;
	Room * room = msg->getUser()->getRoom();
	vector<User *> users = room->getUsers();
	try
	{
		game = new Game(users, room->getQuestionsNo(), _db);
		delete room;
		game->sendFirstQuestion();
	}
	catch (exception e)
	{
		cout << e.what() << endl;
		string packet;
		packet = to_string(SEND_QUESTION) + to_string(SEND_QUESTION_FAILED);

		try
		{
			msg->getUser()->send(packet);
		}
		catch (exception e2)
		{
			cout << e2.what() << endl;
		}
	}
}

void TriviaServer::handlePlayerAnswer(ReceivedMessage * msg)
{
	User * user = msg->getUser();
	Game * game = user->getGame();
	vector<string> values = msg->getValues();

	int answerNo = stoi(values[0]), answerTime = stoi(values[1]);
	bool res;
	if (game != nullptr)
	{
		res = game->handleAnswerFromUser(user, answerNo, answerTime);
		if (res == false)
		{
			delete game;
		}
	}
}

bool TriviaServer::handleCreateRoom(ReceivedMessage * msg)
{
	bool retVal = false;
	User * user = msg->getUser();
	if (user != nullptr)
	{
		string roomName;
		int playersNum, questionsNum, questionTime;
		vector<string> values = msg->getValues();
		roomName = values[0];
		playersNum = stoi(values[1]);
		questionsNum = stoi(values[2]);
		questionTime = stoi(values[3]);

		_roomIdSequence++;
		retVal = user->createRoom(_roomIdSequence, roomName, playersNum, questionsNum, questionTime);
		if (retVal)
		{
			_roomsList.insert(pair<int, Room *>(_roomIdSequence, user->getRoom()));
		}
		else
		{
			_roomIdSequence--;
		}
	}
	return retVal;
}

bool TriviaServer::handleCloseRoom(ReceivedMessage * msg)
{
	User * user = msg->getUser();
	if (user == nullptr)
	{
		return false;
	}
	int roomID = user->getRoom()->getId();
	int res = user->closeRoom();

	if (res != -1)
	{
		_roomsList.erase(roomID);
		return true;
	}
	return false;
}

bool TriviaServer::handleJoinRoom(ReceivedMessage * msg)
{
	User * user = msg->getUser();
	if (user == nullptr)
	{
		return false;
	}
	int roomID = stoi(msg->getValues()[0]);
	Room * room = getRoomById(roomID);
	string packet;
	packet = to_string(JOIN_ROOM_RESP);
	if (room == nullptr)
	{
		packet += to_string(JOIN_ROOM_OTHER);
		user->send(packet);
		return false;
	}

	if (user->joinRoom(room) == false)
	{
		string packet;
		packet += to_string(JOIN_ROOM_FULL);
	}
	else
	{
		int questionNo = room->getQuestionsNo(), questionTime = room->getQuestionTime();
		packet += to_string(JOIN_ROOM_SUC) + to_string(questionNo) + to_string(questionTime);
	}

	try
	{
		user->send(packet);
	}
	catch (exception e)
	{
		cout << e.what() << endl;
	}

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
	int msgCode;
	while (true)
	{
		{
			lock_guard<mutex> lock(_mtxReceivedMessages);
			msg = _queRcvMessages.front();
			msgCode = msg->getMessageCode();

			User * user;
			SOCKET s;
			try
			{
				switch (msgCode)
				{
					case SIGNIN_CODE:
						user = handleSignin(msg);
						s = msg->getSock();
						_connectedUsers.insert(pair<SOCKET, User *>(s, user));
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
						//Don't know what to do
						break;
				}
			}
			catch (exception e)
			{
				cout << e.what() << endl;
			}
		}
	}
}

void TriviaServer::addReceivedMessage(ReceivedMessage * msg)
{
	_mtxReceivedMessages.lock();
	_queRcvMessages.push(msg);
	_mtxReceivedMessages.unlock();
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
		case SIGNIN_CODE:
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
	User * user = nullptr;
	try
	{
		user = _connectedUsers[sock];
	}
	catch (exception e){}

	return user;
}

Room * TriviaServer::getRoomById(int id)
{
	Room * room = nullptr;
	try
	{
		room = _roomsList[id];
	}
	catch (exception e){}

	return room;
}