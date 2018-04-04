#pragma once

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#define PRINT(a) std::cout << (a) << std::endl;

#define TO_ALL 0

#define max(a,b)            (((a) > (b)) ? (a) : (b))
#define min(a,b)            (((a) < (b)) ? (a) : (b))

#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include <atomic>
#include <algorithm>
#include <array>

#define MAX_BUFFER_SIZE 512
#define SOCKET_BUFFER_SIZE 65000
#define MAX_RECIEVED_PER_FRAME 128

#pragma comment(lib, "Ws2_32.lib")

enum class EMessageType : short
{
	Invalid,
	Chat,
	Connect,
	Disconnect,
	Ping,
	Position,
	NewClient,
	NewObject,
	RemoveObject,
	AcceptGuaranteed,
	WorldData,
	DestroyBlock,
	Fuel,
	Size
};

struct SMessage
{
	char myData[128];

	std::string GetString() const
	{
		std::string string(myData);
		return std::move(string);
	}

	void operator=(const std::string& aString)
	{
		int integer = min(128, aString.length());

		for (int i = 0; i < 128; ++i)
		{
			myData[i] = aString[i];

			if (myData[i] == '\0')
			{
				return;
			}
		}
	}
};

struct NetVector2
{
	float x;
	float y;
};

struct SClient
{
	SClient()
	{
		myName = "";
		myID = 666;
		myX = 0.f;
		myY = 0.f;

		myFuel = 1.f;

		myIsConnected = true;
	}

	std::string myName;
	size_t myID;

	float myX;
	float myY;

	float myFuel;

	bool myIsConnected = true;
};

#define MAX_ORE_COUNT 128

enum class ETileType
{
	Sky,
	Ground,
	Ore,
	Rock,
	Count
};

struct STile
{
	ETileType myType;
	bool myIsDestroyed;
};