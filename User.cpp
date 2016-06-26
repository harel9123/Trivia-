#include "User.h"
#include "Helper.h"
#include "Room.h"


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
	//User *u;
	//Room _currRoom(roomId, u, roomName, maxUsers, questionsNo, questionTime);

}

bool User::joinRoom(Room * newRoom)
{
	bool retVal = false;
	if (_currRoom != nullptr)
	{
		newRoom->joinRoom(); //to fix in the class
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
		_currRoom->leaveRoom(); //to fix in the class
	}
	_currRoom = nullptr;
}

void User::closeRoom()
{

}

bool User::leaveGame()
{
	bool retVal = false;
	if (_currGame != nullptr)
	{
		_currGame->leaveGame();//to fix in the class
	}
	_currGame = nullptr;
}