#include "Question.h"

Question::Question(int id, string question, string correctAnswer, string answer2, string answer3, string answer4)
{
	_id = id;
	_question = question;
	srand((unsigned int)time(NULL));
	corrAnswerIndex = rand() % 4 + 1;

	for (int i = 0; i < 4; i++)
	{
		_answers[i] = "";
	}

	_answers[corrAnswerIndex] = correctAnswer;

	int j = 0;
	for (int i = 0; i < 4; i++)
	{
		if (i != corrAnswerIndex)
		{
			if (_answers[i] == ""  && j == 0)
			{
				_answers[i] = answer2;
				j++;
			}
			else if (_answers[i] == ""  && j == 1)
			{
				_answers[i] = answer3;
				j++;
			}
			else if (_answers[i] == ""  && j == 2)
			{
				_answers[i] = answer4;
				j++;
			}
		}
	}

}

string Question::getQuestion()
{
	return _question;
}

string* Question::getAnswers()
{
	return _answers;
}

int Question::getCorrectAnswerIndex()
{
	return corrAnswerIndex;
}

int Question::getId()
{
	return _id;
}