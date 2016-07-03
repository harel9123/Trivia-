#include "TriviaServer.h"

int main()
{
	TriviaServer * t = new TriviaServer();
	t->server();
	delete t;
	system("PAUSE");
	return 0;
}