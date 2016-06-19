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
	_currGame = nullptr;
}

bool User::createRoom(int roomId, string roomName, int maxUsers, int questionsNo, int questionTime)
{
	//Room *room = Room(roomId, roomName, maxUsers, questionsNo, questionTime);

}

bool User::joinRoom(Room * room)
{

}

void User::leaveRoom()
{

}

void User::closeRoom()
{

}

bool User::leaveGame()
{

}