#pragma warning(disable:4996)
#define _SCL_SECURE_NO_WARNINGS
#include "stdafx.h"
#include "ServerMain.h"
#include <iostream>
#include <string>
#include "Utilities.h"
#include "NetMessages.h"

CServerMain::CServerMain()
{
	myCurrentTimeDelta = 0;
	myTimeSincePositionSend = 0;
	myCurrentTimePoint = std::chrono::high_resolution_clock::now();
	myTotalTime = 0.f;
	mySpawnTimer = 0.f;

	srand(static_cast<unsigned int>(time(0)));
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

	myAvailableID = 1;

	myWorld.SetMessageManager(myMessageManager);
	myWorld.Build(25, 60);

	return true;
}

bool CServerMain::RunServer()
{
	UpdateTime();
	UpdateGameObjects();

	myTimeSincePositionSend += myCurrentTimeDelta;

	if (myTimeSincePositionSend >= POSITION_FREQ)
	{
		for (SClient& c : myClients)
		{
			if (c.myIsConnected)
			{
				CNetMessagePosition::SPositionMessageData data;

				data.myTargetID = TO_ALL - c.myID;
				data.mySenderID = c.myID;
				//Test comments
				data.myX = c.myX;
				data.myY = c.myY;

				myMessageManager.CreateMessage<CNetMessagePosition>(data);
			}
		}

		//for (auto& object : myGameObjects)
		//{
		//	CNetMessagePosition::SPositionMessageData data;
		//	data.myTargetID = TO_ALL;
		//	data.mySenderID = object.first;
		//	data.myX = object.second.GetPosition().x;
		//	data.myY = object.second.GetPosition().y;
		//	data.myObjectID = object.first;
		//
		//	myMessageManager.CreateMessage<CNetMessagePosition>(data);
		//}

		myTimeSincePositionSend -= POSITION_FREQ;
	}

	bool shouldContinueToRun = true;

	sockaddr_in from;
	int fromlen = sizeof(from);

	char buff[MAX_BUFFER_SIZE];
	ZeroMemory(buff, MAX_BUFFER_SIZE);
	Sleep(1);

	unsigned short recievedThisFrame = 0;

	while (recievedThisFrame < MAX_RECIEVED_PER_FRAME && recvfrom(mySocket, buff, MAX_BUFFER_SIZE, 0, (sockaddr*)&from, &fromlen) != SOCKET_ERROR)
	{
		recievedThisFrame++;

		size_t buffIndex = 0;
		EMessageType id = static_cast<EMessageType>(buff[buffIndex]);
		bool shouldSkip = false;

		CNetMessage base;
		base.RecieveData(buff, sizeof(CNetMessage::SNetMessageData));
		base.UnpackMessage();
		if (base.GetData().myMessageID > 0 && id != EMessageType::AcceptGuaranteed)
		{
			myMessageManager.AcceptGuaranteedMessage(base.GetData().mySenderID, 0, base.GetData().myMessageID);
			//if (myRecievedGuaranteedMessages.find(base.GetData().myMessageID) != myRecievedGuaranteedMessages.end())
			//{
			//	shouldSkip = true;
			//}
			myRecievedGuaranteedMessages.insert(base.GetData().myMessageID);
		}

		if (base.GetData().mySenderID != 0 && shouldSkip == false)
		{
			shouldSkip = true;
			for (int i = 0; i < myClients.size(); ++i)
			{
				if (myClients[i].myID == base.GetData().mySenderID)
				{
					shouldSkip = false;
				}
			}
		}

		if (shouldSkip == false)
		{
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
				PRINT(myClients[rec.GetData().mySenderID - 1].myName + ": " + rec.GetMessage());

				SNetMessageChatMessageData chatMsgData;
				chatMsgData.myMessage = myClients[rec.GetData().mySenderID - 1].myName + ": " + rec.GetMessage();
				chatMsgData.myTargetID = TO_ALL - rec.GetData().mySenderID;

				myMessageManager.CreateMessage<CNetMessageChatMessage>(chatMsgData);
			}
			break;
			case EMessageType::Position:
			{
				CNetMessagePosition rec;
				rec.RecieveData(buff, sizeof(CNetMessagePosition::SPositionMessageData));
				rec.UnpackMessage();

				float x, y;
				rec.GetPosition(x, y);

				//Update the position of the client in world!
				myClients[rec.GetData().mySenderID - 1].myX = x;
				myClients[rec.GetData().mySenderID - 1].myY = y;
			}
			break;
			case EMessageType::AcceptGuaranteed:
			{
				unsigned int msgID = base.GetData().myMessageID;
				myMessageManager.VerifyGuaranteedMessage(msgID);
			}
			break;
			case EMessageType::DestroyBlock:
			{
				CNetMessageDestroyBlock rec;
				rec.RecieveData(buff, sizeof(CNetMessageDestroyBlock::SDestroyBlockData));
				rec.UnpackMessage();

				unsigned short diggableIndices[4];
				unsigned short playerIndex = (unsigned short)(myClients[rec.GetData().mySenderID-1].myX + 32) / 64 + myWorld.GetWidth() * (unsigned short)((myClients[rec.GetData().mySenderID-1].myY + 32) / 64);
				diggableIndices[0] = playerIndex - 1;
				diggableIndices[1] = playerIndex + 1;
				diggableIndices[2] = playerIndex + myWorld.GetWidth();
				diggableIndices[3] = playerIndex - myWorld.GetWidth();
				
				bool isDiggable = false;

				for (unsigned short i : diggableIndices)
				{
					if (i == rec.GetBlockID())
					{
						isDiggable = true;
						break;
					}
				}


				if (isDiggable)
				{

					if (myWorld.RemoveBlock(rec.GetBlockID()))
					{
						CNetMessageDestroyBlock::SDestroyBlockData destroyData;
						destroyData.myBlockID = rec.GetBlockID();
						destroyData.mySenderID = 0;

						myClients[rec.GetData().mySenderID - 1].myFuel -= 0.03f;

						if (myWorld.GetBlockTypeFromID(rec.GetBlockID()) == ETileType::Ore)
						{
							CNetMessageSimpleType::SSimpleTypeData sData;
							sData.myID = EMessageType::GiveOre;
							sData.myTargetID = rec.GetData().mySenderID;
							myMessageManager.CreateGuaranteedMessage<CNetMessageSimpleType>(sData);
						}

						CNetMessageFuel::SFuelMessageData fuelData;
						fuelData.myTargetID = rec.GetData().mySenderID;
						fuelData.myFuelAmount = myClients[rec.GetData().mySenderID - 1].myFuel;
						myMessageManager.CreateGuaranteedMessage<CNetMessageFuel>(fuelData);

						for (SClient& client : myClients)
						{
							destroyData.myTargetID = client.myID;
							myMessageManager.CreateGuaranteedMessage<CNetMessageDestroyBlock>(destroyData);
						}
					}
				}
			}
			break;
			case EMessageType::Fuel:
			{
				CNetMessageFuel rec;
				rec.RecieveData(buff, sizeof(CNetMessageFuel::SFuelMessageData));
				rec.UnpackMessage();

				SClient& client = myClients[rec.GetData().mySenderID - 1];

				client.myFuel += client.myFuel < 1.f ? rec.GetFuelAmount() : 0.f;
			}
			break;
			}
		}
	}

	time_t currentTime = time(nullptr);
	if (currentTime - myLatestPingTime >= 1)
	{
		myLatestPingTime = currentTime;
		myMessageManager.CreateMessage<CNetMessagePing>(CNetMessage::SNetMessageData());


		//Send fuel amount
		for (SClient& client : myClients)
		{
			client.myFuel = client.myFuel < 0.f ? 0.f : client.myFuel;

			CNetMessageFuel::SFuelMessageData fuelData;
			fuelData.myTargetID = client.myID;
			fuelData.myFuelAmount = client.myFuel;

			myMessageManager.CreateMessage<CNetMessageFuel>(fuelData);
		}

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


	if (VerifyClient(newClient))
	{
		PRINT("Client " + newClient.myName + " connected");
		myClients.push_back(newClient);
		myClients.back().myID = myClients.size();
		myAddressToClientLUT.push_back(aReturnAddress);

		SNetMessageConnectData data;
		data.myTargetID = myClients.size();
		data.myClientConnectName = "OK";
		data.mySenderID = 0;
		myMessageManager.CreateGuaranteedMessage<CNetMessageConnect>(data);

		myWorld.SendWorldData(data.myTargetID);

		for (size_t i = 0; i < myClients.size(); ++i)
		{
			if (i + 1 == data.myTargetID || myClients[i].myIsConnected == false)
				continue;

			CNetMessageNewClient::SNetMessageNewClientData newClientData;
			newClientData.myConnectedClient = myClients[i].myID;
			newClientData.myTargetID = data.myTargetID;
			myMessageManager.CreateGuaranteedMessage<CNetMessageNewClient>(newClientData);
		}

		for (size_t i = 0; i < myClients.size() - 1; ++i)
		{
			CNetMessageNewClient::SNetMessageNewClientData toExistingClient;
			toExistingClient.myConnectedClient = myClients.size();
			toExistingClient.myTargetID = 1;
			toExistingClient.mySenderID = 0;
			myMessageManager.CreateGuaranteedMessage<CNetMessageNewClient>(toExistingClient);
		}

		//for (auto& object : myGameObjects)
		//{
		//	CNetMessageNewObject::SNewObjectData data;
		//	data.myTargetID = myClients.size();
		//	data.myX = object.second.GetPosition().x;
		//	data.myY = object.second.GetPosition().y;
		//	data.myObjectID = object.first;
		//
		//	myMessageManager.CreateGuaranteedMessage<CNetMessageNewObject>(data);
		//}

	}
}

