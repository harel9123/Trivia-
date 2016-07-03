#pragma once

#include <iostream>
#include <string>
#include <time.h>

using namespace std;

class Question
{
	public:
		Question(int id, string question, string correctAnswer, string answer2, string answer3, string answer4);
		string getQuestion();
		string* getAnswers();
		int getCorrectAnswerIndex();
		int getId();

	private:
		string _question;
		string _answers[4];
		int corrAnswerIndex;
		int _id;

};


