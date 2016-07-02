#pragma once

#include <iostream>
#include <string>
#include <vector>

#include "User.h"

class User;

class Room
{
	private:
		vector<User *> _users;
		User * _admin;
		int _maxUsers;
		int _questionTime;
		int _questionsNo;
		string _name;
		int _id;

		string getUsersAsString(vector<User *>, User *);
		void sendMessage(string);
		void sendMessage(User *, string);

	public:
		Room(int id, User * admin, string name, int maxUsers, int questionsNo, int questionTime);

		bool joinRoom(User *);
		void leaveRoom(User *);
		int closeRoom(User *);

		string getUsersListMessage();
		vector<User *> getUsers() { return _users; };
		int getQuestionsNo(){ return _questionsNo; };
		int getQuestionTime(){ return _questionTime; };
		int getId(){ return _id; };
		string getName() { return _name; };
};