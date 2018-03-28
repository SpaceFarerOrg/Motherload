#pragma once
#include <CommonNetworkIncludes.h>
#include "../TShared/NetMessageManager.h"
#include <thread>
#include "SFML/System/Clock.hpp"
#include <set>

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
	void DisconnectFromServer();

	void TryToConnect(const std::string& aUserName);

private:
	unsigned short myID;
	bool myIsConnectedToServer;
	SOCKET mySocket;
	sockaddr_in myLocalAddress;
	sockaddr_in myServerAddress;

	CNetMessageManager myMessageManager;

	std::atomic<bool> myShouldRun;
	std::string myInput;
	std::string myUserName;
	
	CGame* myGame;

	time_t myLatestRecievedMessageTime;

	sf::Clock myClock;
	float myPlayerUpdateTimer;

	std::set<unsigned int> myRecievedGuaranteedMessages;

	void SendPlayerData();
};
