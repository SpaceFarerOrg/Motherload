#include "stdafx.h"
#include "NetMessageSimpleType.h"

int CNetMessageSimpleType::GetInt() const
{
	return myInt;
}

float CNetMessageSimpleType::GetFloat() const
{
	return myFloat;
}

bool CNetMessageSimpleType::GetBool() const
{
	return myBool;
}

void CNetMessageSimpleType::DoSerialize()
{
	CNetMessage::DoSerialize();
	SERIALIZE(myBuffer, myInt);
	SERIALIZE(myBuffer, myBool);
	SERIALIZE(myBuffer, myFloat);
}

void CNetMessageSimpleType::DoDeserialize()
{
	CNetMessage::DoDeserialize();
	DESERIALIZE(myBuffer, myInt);
	DESERIALIZE(myBuffer, myBool);
	DESERIALIZE(myBuffer, myFloat);
}

void CNetMessageSimpleType::Create(SSimpleTypeData aData)
{
	CNetMessage::Create(aData);
	myData.myID = aData.myID;
}
