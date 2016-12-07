#include "general.h"

int main()
{
	Server MyServer(1111, false);
	MyServer.ListenForNewConnection();

	MyServer.HandleInput();

	system("pause");
	return 0;
}