#include "stdafx.h"
#include "NetMessagePosition.h"


CNetMessagePosition::CNetMessagePosition()
{
	myData.myID = EMessageType::Position;
}

void CNetMessagePosition::GetPosition(float & aOutX, float & aOutY)
{
	aOutX = myX;
	aOutY = myY;
}

unsigned short CNetMessagePosition::GetID()
{
	return myObjectID;
}

void CNetMessagePosition::DoSerialize()
{
	CNetMessage::DoSerialize();
	SERIALIZE(myBuffer, myX);
	SERIALIZE(myBuffer, myY);
	SERIALIZE(myBuffer, myObjectID);
}

void CNetMessagePosition::DoDeserialize()
{
	CNetMessage::DoDeserialize();
	DESERIALIZE(myBuffer, myX);
	DESERIALIZE(myBuffer, myY);
	DESERIALIZE(myBuffer, myObjectID);
}

void CNetMessagePosition::Create(SPositionMessageData aData)
{
	CNetMessage::Create(aData);
	myData.myID = EMessageType::Position;
	myX = aData.myX;
	myY = aData.myY;
	myObjectID = aData.myObjectID;
}
