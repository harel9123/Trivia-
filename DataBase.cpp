#include "DataBase.h"

DataBase::DataBase()
{
	int rc;
	sqlite3 * db;
	char * zErrMsg = 0;
	string name;
	int peopleCounter = 0, i = 0;

	rc = sqlite3_open(DATABASE_NAME, &db);

	if (rc)
	{
		sqlite3_close(db);
		string s = "Can't open database: " + string(sqlite3_errmsg(db));
		exception e(s.c_str());
		throw(e);
	}
}

DataBase::~DataBase()
{
	
}

bool DataBase::isUserExists(string)
{

}

bool DataBase::addNewUser(string, string, string)
{

}

bool DataBase::isUserAndPassMatch(string, string)
{

}

vector<Question*> DataBase::initQusestions(int)
{

}

vector<string> DataBase::getBestScores()
{

}

vector<string> DataBase::getPersonalStatus(string)
{

}

int DataBase::insertNewGame()
{

}

bool DataBase::updateGameStatus(int)
{

}

bool DataBase::addAnswerToPlayer(int, string, int, string, bool, int)
{

}

int static callBackCount(void*, int, char**, char**)
{

}

int static callBackQuestions(void*, int, char**, char**)
{

}

int static callBackBestScores(void*, int, char**, char**)
{

}

int static callBackPersonalStatus(void*, int, char**, char**)
{

}
