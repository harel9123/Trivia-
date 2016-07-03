#include "ReceivedMessage.h"

/**
	*constructor of ReceivedMessage.
	*input : socket, meesage code
	*output : None
**/
ReceivedMessage::ReceivedMessage(SOCKET sock, int messageCode)
{
	_sock = sock;
	_messageCode = messageCode;
}

/**
	*anthor constructor of ReceivedMessage.
	*input : socket, meesage code, vector of string- the values
	*output : None
**/
ReceivedMessage::ReceivedMessage(SOCKET sock, int messageCode, vector<string> values)
{
	_sock = sock;
	_messageCode = messageCode;
	_values = vector<string>(values);
}