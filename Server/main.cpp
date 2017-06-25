#include "general.h"

int main()
{
#if _DEBUG
	Server MyServer(1111, true);
#else
	Server MyServer(47128, true);
#endif
	MyServer.ListenForNewConnection();

	MyServer.HandleInput();

	system("pause");
	return 0;
}