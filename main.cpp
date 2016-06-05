#include "TriviaServer.h"

int main()
{
	TriviaServer * t = new TriviaServer();
	t->server();
	delete t;
	return 0;
}