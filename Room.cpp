#include "Room.h"

Room::Room(int id, User * admin, string name, int maxUsers, int questionsNo, int questionTime)
{
	_id =id;
	_admin = admin;
	_name = name;
	_maxUsers = maxUsers;
	_questionsNo = questionsNo;
	_questionTime = questionTime;
}

bool Room::joinRoom(User *)
{

}

void Room::leaveRoom(User *)
{

}

int Room::closeRoom(User *)
{

}

string Room::getUsersListMessage()
{

}

string Room::getUsersAsString(vector<User *>, User *)
{

}

void Room::sendMessage(string)
{

}

void Room::sendMessage(User *, string)
{

}