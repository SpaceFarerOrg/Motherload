#include "stdafx.h"
#include "ClientMain.h"
#include <iostream>
#include "Utilities.h"
#include <fstream>
#include "Game.h"
#include <Windows.h>
#include <lmcons.h>
#include "NetMessages.h"

CClientMain::CClientMain()
{
}


CClientMain::~CClientMain()
{
}

bool CClientMain::StartClient()
{

	myIsConnectedToServer = false;

	PRINT("Starting up client...");

	int error;

	WSADATA data = {};

	error = WSAStartup(MAKEWORD(2, 2), &data);

	if (error != 0)
	{
		PRINT("WSAStartup failed with error code: " + std::to_string(error));
		return false;
	}

	WCHAR username[UNLEN+1];
	DWORD username_len = UNLEN + 1;
	GetUserName(username, &username_len);

	std::wstring wun = username;
	myUserName = std::string(wun.begin(), wun.end());

	PRINT("Client startup successful!");

	myLocalAddress.sin_family = AF_INET;
	myLocalAddress.sin_port = 0;
	myLocalAddress.sin_addr.s_addr = INADDR_ANY;

	mySocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	//Set as non blocking because fuckall
	unsigned long mode = 1;
	ioctlsocket(mySocket, FIONBIO, &mode);

	bind(mySocket, (sockaddr*)&myLocalAddress, sizeof(myLocalAddress));

	int bufferSize = SOCKET_BUFFER_SIZE;
	setsockopt(mySocket, SOL_SOCKET, SO_RCVBUF, (char*)&bufferSize, sizeof(bufferSize));
	setsockopt(mySocket, SOL_SOCKET, SO_SNDBUF, (char*)&bufferSize, sizeof(bufferSize));

	int length = sizeof(myLocalAddress);
	int name = getsockname(mySocket, (sockaddr*)&myLocalAddress, &length);


	std::string serverIP;
	std::ifstream f("networkSettings.txt");
	f >> serverIP;
	f.close();

	//serverIP = Utilities::GetInput("Type an IP to connect with: ", "127.0.0.1");

	inet_pton(AF_INET, serverIP.c_str(), &myServerAddress.sin_addr.s_addr);
	myServerAddress.sin_port = htons(54000);
	myServerAddress.sin_family = AF_INET;

	myMessageManager.Init(256, mySocket);

	TryToConnect(myUserName);

	myShouldRun.store(true);
	myInput = "";

	myGame->SetMessageManager(myMessageManager);

	return true;
}

