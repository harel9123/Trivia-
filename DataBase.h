#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <sstream>
#include "sqlite3.h"
#include "Question.h"

using namespace std;

#define DATABASE_NAME "TriviaDB.db"

class DataBase
{
	public:
		DataBase();
		~DataBase();
		bool isUserExists(string);
		bool addNewUser(string, string, string);
		bool isUserAndPassMatch(string, string);
		vector<Question*> initQusestions(int);
		vector<string> getBestScores();
		vector<string> getPersonalStatus(string);
		int insertNewGame();
		bool updateGameStatus(int);
		bool addAnswerToPlayer(int, string, int, string, bool, int);

	private:
		sqlite3 * db;
		unordered_map<string, vector<string>> results;
		int static callBackCount(void*, int, char**, char**);
		int static callBackQuestions(void*, int, char**, char**);
		int static callBackBestScores(void*, int, char**, char**);
		int static callBackPersonalStatus(void*, int, char**, char**);
};

