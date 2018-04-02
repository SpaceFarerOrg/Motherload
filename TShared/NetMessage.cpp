#include "stdafx.h"
#include "NetMessage.h"
#include <vector>
#include "CommonNetworkIncludes.h"

void CNetMessage::PackMessage()
{
	DoSerialize();

	int size = sizeof(char) * myBuffer.size();

	if (size > 512)
	{
		PRINT("Serialized stream too large!");
		myBuffer = std::move(std::vector<char>());
	}
}

void CNetMessage::UnpackMessage()
{
	DoDeserialize();
}

void CNetMessage::RecieveData(char * aData, short aSize)
{
	myBuffer.resize(aSize);
	memcpy(&myBuffer[0], aData, aSize);
}

void CNetMessage::DoSerialize()
{
	std::vector<char> stream;
	
	SERIALIZE(stream, myData);

	myBuffer = std::move(stream);
}

CNetMessage::SNetMessageData& CNetMessage::GetData()
{
	return myData;
}

void CNetMessage::DoDeserialize()
{
	DESERIALIZE(myBuffer, myData);
}

char* CNetMessage::GetBufferStart()
{
	return &(myBuffer[0]);
}

size_t CNetMessage::GetBufferSize()
{
	return sizeof(char) * myBuffer.size();
}

void CNetMessage::Create(SNetMessageData aData)
{
	myData = aData;
}
