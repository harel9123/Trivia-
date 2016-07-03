#include "Question.h"

/**
	*constructor of Question - random the correct answer.
	*input : string of 4 diffrents answers, string question, int id
	*output : None
**/
Question::Question(int id, string question, string correctAnswer, string answer2, string answer3, string answer4)
{
	_id = id;
	_question = question;

	corrAnswerIndex = rand() % 4;

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
	cout << "correct = " << correctAnswer << " index: " << corrAnswerIndex << endl;
}

/**
	*this function return the current question
	*input : None
	*output : question string
**/
string Question::getQuestion()
{
	return _question;
}

/**
	*this function return the current answers
	*input : None
	*output : pointer to the string answers
**/
string* Question::getAnswers()
{
	return _answers;
}

/**
	*this function return the current correct answer index
	*input : None
	*output : correct answer index
**/
int Question::getCorrectAnswerIndex()
{
	return corrAnswerIndex;
}

/**
	*this function return the current id
	*input : None
	*output :  room id
**/
int Question::getId()
{
	return _id;
}