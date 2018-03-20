#include "stdafx.h"
#include "ServerMain.h"

int main()
{
	CServerMain mainServer;

	bool shouldRun = mainServer.StartServer();

	while (shouldRun)
	{
		shouldRun = mainServer.RunServer();
	}

	mainServer.ShutDownServer();

	return 0;
}