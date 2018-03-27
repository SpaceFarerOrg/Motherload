#include "stdafx.h"
#include "CnetMessageRemoveObject.h"


CnetMessageRemoveObject::CnetMessageRemoveObject()
{
}


CnetMessageRemoveObject::~CnetMessageRemoveObject()
{
}

void CnetMessageRemoveObject::DoSerialize()
{
	CNetMessage::DoSerialize();
}

void CnetMessageRemoveObject::DoDeserialize()
{
	CNetMessage::DoDeserialize();
}

void CnetMessageRemoveObject::Create(SRemoveObjectData aData)
{
	CNetMessage::Create(aData);
	myData.myID = EMessageType::RemoveObject;
}
