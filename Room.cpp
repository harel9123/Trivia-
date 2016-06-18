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

void Room::leaveRoom(User *user)
{
	int i = 0;
	for (vector<User *>::iterator it = _users.begin(); it != _users.end(); ++it)
	{
		if (it.operator != *user)
		{
			_users.erase(_users.begin() + i); //need to take care about sendMessage because i didnt understand it
		}
		i++;
	}
}

int Room::closeRoom(User *user)
{
	int retVal;
	if (user == _admin)
	{
		//need to take care about sendMessage because i didnt understand it
		retVal = _id;
		for (int i = 0; i < _users.size(); i++)
		{
			if (_users[i] != _admin)
			{
				_users[i].clearRoom();
			}
		}
	}
	else
	{
		retVal = -1;
	}
	return retVal;
}

string Room::getUsersListMessage()
{

}

string Room::getUsersAsString(vector<User *>users, User *user)
{
	string usersStr;
	for (vector<User *>::iterator it = users.begin(); it != users.end(); ++it)
	{
		if (it.operator != *user)
		{
			usersStr = *it.operator*.getUsername(); //not sure
		}
	}
	return usersStr;
}

void Room::sendMessage(string)
{

}

void Room::sendMessage(User *, string)
{

}