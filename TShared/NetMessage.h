#pragma once
#define _SCL_SECURE_NO_WARNINGS
#include "SerializeHelper.h"
#include <vector>
#include "CommonNetworkIncludes.h"
#define SERIALIZE(aStream, aType) serialize(aType, aStream)
#define DESERIALIZE(aStream, aType) aType = deserialize<decltype(aType)>(aStream)

class CNetMessage
{
	friend class CNetMessageManager;

public:
	struct SNetMessageData
	{
		EMessageType myID;
		unsigned int myTimeStamp;
		unsigned short mySenderID;
		short myTargetID;

		unsigned int myMessageID = 0;
	};

	void PackMessage();
	void UnpackMessage();

	void RecieveData(char* aData, short aSize);

	SNetMessageData& GetData();

protected:
	virtual void DoSerialize();
	virtual void DoDeserialize();

	char* GetBufferStart();
	size_t GetBufferSize();

	void Create(SNetMessageData aData);

	SNetMessageData myData;

	std::vector<char> myBuffer;
};
