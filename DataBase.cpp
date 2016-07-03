#include "DataBase.h"

unordered_map<string, vector<string>> results;

/**
	*This function constructs a database object.
	*Input: None.
	*Output: None.
**/
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

	srand((unsigned int)time(NULL));
}

DataBase::~DataBase()
{
	sqlite3_close(db);
}

/**
	*This function checks if a given username exits in the database.
	*Input: Username to search for.
	*Output: If found, returns true, else false.
**/
bool DataBase::isUserExists(string username)
{
	cout << "Checking if " << username << " exists" << endl;

	bool retVal = true;
	string query;
	query = "SELECT username FROM t_users WHERE username = '" + username + "';";

	retVal = execute(query);

	if (results.size() != 0 && retVal)
	{
		return retVal;
	}
	return false;
}

/**
	*This function adds a new user to the users table.
	*Input: Users' username, password and email.
	*Output: Upon success returns true, else false.
**/
bool DataBase::addNewUser(string username, string password, string email)
{
	bool retVal = isUserExists(username);
	if (retVal)
	{
		return !(retVal);
	}

	string query;
	query = "INSERT INTO t_users VALUES('" + username + "', '" + password + "', '" + email + "');";

	retVal = execute(query);

	return retVal;
}

/**
	*This function checks if given username and password match in the database.
	*Input: Username and password to check match.
	*Output: If they match, returns true, else false.
**/
bool DataBase::isUserAndPassMatch(string username, string password)
{
	bool retVal = isUserExists(username);
	if (retVal == false)
	{
		return retVal;
	}

	string query, pass;
	query = "SELECT password FROM t_users WHERE username = '" + username + "';";

	retVal = execute(query);
	if (retVal == false)
	{
		return retVal;
	}

	pass = results["password"][0];

	return pass == password;
}

/**
	*This function initializes the questions vector.
	*Input: Number of questions.
	*Output: A vector of questions.
**/
vector<Question *> DataBase::initQuestions(int questionsNo)
{
	vector<Question *> questions = vector<Question *>();
	int i;
	string query;
	vector<int> questionNums;

	srand((unsigned)time(0));

	query = "SELECT * FROM t_questions ORDER BY RANDOM() LIMIT " + to_string(questionsNo) + ";";
	if (execute(query) == false)
	{
		return questions;
	}

	for (i = 0; i < questionsNo; i++)
	{
		questions.push_back(createQuestion(i));
	}

	return questions;
}

/**
	*This function creates a question.
	*Input: The number of the question to create.
	*Output: The created question.
**/
Question * DataBase::createQuestion(int num)
{
	int q_id;
	Question * q;
	string question, correctAns, A2, A3, A4;
	q_id = stoi(results["question_id"][num]);
	question = results["question"][num];
	correctAns = results["correct_ans"][num];
	A2 = results["ans2"][num];
	A3 = results["ans3"][num];
	A4 = results["ans4"][num];

	q = new Question(q_id, question, correctAns, A2, A3, A4);

	return q;
}

