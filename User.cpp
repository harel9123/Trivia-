#include "User.h"


User::User(string name, SOCKET sock)
{
	_username = name;
	_sock = sock;
}

void User::send(string message)
{
	try
	{
		Helper::sendData(_sock, message);
	}
	catch (exception e)
	{
		throw(e);
	}
}

void User::clearRoom()
{
	_currRoom = nullptr;
}

void User::clearGame()
{
	_currGame = nullptr;
}

bool User::createRoom(int roomId, string roomName, int maxUsers, int questionsNo, int questionTime)
{
	bool retVal = false;
	if (_currRoom == nullptr)
	{
		_currRoom = new Room(roomId, this, roomName, maxUsers, questionsNo, questionTime);
		if (_currRoom != nullptr)
		{
			send(to_string(CREATE_ROOM_RESP) + to_string(CREATE_ROOM_SUC));
			retVal = true;
		}
		else
		{
			send(to_string(CREATE_ROOM_RESP) + to_string(CREATE_ROOM_FAIL));
			retVal = false;
		}
	}
	else
	{
		send(to_string(CREATE_ROOM_RESP) + to_string(CREATE_ROOM_FAIL));
		retVal = false;
	}
	return retVal;
}

bool User::joinRoom(Room * newRoom)
{
	bool retVal = false;
	if (_currRoom != nullptr)
	{
		newRoom->joinRoom(this);
		if (_currRoom != nullptr)
		{
			retVal = true;
		}
	}
	return retVal;
}

void User::leaveRoom()
{
	if (_currRoom != nullptr)
	{
		_currRoom->leaveRoom(this);
	}
	_currRoom = nullptr;
}

int User::closeRoom()
{
	int retVal = -1;
	if (_currRoom != nullptr)
	{
		if (_currRoom->closeRoom(this) == _currRoom->getId()) //or != -1
		{
			retVal = _currRoom->getId();
			delete _currRoom;
			_currRoom = nullptr;
		}
	}
	return retVal;
}

bool User::leaveGame()
{
	bool retVal = false;
	if (_currGame != nullptr)
	{
		if (_currGame->leaveGame(this))
		{
			retVal = true;
			_currGame = nullptr;
		}
	}
	return retVal;
}