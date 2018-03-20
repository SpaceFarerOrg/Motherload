#pragma once
#pragma warning(disable:4996)
#define _SCL_SECURE_NO_WARNINGS
#include <CommonNetworkIncludes.h>
#include <vector>
#include "NetMessageManager.h"
#include "NetMessageConnect.h"

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

	SOCKET mySocket;
	sockaddr_in myLocalAddress;

	std::vector<SClient> myClients;
	std::vector<sockaddr_in> myAddressToClientLUT;

	CNetMessageManager myMessageManager;
	time_t myLatestPingTime;
};

