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

}

bool Game::handleNextTurn()
{

}

bool Game::handleAnswerFromUser(User *, int, int)
{

}

bool Game::leaveGame(User *)
{

}

int Game::getID()
{

}

bool Game::insertGameToDB()
{
	bool result;
	result = _db.insertNewGame();
	return result;
}

void Game::sendQuestionToAllUsers()
{
	int i, j, length, usersNum;
	string packet;

	length = _questions.size();
	usersNum = _players.size();
	for (i = 0; i < length; i++)
	{
		packet = buildQuestionPacket(i);
		for (j = 0; j < usersNum; j++)
		{
			_players[j]->send(packet);
		}
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
	packet += padNumbers(len) + str;

	for (i = 0; i < 4; i++)
	{
		str = answers[i];
		len = str.size();
		packet += padNumbers(len) + str;
	}

	return packet;
}

string Game::padNumbers(int num)
{
	int i;
	string number;
	number = to_string(num);
	for (i = 0; i < 3 - number.size(); i++)
	{
		number = "0" + number;
	}
	return number;
}