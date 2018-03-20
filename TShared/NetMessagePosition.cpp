#include "stdafx.h"
#include "NetMessagePosition.h"


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
	myData = aData;
}
