#pragma once

#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>
#include <map>

#include "sqlite3.h"
#include "Question.h"

using namespace std;

#define DATABASE_NAME "TriviaDB.db"

class DataBase
{
	public:
		DataBase();
		~DataBase();
		bool isUserExists(string username);
		bool addNewUser(string username, string password, string email);
		bool isUserAndPassMatch(string username, string password);
		vector<Question *> initQuestions(int questionsNo);
		vector<string> getBestScores();
		vector<string> getPersonalStatus(string username);
		int insertNewGame();
		bool updateGameStatus(int gameID);
		bool addAnswerToPlayer(int gameID, string username, int questionID, string answer, bool isCorrect, int answerTime);

	private:
		sqlite3 * db;		
		void buildScoresMap(map<string, int> scores);
		Question * createQuestion(int num);
		void clearTable();
		bool execute(string);
		string getCurrentDate();
};