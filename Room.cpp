#include "Room.h"

/**
	*constructor of room
	*input : the variables of room
	*output : None
**/
Room::Room(int id, User * admin, string name, int maxUsers, int questionsNo, int questionTime)
{
	_id = id;
	_admin = admin;
	_name = name;
	_maxUsers = maxUsers;
	_questionsNo = questionsNo;
	_questionTime = questionTime;
}

/**
	*this function get user and try to join him to the room.
	*input : pointer to some user
	*output : true - succses join || false - failed
**/
bool Room::joinRoom(User * user)
{
	string excep = "";
	bool retVal = false;
	if ((signed int)_users.size() < _maxUsers)
	{
		_users.push_back(user); 
		retVal = true;
	}

	string packet = to_string(JOIN_ROOM_RESP);
	try
	{
		if (retVal == false)
		{
			packet += to_string(JOIN_ROOM_FULL);
		}
		else
		{
			packet += to_string(JOIN_ROOM_SUC) + Helper::getPaddedNumber(_questionsNo, 2) + Helper::getPaddedNumber(_questionTime, 2);
		}
		user->send(packet);
	}
	catch (exception e)
	{
		excep += string(e.what()) + "\n";
	}

	try
	{
		sendMessage(getUsersListMessage());
	}
	catch (exception e)
	{
		excep += string(e.what()) + "\n";
		exception e2(excep.c_str());
		throw(e2);
	}

	return retVal;
}

/**
	*this function get user and erase him from the vactor of users -
	the user leave the room and send message to the other.
	*input : pointer to some user
	*output : None
**/
void Room::leaveRoom(User *user)
{
	for (vector<User *>::iterator it = _users.begin(); it != _users.end(); ++it)
	{
		if (*it == user)
		{
			_users.erase(it); 
			break;
		}
	}

	string packet = to_string(LEAVE_ROOM_RESP), excep = "";
	try
	{
		user->send(packet);
	}
	catch (exception e)
	{
		excep += string(e.what()) + "\n";
	}

	try
	{
		sendMessage(user, getUsersListMessage());
	}
	catch (exception e)
	{
		excep += string(e.what()) + "\n";
		exception e2(excep.c_str());
		throw(e2);
	}
}

/**
	*this function get user and check if he is the admin -
		if he is close the room - use in clearRoom.
	*input : pointer to some user
	*output : id of the room - succses close || -1 - failed
**/
int Room::closeRoom(User *user)
{
	int retVal;
	if (user == _admin)
	{
		int i, length = _users.size();
		retVal = _id;
		for (i = 0; i < length; i++)
		{
			if (_users[i] != _admin)
			{
				_users[i]->clearRoom();
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

/**
	*this function create a string that presents
		the list of user(according to the protocol).
	*input : None
	*output : list of all the users.
**/
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

/**
	*this fuction return the user as a string.
	*input : pointer to vectors of all the users, pointer to some user
	*output : Users as string
**/
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

/**
	*this fuction try to use in the sendMessage without user(null)/send to everybody.
	*input : string meesage
	*output : None
**/
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

/**
	*this fuction try to send message to all the users except the exclude user.
	*input : pointer to exclude user, string message.
	*output : None.
**/
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