/**
	*This function returns a vector of the best scores in the order: username - score, username - score...
	*Input: None.
	*Output: A vector of all time best scores.
**/
vector<string> DataBase::getBestScores()
{
	map<string, int> scores = map<string, int>();
	vector<string> bestScores = vector<string>();

	string query = "SELECT username, is_correct FROM t_players_answers;";

	if (execute(query) == false)
	{
		return bestScores;
	}

	buildScoresMap(scores);

	string name, highestName;
	int i, j, length = results["username"].size(), val, max = 0;
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

/**
	*This function builds a map of all time users scores.
	*Input: A map to build.
	*Output: None.
**/
void DataBase::buildScoresMap(map<string, int> &scores)
{
	string name;
	int i, length = results["username"].size(), max = 0, val = 0;
	
	for (i = 0; i < length; i++)
	{
		name = results["username"][i];
		scores.insert(pair<string, int>(results["username"][i], 0));
	}
	
	for (i = 0; i < length; i++)
	{
		name = results["username"][i];
		val = scores[name];

		if (stoi(results["is_correct"][i]) == 1)
		{
			val++;
		}

		scores[name] = val;
	}
}

/**
	*This function returns the personal status of a given user.
	*Input: A username to get personal info about.
	*Output: A vector of the users personal status.
**/
vector<string> DataBase::getPersonalStatus(string username)
{
	std::pair<std::map<string, int>::iterator, bool> ret;
	map<string, int> tempMap = map<string, int>();
	vector<string> status = vector<string>();
	map<string, int> scores = map<string, int>();

	string query;
	query = "SELECT game_id, username, is_correct, answer_time FROM t_players_answers WHERE username = '" + username + "';";

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
	if (numOfQuestions == 0)
	{
		numOfQuestions = 1;
	}
	answerTime = answerTime / numOfQuestions;

	status.push_back(to_string(gamesNum));
	status.push_back(to_string(correctAns));
	status.push_back(to_string(wrongAns));
	status.push_back(to_string(answerTime));
	
	return status;
}

/**
	*This function creates a string of the current date, according to the format.
	*Input: None.
	*Output: Current date in string.
**/
string DataBase::getCurrentDate()
{
	time_t now;
	struct tm localTime;
	now = time(NULL);

	localTime = *localtime(&now);

	string date = to_string(localTime.tm_year + 1900) + "-" + to_string(localTime.tm_mon + 1) + "-" + to_string(localTime.tm_mday) + " ";
	date += to_string(localTime.tm_hour) + ":" + to_string(localTime.tm_min) + ":" + to_string(localTime.tm_sec);

	return date;
}

/**
	*This function inserts a game into the games table.
	*Input: None.
	*Output: Returns the id of the inserted game.
**/
int DataBase::insertNewGame()
{
	string query, date = getCurrentDate();
	query = "INSERT INTO t_games(status, start_time) VALUES(0, '" + date + "');";

	if (!execute(query))
	{
		return -1;
	}

	query = "SELECT game_id FROM t_games ORDER BY game_id DESC LIMIT 1;";
	if (!execute(query))
	{
		return -1;
	}
	int gameID = stoi(results["game_id"][0]);

	return gameID;
}

/**
	*This function updates a game status to over.
	*Input: Game id to change.
	*Output: Upon success returns true, else false.
**/
bool DataBase::updateGameStatus(int gameID)
{
	string query, date = getCurrentDate();
	query = "UPDATE t_games SET status = " + to_string(1) + ", end_time = '" + date + "' WHERE game_id = " + to_string(gameID) + ";";

	if (execute(query))
	{
		return false;
	}

	return true;
}

/**
	*This function adds to answers table the user answer.
	*Input: Answer properties.
	*Output: Upon success returns true, else false.
**/
bool DataBase::addAnswerToPlayer(int gameID, string username, int questionID, string answer, bool isCorrect, int answerTime)
{
	int correct = isCorrect ? 1 : 0;
	string query;
	query = "INSERT INTO t_players_answers VALUES(" + to_string(gameID) + ", '" + username + "', " + to_string(questionID) + ", '" + answer + "', " + to_string(correct) + ", " + to_string(answerTime) + ");";

	return execute(query);
}

/**
	*This function clears the results table.
	*Input: None.
	*Output: None.
**/
void DataBase::clearTable()
{
	for (auto it = results.begin(); it != results.end(); ++it)
	{
		it->second.clear();
	}
	results.clear();
}

/**
	*This function is being called after every query to the db.
	*Input: Some variables.
	*Output: Returns 0.
**/
int callback(void* notUsed, int argc, char** argv, char** azCol)
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

/**
	*This function executes a query in the db.
	*Input: The query.
	*Output: Upon success returns true, else false.
**/
bool DataBase::execute(string query)
{
	clearTable();
	cout << "QUERY: " << query << endl;
	int rc;
	char * errMsg;
	rc = sqlite3_exec(db, query.c_str(), callback, 0, &errMsg);

	if (rc != SQLITE_OK)
	{
		cout << "SQL Error: " << errMsg << endl;
		sqlite3_free(errMsg);
		return false;
	}
	return true;
}