#include "DataBase.h"

DataBase::DataBase()
{
	int rc;

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

bool DataBase::isUserExists(string username)
{
	cout << "Checking if " << username << " exists" << endl;

	int rc;
	string query;
	char * errMsg;
	query = "SELECT COUNT(username) FROM users WHERE username = " + username;

	rc = sqlite3_exec(db, query.c_str(), callBackCount, 0, &errMsg);

	if (rc != SQLITE_OK)
	{
		cout << "SQL Error: " << errMsg << endl;
		sqlite3_free(errMsg);
		return false;
	}
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
