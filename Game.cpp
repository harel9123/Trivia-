#include "Game.h"

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

	_gameID = 0;
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

void Game::sendFirstQuestion()
{
	sendQuestionToAllUsers();
}

void Game::handleFinishGame()
{
	_db.updateGameStatus(_gameID);
	int i, length = _players.size(), num;

	string packet, name, paddedLen, score, excep = "";
	packet = to_string(GAME_OVER) + to_string(length);

	for (i = 0; i < length; i++)
	{
		name = _players[i]->getUsername();
		score = padNumbers(_results[name], 2);
		paddedLen = padNumbers(name.size(), 2);
		packet += paddedLen + name + score;
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

		sendQuestionToAllUsers();
		_currentTurnAnswers = 0;
	}
	return true;
}

bool Game::handleAnswerFromUser(User * user, int answerNo, int time)
{
	_currentTurnAnswers++;

	string answer = "";
	int correctIndex = _questions[_currQuestionIndex]->getCorrectAnswerIndex();
	int isCorrect = answerNo == correctIndex ? 1 : 0;

	string packet;
	packet = to_string(ANSWER_RELATE);
	if (isCorrect)
	{
		_results[user->getUsername()] += 1;
		packet += to_string(CORRECT);
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

	if (_currQuestionIndex + 1 == _questionsNo)
	{
		return false;
	}

	return handleNextTurn();
}

bool Game::leaveGame(User * user)
{
	vector<User *>::iterator it;
	int length, i;
	length = _players.size();

	for (it = _players.begin(); it != _players.end(); ++it)
	{
		if (*it == user)
		{
			_players.erase(it);
		}
	}
	return handleNextTurn();
}

int Game::getID()
{
	return _gameID;
}

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

void Game::sendQuestionToAllUsers()
{
	int i, length, usersNum;
	string packet;

	length = _questions.size();
	usersNum = _players.size();

	packet = buildQuestionPacket(_currQuestionIndex);
	for (i = 0; i < usersNum; i++)
	{
		_players[i]->send(packet);
	}
}

string Game::buildQuestionPacket(int questionNo)
{
	Question * q = _questions[questionNo];
	
	int len, i;
	string packet, str;
	string * answers = q->getAnswers();
	packet = to_string(SEND_QUESTION);
	
	str = q->getQuestion();
	len = str.size();
	packet += padNumbers(len, 3) + str;

	for (i = 0; i < 4; i++)
	{
		str = answers[i];
		len = str.size();
		packet += padNumbers(len, 3) + str;
	}

	return packet;
}

string Game::padNumbers(int num, int finishLength)
{
	int i;
	string number;
	number = to_string(num);
	for (i = 0; i < finishLength - number.size(); i++)
	{
		number = "0" + number;
	}
	return number;
}