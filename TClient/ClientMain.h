#pragma once
#include <CommonNetworkIncludes.h>
#include "../TShared/NetMessageManager.h"
#include <thread>

class CGame;

class CClientMain
{
public:
	CClientMain();
	~CClientMain();

	bool StartClient();
	bool RunClient();
	void ShutDownClient();

	void BindGame(CGame& aGameRef);

	void UpdateChatMode();

private:
	unsigned short myID;
	bool myIsConnectedToServer;
	SOCKET mySocket;
	sockaddr_in myLocalAddress;
	sockaddr_in myServerAddress;

	CNetMessageManager myMessageManager;

	std::atomic<bool> myShouldRun;
	std::string myInput;
	
	CGame* myGame;

};
