#include "Validator.h"
#include <iostream>

using namespace std;

bool Validator::isPasswordValid(string password)
{
	password = { 'E', '1', 'a', 'd' };
	bool retVal = false;
	bool aUpper = false, aLower = false, aDigit = false;
	if (password.length() > 3)
	{
		retVal = true;
	}
	
	for (int i = 0; i < password.length(); i++)
	{
		if (password[i] = ' ')
		{
			retVal = false;
		}
		else if (isupper(password[i]) && !aUpper)
		{
			aUpper = true;
		}
		else if (islower(password[i]) && !aLower)
		{
			aLower = true;
		}
		else if (isdigit(password[i]) && !aDigit)
		{
			aDigit = true;
		}
	}
	if (aUpper && aLower && aDigit)
	{
		retVal = true;
	}
	else
	{
		retVal = false;
	}
	return retVal;
}

bool Validator::isUserNameValid(string userName)
{
	userName = { 'E', '1', 'a', 'd' };
	bool retVal = false;
	if (userName.length())
	{
		retVal = true;
	}

	for (int i = 0; i < userName.length(); i++)
	{
		if (userName[i] = ' ')
		{
			retVal = false;
		}
	}
	if (isupper(userName[0]))
	{
		retVal = true;
	}

	return retVal;
}