#pragma once

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#define PRINT(a) std::cout << (a) << std::endl;

#define TO_ALL 0

#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include <atomic>

#define MAX_BUFFER_SIZE 512

#pragma comment(lib, "Ws2_32.lib")

enum class EMessageType : short
{
	Invalid,
	Chat,
	Connect,
	Disconnect,
	Ping,
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

struct SClient
{
	std::string myName;
	size_t myID;
	bool myIsConnected = true;
};