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

void CNetMessagePosition::DoSerialize()
{
	CNetMessage::DoSerialize();
	SERIALIZE(myBuffer, myX);
	SERIALIZE(myBuffer, myY);
}

void CNetMessagePosition::DoDeserialize()
{
	CNetMessage::DoDeserialize();
	DESERIALIZE(myBuffer, myX);
	DESERIALIZE(myBuffer, myY);
}

void CNetMessagePosition::Create(SPositionMessageData aData)
{
	CNetMessage::Create(myData);
	myData.myID = EMessageType::Position;
	myX = aData.myX;
	myY = aData.myY;
}
