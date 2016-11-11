#include "general.h"

int main()
{
	Server MyServer(1111, false);
	for (int i = 0; i < 100; i++)
	{
		MyServer.listenForNewConnection();
	}
	system("pause");
	return 0;
}