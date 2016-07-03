#include "Game.h"

/**
	*This function construct a Game object.
	*Input: Vector of players in the game, number of questions, database.
	*Output: None.
**/

Game::Game(const vector<User *>& players, int questionsNo, DataBase& db) :_db(db)
{
	int i, length;

	if (insertGameToDB() == false)
	{
		exception e("Inserting game to db has failed !");
		throw(e);
	}

	_players = players;
	string name;
	length = players.size();
	for (i = 0; i < length; i++)
	{
		players[i]->setGame(this);
		name = players[i]->getUsername();
		_results.insert(pair<string, int>(name, 0));
	}
	_questionsNo = questionsNo;
	_questions = _db.initQuestions(_questionsNo);

	_currentTurnAnswers = 0;
}

Game::~Game()
{
	int length, i;
	length = _questions.size();

	for (i = 0; i < length; i++)
	{
		delete _questions[i];
	}
	_questions.clear();

	_players.clear();
}

/**
	*This function call to the function sendQuestionToAllUsers
	*Input: None
	*Output: None
**/
void Game::sendFirstQuestion()
{
	sendQuestionToAllUsers();
}

/**
	*This function handles the finishGame event - changes the game state in the db and tells all the players the game is over.
	*Input: None.
	*Output: None.
**/
void Game::handleFinishGame()
{
	if (_players.size() == _currentTurnAnswers)
	{
		_db.updateGameStatus(_gameID);
		int i, length = _players.size();

		string packet, name, paddedLen, score, excep = "";
		packet = to_string(GAME_OVER) + to_string(length);

		for (i = 0; i < length; i++)
		{
			name = _players[i]->getUsername();
			score = Helper::getPaddedNumber(_results[name], 2);
			paddedLen = Helper::getPaddedNumber(name.size(), 2);
			packet += paddedLen + name + score;
		}

		for (i = 0; i < length; i++)
		{
			try
			{
				_players[i]->send(packet);
			}
			catch (exception e)
			{
				excep += string(e.what()) + "\n";
			}
		}

		for (i = 0; i < length; i++)
		{
			_players[i]->setGame(nullptr);
		}

		if (excep != "")
		{
			exception e(excep.c_str());
			throw(e);
		}
	}
}

/**
	*This function handles the NextTurn event - checks if the game is over and if so calls finishGame handler.
	*Input: None.
	*Output: Returns true upon success / else false.
**/
bool Game::handleNextTurn()
{
	if (_players.size() == 0)
	{
		handleFinishGame();
		return false;
	}

	if (_currentTurnAnswers == _players.size())
	{
		if (_currQuestionIndex + 1 == _questionsNo)
		{
			handleFinishGame();
			return false;
		}
		_currQuestionIndex++;

		try
		{
			sendQuestionToAllUsers();
		}
		catch (exception e)
		{
			throw(e);
		}
		_currentTurnAnswers = 0;
	}
	return true;
}

/**
	*This function handles the answer from the user - inserts the user answer to the db and updates his game score.
	*Input: User who sent the answer, answer choice number, time it took the user to answer.
	*Output: Upon succes returns true, else false.
**/
bool Game::handleAnswerFromUser(User * user, int answerNo, int time)
{
	string answer = "";
	int correctIndex = _questions[_currQuestionIndex]->getCorrectAnswerIndex();
	--answerNo;
	bool isCorrect = (answerNo == correctIndex);

	string packet;
	packet = to_string(ANSWER_RELATE);
	if (isCorrect)
	{
		_results[user->getUsername()] += 1;
		packet += to_string(CORRECT);
	}
	else
	{
		packet += to_string(INCORRECT);
	}

	if (answerNo != 5)
	{
		answer = _questions[_currQuestionIndex]->getAnswers()[answerNo];
	}
	_db.addAnswerToPlayer(_gameID, user->getUsername(), _questions[_currQuestionIndex]->getId(),
						  answer, isCorrect, time);
	try
	{
		user->send(packet);
	}
	catch (exception e)
	{
		throw(e);
	}

	_currentTurnAnswers++;

	return handleNextTurn();
}

/**
	*This function handles the is used when a user wants to leave the game.
	*Input: User that wants to leave the game.
	*Output: Upon successful leaving, returns true, else false.
**/
bool Game::leaveGame(User * user)
{
	vector<User *>::iterator it;
	int length;
	length = _players.size();

	for (it = _players.begin(); it != _players.end(); ++it)
	{
		if (*it == user)
		{
			_players.erase(it);
			break;
		}
	}
	return _questionsNo == _currQuestionIndex + 1;
}

/**
	*This function returns the game id.
	*Input: None.
	*Output: The game id.
**/
int Game::getID()
{
	return _gameID;
}

/**
	*This function inserts the game into the database.
	*Input: None.
	*Output: Upon success returns true, else false.
**/
bool Game::insertGameToDB()
{
	int result;
	result = _db.insertNewGame();
	if (result == -1)
	{
		return false;
	}
	_gameID = result;
	return true;
}

/**
	*This function sends the current question to all the users in the game.
	*Input: None.
	*Output: None.
**/
void Game::sendQuestionToAllUsers()
{
	int i, length, usersNum;
	string packet, excep = "";

	length = _questions.size();
	usersNum = _players.size();

	packet = buildQuestionPacket(_currQuestionIndex);
	for (i = 0; i < usersNum; i++)
	{
		try
		{
			_players[i]->send(packet);
		}
		catch (exception e)
		{
			excep += string(e.what());
		}
	}
	if (excep != "")
	{
		exception e(excep.c_str());
		throw(e);
	}
}

/**
	*This function builds a packet that presents a question according to the protocol.
	*Input: Index of question.
	*Output: Question packet as the protocol.
**/
string Game::buildQuestionPacket(int questionNo)
{
	Question * q = _questions[questionNo];
	
	int len, i;
	string packet, str;
	string * answers = q->getAnswers();
	packet = to_string(SEND_QUESTION);
	
	str = q->getQuestion();
	len = str.size();
	packet += Helper::getPaddedNumber(len, 3) + str;

	for (i = 0; i < 4; i++)
	{
		str = answers[i];
		len = str.size();
		packet += Helper::getPaddedNumber(len, 3) + str;
	}

	return packet;
}