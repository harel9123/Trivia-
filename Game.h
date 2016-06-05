#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <map>

//#include "DataBase.h" //Add after creation
//#include "Question.h" //Add after creation
#include "User.h"

using namespace std;

class Game
{
	private:
		vector<string> _questions;//string --> Question *
		vector<User *> _players;
		int _questionsNo;
		int _currQuestionIndex;
		string &_db;//string --> DataBase
		map<string, int> _results;
		int _currentTurnAnswers;

		bool insertGameToDB();
		void initQuestionsFromDB();
		void sendQuestionToAllUsers();
	public:
		Game(const vector<User *>&, int, string&);//string --> DataBase
		~Game();

		void sendFirstQuestion();
		void handleFinishGame();
		bool handleNextTurn();
		bool handleAnswerFromUser(User *, int, int);

		bool leaveGame(User *);
		int getID();
};