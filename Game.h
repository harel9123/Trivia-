#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <map>

#include "DataBase.h"
#include "Question.h"
#include "User.h"

using namespace std;

class Game
{
	private:
		vector<Question *> _questions;
		vector<User *> _players;
		int _questionsNo;
		int _currQuestionIndex;
		DataBase &_db;
		map<string, int> _results;
		int _currentTurnAnswers;

		bool insertGameToDB();
		void initQuestionsFromDB();
		void sendQuestionToAllUsers();
	public:
		Game(const vector<User *>&, int, DataBase&);
		~Game();

		void sendFirstQuestion();
		void handleFinishGame();
		bool handleNextTurn();
		bool handleAnswerFromUser(User *, int, int);

		bool leaveGame(User *);
		int getID();
};