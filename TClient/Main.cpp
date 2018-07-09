#include "stdafx.h"
#include "ClientMain.h"
#include "Game.h"

#ifdef _DEBUG
#pragma comment(lib, "sfml-system-d")
#pragma comment(lib, "sfml-window-d")
#pragma comment(lib, "sfml-graphics-d")
#else
#pragma comment(lib, "sfml-system")
#pragma comment(lib, "sfml-window")
#pragma comment(lib, "sfml-graphics")
#endif

int main()
{
	CClientMain client;
	CGame game;

	game.Init();
	client.BindGame(game);

	bool shouldRun = client.StartClient();

	while (shouldRun && game.GetShouldRun())
	{
		game.Update();
		shouldRun = client.RunClient();
	}

	client.ShutDownClient();

	return 0;
}