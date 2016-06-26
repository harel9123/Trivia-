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

bool Room::joinRoom(User * user)
{
	bool retVal = false;
	if (_users.size() < _maxUsers)
	{
		_users.push_back(user); 
		retVal = true;
	}
	sendMessage(getUsersListMessage());
}

void Room::leaveRoom(User *user)
{
	for (vector<User *>::iterator it = _users.begin(); it != _users.end(); ++it)
	{
		if (*it == user)
		{
			_users.erase(it); 
		}
	}
	for (vector<User *>::iterator it = _users.begin(); it != _users.end(); ++it)
	{
		sendMessage(*it, getUsersListMessage());
	}
}

int Room::closeRoom(User *user)
{
	int retVal;
	if (user == _admin)
	{
		//need to take care about sendMessage because i didnt understand it
		retVal = _id;
		for (vector<User *>::iterator it = _users.begin(); it != _users.end(); ++it)
		{
			if (*it != _admin)
			{
				(*it)->clearRoom();
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
	string str = "180";
	str += _users.size();
	for (vector<User *>::iterator it = _users.begin(); it != _users.end(); ++it)
	{
		str += "##";
		str += (*it)->getUsername();
	}
}

string Room::getUsersAsString(vector<User *> users, User * user)
{
	string usersStr = "";
	for (vector<User *>::iterator it = users.begin(); it != users.end(); ++it)
	{
		if (*it != user)
		{
			usersStr += (*it)->getUsername(); //not sure
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