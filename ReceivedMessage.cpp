#include "ReceivedMessage.h"

ReceivedMessage::ReceivedMessage(SOCKET sock, int messageCode)
{
	_sock = sock;
	_messageCode = messageCode;
}

ReceivedMessage::ReceivedMessage(SOCKET sock, int messageCode, vector<string> values)
{
	_sock = sock;
	_messageCode = messageCode;
	_values = vector<string>(values);
}