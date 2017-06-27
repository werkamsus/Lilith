#include "general.h"

int main()
{
#if _DEBUG
	Server MyServer(1337, true);
#else
	Server MyServer(1337, true);
#endif
	MyServer.ListenForNewConnection();

	MyServer.HandleInput();

	system("pause");
	return 0;
}