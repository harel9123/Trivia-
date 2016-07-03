#include "Validator.h"
#include <iostream>

using namespace std;

bool Validator::isPasswordValid(string password)
{
	bool retVal = false;
	bool aUpper = false, aLower = false, aDigit = false;
	if (password.size() < 4)
	{
		return false;
	}
	
	if (('a' <= password[0] && password[0] <= 'z') ||
		('A' <= password[0] && password[0] <= 'Z'))
	{
		retVal = true;
	}
	else
	{
		return false;
	}

	for (int i = 0; i < password.size(); i++)
	{
		if (password[i] == ' ')
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
	bool retVal = false;
	if (userName.length())
	{
		retVal = true;
	}

	for (int i = 0; i < userName.size(); i++)
	{
		if (userName[i] == ' ')
		{
			retVal = false;
		}
	}
	if (('a' < userName[0] && userName[0] < 'z' ) || 
		('A' < userName[0] && userName[0] < 'Z'))
	{
		retVal = true;
	}

	return retVal;
}