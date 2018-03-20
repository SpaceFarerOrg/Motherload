#include "stdafx.h"
#include "NetMessagePosition.h"


CNetMessagePosition::CNetMessagePosition()
{
	myData.myID = EMessageType::Position;
}

void CNetMessagePosition::GetPosition(float & aOutX, float & aOutY)
{
	aOutX = myData.myX;
	aOutY = myData.myY;
}

void CNetMessagePosition::DoSerialize()
{
	CNetMessage::DoSerialize();
	SERIALIZE(myBuffer, myData.myX);
	SERIALIZE(myBuffer, myData.myY);
}

void CNetMessagePosition::DoDeserialize()
{
	CNetMessage::DoDeserialize();
	DESERIALIZE(myBuffer, myData.myX);
	DESERIALIZE(myBuffer, myData.myY);
}

void CNetMessagePosition::Create(SPositionMessageData aData)
{
	CNetMessage::Create(myData);
	myData.myID = EMessageType::Position;
	myData.myX = aData.myX;
	myData.myY = aData.myY;
}
