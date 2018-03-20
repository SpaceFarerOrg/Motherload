#pragma warning(disable:4996)
#define _SCL_SECURE_NO_WARNINGS
#include "stdafx.h"
#include "ServerMain.h"
#include <iostream>
#include <string>
#include "Utilities.h"
#include "NetMessageChatMessage.h"
#include "NetDisconnectMessage.h"
#include "NetMessagePing.h"
#include "NetMessagePosition.h"

CServerMain::CServerMain()
{
	myCurrentTimeDelta = 0;
	myTimeSincePositionSend = 0;
	myCurrentTimePoint = std::chrono::high_resolution_clock::now();
}


CServerMain::~CServerMain()
{
}

bool CServerMain::StartServer()
{
	PRINT("Starting up server...");

	int error;

	WSADATA data = {};

	error = WSAStartup(MAKEWORD(2, 2), &data);

	if (error != 0)
	{
		PRINT("WSAStartup failed with error code: " + std::to_string(error));
		return false;
	}

	PRINT("Server startup successful!");

	myLocalAddress.sin_family = AF_INET;
	myLocalAddress.sin_port = htons(53000);
	myLocalAddress.sin_addr.s_addr = INADDR_ANY;

	mySocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	//Set as non blocking because fuckall
	unsigned long mode = 1;
	ioctlsocket(mySocket, FIONBIO, &mode);

	int fel = bind(mySocket, (sockaddr*)&myLocalAddress, sizeof(myLocalAddress));

	PRINT("Listening for new connections...");

	myMessageManager.Init(256, mySocket);

	myLatestPingTime = time(0);

	return true;
}

bool CServerMain::RunServer()
{
	UpdateTime();

	myTimeSincePositionSend += myCurrentTimeDelta;

	if (myTimeSincePositionSend >= POSITION_FREQ)
	{
		for (SClient& c : myClients)
		{
			CNetMessagePosition::SPositionMessageData data;

			data.myTargetID = TO_ALL - c.myID;
			data.myX = c.myX;
			data.myY = c.myY;

			myMessageManager.CreateMessage<CNetMessagePosition>(data);
		}

		myTimeSincePositionSend -= POSITION_FREQ;
	}
	
	bool shouldContinueToRun = true;

	sockaddr_in from;
	int fromlen = sizeof(from);

	char buff[MAX_BUFFER_SIZE];
	ZeroMemory(buff, MAX_BUFFER_SIZE);
	if (recvfrom(mySocket, buff, MAX_BUFFER_SIZE, 0, (sockaddr*)&from, &fromlen) != SOCKET_ERROR)
	{
		size_t buffIndex = 0;
		EMessageType id = static_cast<EMessageType>(buff[buffIndex]);

		switch (id)
		{
		case EMessageType::Connect:
		{
			CNetMessageConnect rec;
			rec.RecieveData(buff, sizeof(SNetMessageConnectData));
			rec.UnpackMessage();
			ConnectWith(rec, from);
		}
		break;
		case  EMessageType::Disconnect:
		{
			CNetDisconnectMessage rec;
			rec.RecieveData(buff, sizeof(CNetMessage::SNetMessageData));
			rec.UnpackMessage();

			SNetMessageChatMessageData sendData;
			sendData.myTargetID = TO_ALL - rec.GetData().mySenderID;
			sendData.myMessage = myClients[rec.GetData().mySenderID - 1].myName + " disconnected";

			DisconnectClient(rec.GetData().mySenderID);

			myMessageManager.CreateMessage<CNetMessageChatMessage>(sendData);
		}
		break;
		case EMessageType::Chat:
		{
			CNetMessageChatMessage rec;
			rec.RecieveData(buff, sizeof(SNetMessageChatMessageData));
			rec.UnpackMessage();
			PRINT(myClients[rec.GetData().mySenderID-1].myName + ": " + rec.GetMessage());

			SNetMessageChatMessageData chatMsgData;
			chatMsgData.myMessage = myClients[rec.GetData().mySenderID-1].myName + ": " + rec.GetMessage();
			chatMsgData.myTargetID = TO_ALL - rec.GetData().mySenderID;
			
			myMessageManager.CreateMessage<CNetMessageChatMessage>(chatMsgData);
		}
		break;
		case EMessageType::Position:
		{
			CNetMessagePosition rec;
			rec.RecieveData(buff, sizeof(CNetMessagePosition::SPositionMessageData));
			rec.UnpackMessage();

			float x,y;
			rec.GetPosition(x, y);
			
			//Update the position of the client in world!
			myClients[rec.GetData().mySenderID].myX = x;
			myClients[rec.GetData().mySenderID].myY = y;
		}
		break;
		}
	}

	time_t currentTime = time(nullptr);
	if (currentTime - myLatestPingTime >= 1)
	{
		myLatestPingTime = currentTime;
		myMessageManager.CreateMessage<CNetMessagePing>(CNetMessage::SNetMessageData());
	}

	int errorCode = WSAGetLastError();

	myMessageManager.Flush(myAddressToClientLUT);

	return shouldContinueToRun;
}

void CServerMain::ShutDownServer()
{
	closesocket(mySocket);
	WSACleanup();
}

void CServerMain::ConnectWith(CNetMessageConnect aConnectMessage, const sockaddr_in& aReturnAddress)
{
	SClient newClient;
	newClient.myName = aConnectMessage.GetClientName();

	PRINT("Client " + newClient.myName + " connected");

	if (VerifyClient(newClient))
	{
		myClients.push_back(newClient);
		myClients.back().myID = myClients.size();
		myAddressToClientLUT.push_back(aReturnAddress);

		SNetMessageConnectData data;
		data.myTargetID = myClients.size();
		data.myClientConnectName = "OK";
		myMessageManager.CreateMessage<CNetMessageConnect>(data);

		SNetMessageChatMessageData welcomeMsg;
		welcomeMsg.myTargetID = TO_ALL;
		welcomeMsg.myMessage = myClients.back().myName + " has connected!";

		myMessageManager.CreateMessage<CNetMessageChatMessage>(welcomeMsg);
	}
}

void CServerMain::DisconnectClient(short aClientID)
{
	PRINT(myClients[aClientID - 1].myName + " disconnected");
	myClients[aClientID-1].myIsConnected = false;
}

//void CServerMain::RecieveChatMessage(SProtocol aProtocol)
//{
//	PRINT(myClients[aProtocol.mySenderID].myName + " says: " + aProtocol.myMessage.GetString());
//}

bool CServerMain::VerifyClient(const SClient & aClient)
{
	return true;
}

void CServerMain::UpdateTime()
{
	timePoint now = std::chrono::high_resolution_clock::now();

	std::chrono::duration<float> dtNow = now - myCurrentTimePoint;

	myCurrentTimeDelta = dtNow.count();

	myCurrentTimePoint = now;
}
