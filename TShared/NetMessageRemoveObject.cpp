#include "stdafx.h"
#include "NetMessageRemoveObject.h"


CNetMessageRemoveObject::CNetMessageRemoveObject()
{
}


CNetMessageRemoveObject::~CNetMessageRemoveObject()
{
}

void CNetMessageRemoveObject::DoSerialize()
{
	CNetMessage::DoSerialize();
}

void CNetMessageRemoveObject::DoDeserialize()
{
	CNetMessage::DoDeserialize();
}

void CNetMessageRemoveObject::Create(SRemoveObjectData aData)
{
	CNetMessage::Create(aData);
	myData.myID = EMessageType::RemoveObject;
}
