#include "stdafx.h"
#include "NetMessageRemoveObject.h"


CNetMessageRemoveObject::CNetMessageRemoveObject()
{
}


CNetMessageRemoveObject::~CNetMessageRemoveObject()
{
}

unsigned short CNetMessageRemoveObject::GetID()
{
	return myObjectID;
}

void CNetMessageRemoveObject::DoSerialize()
{
	CNetMessage::DoSerialize();
	SERIALIZE(myBuffer, myObjectID);
}

void CNetMessageRemoveObject::DoDeserialize()
{
	CNetMessage::DoDeserialize();
	DESERIALIZE(myBuffer, myObjectID);
}

void CNetMessageRemoveObject::Create(SRemoveObjectData aData)
{
	CNetMessage::Create(aData);
	myData.myID = EMessageType::RemoveObject;
	myObjectID = aData.myObjectID;
}
