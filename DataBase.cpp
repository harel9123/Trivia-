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

	query = "SELECT * FROM t_questions;";
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
	string question, correctAns, A2, A3, A4;
	question = results["question"][num];
	correctAns = results["correct_ans"][num];
	A2 = results["ans2"][num];
	A3 = results["ans3"][num];
	A4 = results["ans4"][num];

	q = new Question(num + 1, question, correctAns, A2, A3, A4);

	return q;
}

vector<string> DataBase::getBestScores()
{
	map<string, int> scores = map<string, int>();
	vector<string> bestScores = vector<string>();
	string query = "SELECT username, is_correct FROM t_users;";

	if (execute(query) == false)
	{
		return bestScores;
	}

	buildScoresMap(scores);

	string name, highestName;
	int i, j, length = results["username"].size(), val, max;
	for (i = 0; i < 3; i++)
	{
		for (j = 0; j < length; j++)
		{
			name = results["username"][j];
			val = scores[name];

			if (val > max)
			{
				max = val;
				highestName = name;
			}
		}

		bestScores.push_back(highestName);
		bestScores.push_back(to_string(max));
		max = 0;

		scores.erase(highestName);
	}

	return bestScores;
}

void DataBase::buildScoresMap(map<string, int> scores)
{
	string name, highestName;
	int i, j, length = results["username"].size(), max = 0, val = 0;
	
	for (i = 0; i < length; i++)
	{
		name = results["username"][i];
		scores.insert(pair<string, int>(results["username"][j], 0));
	}
	
	for (i = 0; i < length; i++)
	{
		name = results["username"][j];
		val = scores[name];

		if (stoi(results["is_correct"][j]) == 1)
		{
			val++;
		}

		scores["username"] = val;
	}
}

vector<string> DataBase::getPersonalStatus(string username)
{
	std::pair<std::map<char, int>::iterator, bool> ret;
	map<string, int> tempMap = map<string, int>();
	vector<string> status = vector<string>();
	map<string, int> scores = map<string, int>();

	string query;
	query = "SELECT game_id, username, is_correct, answer_time FROM t_players_answers WHERE username = " + username + ";";

	if (execute(query) == false)
	{
		return status;
	}

	buildScoresMap(scores);

	int numOfQuestions, correctAns, wrongAns, gamesNum = 0, i;
	double answerTime = 0;

	numOfQuestions = results["username"].size();
	correctAns = scores[username];
	wrongAns = numOfQuestions - correctAns;

	for (i = 0; i < numOfQuestions; i++)
	{
		ret = tempMap.insert(pair<string, int>(results["game_id"][i], 0));
		if (ret.second != false)
		{
			gamesNum++;
		}
	}

	for (i = 0; i < numOfQuestions; i++)
	{
		answerTime += stoi(results["answer_time"][i]);
	}
	answerTime = answerTime / numOfQuestions;

	status.push_back(to_string(gamesNum));
	status.push_back(to_string(correctAns));
	status.push_back(to_string(wrongAns));
	status.push_back(to_string(answerTime));
	
	return status;
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
