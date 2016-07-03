#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <map>

#include "User.h"
#include "Question.h"
#include "DataBase.h"

using namespace std;

class User;

class Game
{
	private:
		vector<Question *> _questions;
		vector<User *> _players;
		int _questionsNo;
		int _currQuestionIndex;
		int _gameID;
		DataBase &_db;
		map<string, int> _results;
		int _currentTurnAnswers;

		bool insertGameToDB();
		void sendQuestionToAllUsers();

	public:
		Game(const vector<User *>& players, int questionsNo, DataBase& db);
		~Game();

		void sendFirstQuestion();
		void handleFinishGame();
		bool handleNextTurn();
		bool handleAnswerFromUser(User * user, int answerNo, int time);

		string buildQuestionPacket(int questionNo);

		bool leaveGame(User *);
		int getID();
};