#pragma once
#pragma warning(disable:4996)
#define _SCL_SECURE_NO_WARNINGS
#include <CommonNetworkIncludes.h>
#include <vector>
#include "NetMessageManager.h"
#include "NetMessageConnect.h"
#include <chrono>
#include <unordered_map>
#include "ServerGameObject.h"
#include <set>
#include "ServerWorld.h"

#define POSITION_FREQ 1.f/60.f

typedef std::chrono::high_resolution_clock::time_point timePoint;

class CServerMain
{
public:
	CServerMain();
	~CServerMain();
	bool StartServer();
	bool RunServer();
	void ShutDownServer();

private:

	void ConnectWith(CNetMessageConnect aConnectMsg, const sockaddr_in& aReturnAddress);
	void DisconnectClient(short aClientID);
	//void RecieveChatMessage(SProtocol aProtocol);
	bool VerifyClient(const SClient& aClient);

	void UpdateGameObjects();

	void UpdateTime();
	float myCurrentTimeDelta;
	float myTotalTime;
	timePoint myCurrentTimePoint;

	float myTimeSincePositionSend;

	SOCKET mySocket;
	sockaddr_in myLocalAddress;

	std::vector<SClient> myClients;
	std::vector<sockaddr_in> myAddressToClientLUT;
	std::unordered_map<short, CServerGameObject> myGameObjects;
	std::set<short> myIDsToRemove;
	float mySpawnTimer;
	short myAvailableID;

	CNetMessageManager myMessageManager;
	time_t myLatestPingTime;
	std::set<unsigned int> myRecievedGuaranteedMessages;

	CServerWorld myWorld;
};

