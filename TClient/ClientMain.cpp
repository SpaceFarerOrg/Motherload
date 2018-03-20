#include "stdafx.h"
#include "ClientMain.h"
#include <iostream>
#include "Utilities.h"
#include "NetMessageConnect.h"
#include "NetDisconnectMessage.h"
#include "NetMessageChatMessage.h"
#include <fstream>
#include "Game.h"
#include <Windows.h>
#include <lmcons.h>

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
	std::string userName(wun.begin(), wun.end());

	PRINT("Client startup successful!");

	myLocalAddress.sin_family = AF_INET;
	myLocalAddress.sin_port = 0;
	myLocalAddress.sin_addr.s_addr = INADDR_ANY;

	mySocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	//Set as non blocking because fuckall
	unsigned long mode = 1;
	ioctlsocket(mySocket, FIONBIO, &mode);

	bind(mySocket, (sockaddr*)&myLocalAddress, sizeof(myLocalAddress));


	int length = sizeof(myLocalAddress);
	int name = getsockname(mySocket, (sockaddr*)&myLocalAddress, &length);


	std::string serverIP;
	std::ifstream f("networkSettings.txt");
	f >> serverIP;
	f.close();

	//serverIP = Utilities::GetInput("Type an IP to connect with: ", "127.0.0.1");

	inet_pton(AF_INET, serverIP.c_str(), &myServerAddress.sin_addr.s_addr);
	myServerAddress.sin_port = htons(53000);
	myServerAddress.sin_family = AF_INET;

	myMessageManager.Init(256, mySocket);

	SNetMessageConnectData msgData;
	msgData.myTargetID = TO_ALL;
	msgData.myClientConnectName = userName;

	myMessageManager.CreateMessage<CNetMessageConnect>(msgData);

	myShouldRun.store(true);
	myInput = "";

	return true;
}

bool CClientMain::RunClient()
{
	time_t currentTime = time(nullptr);

	if (myIsConnectedToServer)
	{
		if (currentTime - myLatestRecievedMessageTime >= 5)
		{
			myGame->SetIsConnected(false);
			DisconnectFromServer();
		}
	}

	char buff[MAX_BUFFER_SIZE];
	ZeroMemory(buff, MAX_BUFFER_SIZE);

	sockaddr_in from;

	if (recvfrom(mySocket, buff, MAX_BUFFER_SIZE, 0, nullptr, 0) != SOCKET_ERROR)
	{
		myLatestRecievedMessageTime = currentTime;

		size_t buffIndex = 0;
		EMessageType id = static_cast<EMessageType>(buff[buffIndex]);

		switch (id)
		{
		case EMessageType::Connect:
		{
			CNetMessageConnect rec;
			rec.RecieveData(buff, sizeof(SNetMessageConnectData));
			rec.UnpackMessage();

			PRINT("You are client no. " + std::to_string(rec.GetData().myTargetID) + " Server says " + rec.GetClientName());
			myIsConnectedToServer = true;
			myID = rec.GetData().myTargetID;
			myGame->SetIsConnected(true);
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
		}
	}

	if (myIsConnectedToServer)
	{
		UpdateChatMode();
	}

	std::this_thread::yield();

	myMessageManager.Flush({ myServerAddress });

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

