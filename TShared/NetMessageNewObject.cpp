#include "stdafx.h"
#include "NetMessageNewObject.h"


CNetMessageNewObject::CNetMessageNewObject()
{
}


CNetMessageNewObject::~CNetMessageNewObject()
{
}

void CNetMessageNewObject::GetPosition(float & aOutX, float & aOutY)
{
	aOutX = myX;
	aOutY = myY;
}

unsigned short CNetMessageNewObject::GetID()
{
	return myObjectID;
}

void CNetMessageNewObject::DoSerialize()
{
	CNetMessage::DoSerialize();
	SERIALIZE(myBuffer, myX);
	SERIALIZE(myBuffer, myY);
	SERIALIZE(myBuffer, myObjectID);
}

void CNetMessageNewObject::DoDeserialize()
{
	CNetMessage::DoDeserialize();
	DESERIALIZE(myBuffer, myX);
	DESERIALIZE(myBuffer, myY);
	DESERIALIZE(myBuffer, myObjectID);
}

void CNetMessageNewObject::Create(CNetMessageNewObject::SNewObjectData aData)
{
	CNetMessage::Create(aData);
	myData.myID = EMessageType::NewObject;
	myX = aData.myX;
	myY = aData.myY;
	myObjectID = aData.myObjectID;
}