void CServerMain::DisconnectClient(short aClientID)
{
	if (aClientID != 0)
	{
		PRINT(myClients[aClientID - 1].myName + " disconnected");
		myClients[aClientID - 1].myIsConnected = false;

		CNetMessage::SNetMessageData msg;
		msg.mySenderID = myClients[aClientID - 1].myID;
		msg.myTargetID = TO_ALL - myClients[aClientID - 1].myID;

		myMessageManager.CreateMessage<CNetDisconnectMessage>(msg);
	}
}

//void CServerMain::RecieveChatMessage(SProtocol aProtocol)
//{
//	PRINT(myClients[aProtocol.mySenderID].myName + " says: " + aProtocol.myMessage.GetString());
//}

bool CServerMain::VerifyClient(const SClient & aClient)
{
	for (SClient& client : myClients)
	{
		if (client.myName == aClient.myName)
		{
			PRINT("Client " + client.myName + " reconnected!");
			client.myIsConnected = true;

			SNetMessageConnectData data;
			data.myTargetID = client.myID;
			data.myClientConnectName = "OK";
			data.mySenderID = 0;
			myMessageManager.CreateMessage<CNetMessageConnect>(data);

			return false;
		}
	}

	return true;
}

void CServerMain::UpdateGameObjects()
{
	mySpawnTimer += myCurrentTimeDelta;
	if (mySpawnTimer > 1.f)
	{
		mySpawnTimer = 0.f;
		PRINT("Spawned an Object");
		CServerGameObject newObject;
		newObject.Init({ 800.f, 400.f });
		myGameObjects.insert(std::pair<short, CServerGameObject>(myAvailableID, newObject));

		for (SClient& client : myClients)
		{
			CNetMessageNewObject::SNewObjectData data;
			data.myTargetID = client.myID;
			data.myObjectID = myAvailableID;
			data.myX = 800.f;
			data.myY = 450.f;
			myMessageManager.CreateGuaranteedMessage<CNetMessageNewObject>(data);
		}

		myAvailableID++;
	}

	for (auto& object : myGameObjects)
	{
		object.second.Update(myCurrentTimeDelta);

		for (auto& other : myGameObjects)
		{
			if (object.first == other.first)
				continue;

			float distanceMax = 60;
			NetVector2 between = object.second.GetPosition();
			between.x -= other.second.GetPosition().x;
			between.y -= other.second.GetPosition().y;
			float length = sqrt(between.x * between.x + between.y * between.y);

			if (length < distanceMax)
			{
				for (SClient& client : myClients)
				{
					CNetMessageRemoveObject::SRemoveObjectData removeData;
					removeData.myTargetID = client.myID;
					removeData.myObjectID = object.first;
					myMessageManager.CreateGuaranteedMessage<CNetMessageRemoveObject>(removeData);
					removeData.myObjectID = other.first;
					myMessageManager.CreateGuaranteedMessage<CNetMessageRemoveObject>(removeData);

					myIDsToRemove.insert(other.first);
					myIDsToRemove.insert(object.first);
				}
			}
		}
	}

	for (const short& id : myIDsToRemove)
	{
		myGameObjects.erase(id);
	}
	myIDsToRemove.clear();
}

void CServerMain::UpdateTime()
{
	timePoint now = std::chrono::high_resolution_clock::now();

	std::chrono::duration<float> dtNow = now - myCurrentTimePoint;

	myCurrentTimeDelta = dtNow.count();

	myCurrentTimePoint = now;
	myTotalTime += myCurrentTimeDelta;
}
