#pragma once

#include <string>

using namespace std;

class Validator
{
	public:
		bool static isPasswordValid(string);
		bool static isUserNameValid(string);
};