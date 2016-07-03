#include "Room.h"

Room::Room(int id, User * admin, string name, int maxUsers, int questionsNo, int questionTime)
{
	_id = id;
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

	try
	{
		sendMessage(getUsersListMessage());
	}
	catch (exception e)
	{
		throw(e);
	}

	return retVal;
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

		try
		{
			sendMessage(to_string(CLOSE_ROOM_RESP));
		}
		catch (exception e)
		{
			throw(e);
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
	string str = to_string(ROOM_USER_RESP) + to_string(_users.size());

	int i, length = _users.size();
	string name;
	for (i = 0; i < length; i++)
	{
		name = _users[i]->getUsername();
		str += Helper::getPaddedNumber(name.size(), 2);
		str += name;
	}
	return str;
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

void Room::sendMessage(string message)
{
	try
	{
		sendMessage(NULL, message);
	}
	catch (exception e)
	{
		throw (e);
	}
}

void Room::sendMessage(User *excludeUser, string message)
{
	string excep = "";
	for (vector<User *>::iterator it = _users.begin(); it != _users.end(); ++it)
	{
		if (*it != excludeUser)
		{
			try
			{
				(*it)->send(message);
			}
			catch (exception e)
			{
				excep += string(e.what()) + "\n";
			}
		}
	}

	if (excep != "")
	{
		exception e(excep.c_str());
		throw(e);
	}
}