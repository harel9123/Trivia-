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
	bool retVal = isUserExists(username);
	if (retVal == false)
	{
		return retVal;
	}

	string query, pass;
	query = "SELECT password FROM users WHERE username = " + username + ";";

	retVal = execute(query);
	if (retVal == false)
	{
		return retVal;
	}

	pass = results["password"][0];

	return pass == password;
}

vector<Question *> DataBase::initQuestions(int questionsNo)
{
	Question * question;
	vector<Question *> questions = vector<Question *>();
	int i, num, len;
	string query;

	srand((unsigned)time(0));

	query = "SELECT * FROM questions;";
	if (execute(query) == false)
	{
		return questions;
	}

	len = results["question"].size();

	for (i = 0; i < questionsNo; i++)
	{
		num = rand() % len;
		questions.push_back(createQuestion(num));
	}

	return questions;
}

Question * DataBase::createQuestion(int num)
{
	Question * q;
	string question, A1, A2, A3, A4;
	question = results["question"][num];
	A1 = results["correntAnswer"][num];
	A2 = results["answer2"][num];
	A3 = results["answer3"][num];
	A4 = results["answer4"][num];

	q = new Question(num + 1, question, A1, A2, A3, A4);

	return q;
}

vector<string> DataBase::getBestScores()
{
	vector<string> bestScores = vector<string>();
	string query = "SELECT username, correctAnswers FROM users ORDER BY correctAnswers ASC LIMIT 3;";

	if (execute(query) == false)
	{
		return bestScores;
	}

	int i;
	for (i = 0; i < 3; i++)
	{
		bestScores.push_back(results["username"][i]);
		bestScores.push_back(results["correctAnswers"][i]);
	}

	return bestScores;
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

void DataBase::clearTable()
{
	for (auto it = results.begin(); it != results.end(); ++it)
	{
		it->second.clear();
	}
	results.clear();
}

bool DataBase::execute(string query)
{
	clearTable();

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
