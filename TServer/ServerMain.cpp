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
#include "NetMessageNewClient.h"
#include "NetMessageNewObject.h"

CServerMain::CServerMain()
{
	myCurrentTimeDelta = 0;
	myTimeSincePositionSend = 0;
	myCurrentTimePoint = std::chrono::high_resolution_clock::now();
	myTotalTime = 0.f;
	mySpawnTimer = 0.f;
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
	myLocalAddress.sin_port = htons(54000);
	myLocalAddress.sin_addr.s_addr = INADDR_ANY;

	mySocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	//Set as non blocking because fuckall
	unsigned long mode = 1;
	ioctlsocket(mySocket, FIONBIO, &mode);

	int bufferSize = SOCKET_BUFFER_SIZE;
	setsockopt(mySocket, SOL_SOCKET, SO_RCVBUF, (char*)&bufferSize, sizeof(bufferSize));
	setsockopt(mySocket, SOL_SOCKET, SO_SNDBUF, (char*)&bufferSize, sizeof(bufferSize));

	int fel = bind(mySocket, (sockaddr*)&myLocalAddress, sizeof(myLocalAddress));

	PRINT("Listening for new connections...");

	myMessageManager.Init(256, mySocket);

	myLatestPingTime = time(0);

	return true;
}

bool CServerMain::RunServer()
{
	UpdateTime();
	UpdateGameObjects();

	myTimeSincePositionSend += myCurrentTimeDelta;

	if (myTimeSincePositionSend >= POSITION_FREQ)
	{
		//for (SClient& c : myClients)
		//{
		//	if (c.myIsConnected)
		//	{
		//		CNetMessagePosition::SPositionMessageData data;
		//	
		//		data.myTargetID = TO_ALL - c.myID;
		//		data.mySenderID = c.myID;
		//		//Test comments
		//		data.myX = c.myX;
		//		data.myY = c.myY;
		//	
		//		myMessageManager.CreateMessage<CNetMessagePosition>(data);
		//	}
		//}

		for (auto& object : myGameObjects)
		{
			CNetMessagePosition::SPositionMessageData data;
			data.myTargetID = TO_ALL;
			data.mySenderID = object.first;
			data.myX = object.second.x;
			data.myY = object.second.y;
		
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

			DisconnectClient(rec.GetData().mySenderID);
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
			myClients[rec.GetData().mySenderID-1].myX = x;
			myClients[rec.GetData().mySenderID-1].myY = y;
		}
		break;
		}
	}

	time_t currentTime = time(nullptr);
	if (currentTime - myLatestPingTime >= 1)
	{
		myLatestPingTime = currentTime;
		myMessageManager.CreateMessage<CNetMessagePing>(CNetMessage::SNetMessageData());
		PRINT(std::to_string((float)myMessageManager.GetSentDataLastSecond() / 125) + "kb/s");
	}

	int errorCode = WSAGetLastError();

	myMessageManager.Flush(myAddressToClientLUT);
	myMessageManager.Update(myCurrentTimeDelta);

	return shouldContinueToRun;
}

void CServerMain::ShutDownServer()
{
	CNetMessage::SNetMessageData disconMes;

	disconMes.myTargetID = TO_ALL;

	myMessageManager.CreateMessage<CNetDisconnectMessage>(disconMes);
	myMessageManager.Flush(myAddressToClientLUT);

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

		for (size_t i = 0; i < myClients.size(); ++i)
		{
			if (i+1 == data.myTargetID || myClients[i].myIsConnected == false)
				continue;

			CNetMessageNewClient::SNetMessageNewClientData newClientData;
			newClientData.myConnectedClient = myClients[i].myID;
			newClientData.myTargetID = data.myTargetID;
			myMessageManager.CreateMessage<CNetMessageNewClient>(newClientData);
		}

		CNetMessageNewClient::SNetMessageNewClientData newClientData;
		newClientData.myConnectedClient = myClients.size();
		newClientData.myTargetID = TO_ALL - myClients.back().myID;
		myMessageManager.CreateMessage<CNetMessageNewClient>(newClientData);

	}
}

void CServerMain::DisconnectClient(short aClientID)
{
	PRINT(myClients[aClientID - 1].myName + " disconnected");
	myClients[aClientID-1].myIsConnected = false;

	CNetMessage::SNetMessageData msg;
	msg.mySenderID = myClients[aClientID - 1].myID;
	msg.myTargetID = TO_ALL - myClients[aClientID - 1].myID;

	myMessageManager.CreateMessage<CNetDisconnectMessage>(msg);
	
}

//void CServerMain::RecieveChatMessage(SProtocol aProtocol)
//{
//	PRINT(myClients[aProtocol.mySenderID].myName + " says: " + aProtocol.myMessage.GetString());
//}

bool CServerMain::VerifyClient(const SClient & aClient)
{
	return true;
}

void CServerMain::UpdateGameObjects()
{
	mySpawnTimer += myCurrentTimeDelta;
	if (mySpawnTimer > 1.f)
	{
		mySpawnTimer = 0.f;
		PRINT("Spawned an Object");
		myGameObjects.insert(std::pair<short, NetVector2>(myAvailableID, { 800.f, 400.f }));

		CNetMessageNewObject::SNewObjectData data;
		data.myTargetID = TO_ALL;
		data.mySenderID = myAvailableID;
		data.myX = 800.f;
		data.myY = 450.f;

		myMessageManager.CreateMessage<CNetMessageNewObject>(data);

		myAvailableID++;
	}

	for (auto& object : myGameObjects)
	{
		//object.second.x += myCurrentTimeDelta;
		float direction = (float)(rand() % 360) / 360.f * 6.28f;
		object.second.y += myCurrentTimeDelta * 600.f * -cosf(myTotalTime * 3.14f);
		object.second.x += myCurrentTimeDelta * 300 * cosf(myTotalTime);
	}

	
}

void CServerMain::UpdateTime()
{
	timePoint now = std::chrono::high_resolution_clock::now();

	std::chrono::duration<float> dtNow = now - myCurrentTimePoint;

	myCurrentTimeDelta = dtNow.count();

	myCurrentTimePoint = now;
	myTotalTime += myCurrentTimeDelta;
}
