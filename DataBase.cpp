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

	bool retVal = true;
	string query;
	query = "SELECT username FROM users WHERE username = " + username + ";";

	retVal = execute(query);

	if (results.size() != 0 && retVal)
	{
		return retVal;
	}
	return false;
}

bool DataBase::addNewUser(string username, string password, string email)
{
	bool retVal = isUserExists(username);
	if (retVal)
	{
		return !(retVal);
	}

	string query;
	query = "INSERT INTO users VALUES(" + username + ", " + password + ", " + email + ");";

	retVal = execute(query);

	return retVal;
}

bool DataBase::isUserAndPassMatch(string username, string password)
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

bool DataBase::execute(string query)
{
	int rc;
	char * errMsg;
	rc = sqlite3_exec(db, query.c_str(), this->callback, 0, &errMsg);

	if (rc != SQLITE_OK)
	{
		cout << "SQL Error: " << errMsg << endl;
		sqlite3_free(errMsg);
		return false;
	}
	return true;
}

int DataBase::callback(void* notUsed, int argc, char** argv, char** azCol)
{
	int i;

	for (i = 0; i < argc; i++)
	{
		auto it = results.find(azCol[i]);
		if (it != results.end())
		{
			it->second.push_back(argv[i]);
		}
		else
		{
			pair<string, vector<string>> p;
			p.first = azCol[i];
			p.second.push_back(argv[i]);
			results.insert(p);
		}
	}

	return 0;
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