bool CClientMain::RunClient()
{
	float dt = myClock.getElapsedTime().asSeconds();
	myClock.restart();
	time_t currentTime = time(nullptr);

	if (myIsConnectedToServer)
	{
		if (currentTime - myLatestRecievedMessageTime >= 2)
		{
			myGame->SetIsConnected(false);
			DisconnectFromServer();
			myIsConnectedToServer = false;
		}
	}

	char buff[MAX_BUFFER_SIZE];
	ZeroMemory(buff, MAX_BUFFER_SIZE);

	sockaddr_in from;

	while (recvfrom(mySocket, buff, MAX_BUFFER_SIZE, 0, nullptr, 0) != SOCKET_ERROR)
	{
		myLatestRecievedMessageTime = currentTime;

		size_t buffIndex = 0;
		EMessageType id = static_cast<EMessageType>(buff[buffIndex]);
		bool shouldSkip = false;

		CNetMessage base;
		base.RecieveData(buff, sizeof(CNetMessage::SNetMessageData));
		base.UnpackMessage();
		{
		if (base.GetData().myMessageID > 0 && id != EMessageType::AcceptGuaranteed)
			myMessageManager.AcceptGuaranteedMessage(0, myID, base.GetData().myMessageID);
			//if (myRecievedGuaranteedMessages.find(base.GetData().myMessageID) != myRecievedGuaranteedMessages.end())
			//{
			//	shouldSkip = true;
			//}
			myRecievedGuaranteedMessages.insert(base.GetData().myMessageID);
		}

		if (shouldSkip == false)
		{
			switch (id)
			{
			case EMessageType::Connect:
			{
				if (myIsConnectedToServer == false)
				{
					CNetMessageConnect rec;
					rec.RecieveData(buff, sizeof(SNetMessageConnectData));
					rec.UnpackMessage();

					PRINT("You are client no. " + std::to_string(rec.GetData().myTargetID) + " Server says " + rec.GetClientName());
					myIsConnectedToServer = true;
					myID = rec.GetData().myTargetID;
					myGame->SetIsConnected(true);
				}
			}
			break;
			case EMessageType::Disconnect:
			{
				CNetMessageConnect rec;
				rec.RecieveData(buff, sizeof(SNetMessageConnectData));
				rec.UnpackMessage();

				if (rec.GetData().mySenderID == 0)
				{
					PRINT("Server disconnected!");

					myIsConnectedToServer = false;
					myGame->SetIsConnected(false);
				}
				else
				{
					PRINT("A player disconnected! :(");
					myGame->RemovePlayer(rec.GetData().mySenderID);
				}
			}
			break;
			case EMessageType::Chat:
			{
				CNetMessageChatMessage rec;
				rec.RecieveData(buff, sizeof(SNetMessageChatMessageData));
				rec.UnpackMessage();

				PRINT(rec.GetMessage());
			}
			break;
			case EMessageType::Ping:
			{
				PRINT("Recieved ping");
			}
			break;
			case EMessageType::Position:
			{
				CNetMessagePosition rec;
				rec.RecieveData(buff, sizeof(CNetMessagePosition::SPositionMessageData));
				rec.UnpackMessage();

				sf::Vector2f pos;
				rec.GetPosition(pos.x, pos.y);

				//myGame->UpdateOtherPlayer(rec.GetData().mySenderID, pos);
				myGame->UpdateObject(rec.GetData().mySenderID, pos);
			}
			break;
			case EMessageType::NewClient:
			{
				CNetMessageNewClient rec;
				rec.RecieveData(buff, sizeof(CNetMessageNewClient::SNetMessageNewClientData));
				rec.UnpackMessage();

				myGame->AddPlayer(rec.GetConnectedClient());
			}
			break;
			case EMessageType::NewObject:
			{
				CNetMessageNewObject rec;
				rec.RecieveData(buff, sizeof(CNetMessageNewObject::SNewObjectData));
				rec.UnpackMessage();

				sf::Vector2f position;
				rec.GetPosition(position.x, position.y);

				myGame->AddObject(rec.GetData().mySenderID, position);
			}
			break;
			case EMessageType::RemoveObject:
			{
				CNetMessageRemoveObject rec;
				rec.RecieveData(buff, sizeof(CNetMessageRemoveObject::SRemoveObjectData));
				rec.UnpackMessage();

				myGame->RemoveObject(rec.GetData().mySenderID);
			}
			break;
			case EMessageType::AcceptGuaranteed:
			{
				unsigned int msgID = base.GetData().myMessageID;
				myMessageManager.VerifyGuaranteedMessage(msgID);
			}
			break;
			case EMessageType::WorldData:
			{
				CNetMessageWorldData rec;
				rec.RecieveData(buff, sizeof(CNetMessageWorldData::SWorldData));
				rec.UnpackMessage();

				myGame->LoadWorld(rec.GetWorldWidth(), rec.GetWorldHeight(), rec.GetSkyCutOff(), rec.GetOres());
			}
			break;
			case EMessageType::DestroyBlock:
			{
				CNetMessageDestroyBlock rec;
				rec.RecieveData(buff, sizeof(CNetMessageDestroyBlock::SDestroyBlockData));
				rec.UnpackMessage();

				myGame->DestroyBlock(rec.GetBlockID());
			}
			break;
			}
		}
	}

	if (myIsConnectedToServer)
	{
		UpdateChatMode();
		myPlayerUpdateTimer += dt;

		if (myPlayerUpdateTimer >= 1.f / 60.f)
		{
			myPlayerUpdateTimer = 0.f;
			//SendPlayerData();
		}
	}
	else
	{
		TryToConnect(myUserName);
	}


	std::this_thread::yield();

	myMessageManager.Flush({ myServerAddress });

	myMessageManager.Update(dt);
	myGame->SetKbPerSecond((float)myMessageManager.GetSentDataLastSecond() / 125.f);


	return myShouldRun;
}

void CClientMain::ShutDownClient()
{
	DisconnectFromServer();

	myShouldRun.store(false);

	closesocket(mySocket);
	WSACleanup();
}

void CClientMain::BindGame(CGame & aGameRef)
{
	myGame = &aGameRef;
}

void CClientMain::UpdateChatMode()
{
	if (myShouldRun.load() && myInput.length() > 0)
	{
		SNetMessageChatMessageData chatMsg;
		chatMsg.myMessage = myInput;
		chatMsg.myTargetID = TO_ALL;
		chatMsg.mySenderID = myID;

		myMessageManager.CreateMessage<CNetMessageChatMessage>(chatMsg);

		myInput.clear();
	}
}

void CClientMain::DisconnectFromServer()
{
	CNetMessage::SNetMessageData disconMes;
	disconMes.myTargetID = TO_ALL;
	disconMes.mySenderID = myID;

	myMessageManager.CreateMessage<CNetDisconnectMessage>(disconMes);

	myMessageManager.Flush({ myServerAddress });
}

void CClientMain::TryToConnect(const std::string& aUserName)
{
	SNetMessageConnectData msgData;
	msgData.myTargetID = 0;
	msgData.myClientConnectName = aUserName;

	myMessageManager.CreateMessage<CNetMessageConnect>(msgData);
}

void CClientMain::SendPlayerData()
{
	CNetMessagePosition::SPositionMessageData data;
	data.myX = myGame->GetPlayerPosition().x;
	data.myY = myGame->GetPlayerPosition().y;

	data.mySenderID = myID;
	data.myTargetID = TO_ALL;

	myMessageManager.CreateMessage<CNetMessagePosition>(data);
}

