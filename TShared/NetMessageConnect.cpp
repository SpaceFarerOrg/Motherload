#include "stdafx.h"
#include "NetMessageConnect.h"

CNetMessageConnect::CNetMessageConnect()
{
	myData.myID = EMessageType::Connect;
}

CNetMessageConnect::~CNetMessageConnect()
{
}

std::string CNetMessageConnect::GetClientName() const
{
	return std::move(myClientConnectName.GetString());
}

void CNetMessageConnect::DoSerialize()
{
	CNetMessage::DoSerialize();
	SERIALIZE(myBuffer, myClientConnectName);
}

void CNetMessageConnect::DoDeserialize()
{
	CNetMessage::DoDeserialize();
	DESERIALIZE(myBuffer, myClientConnectName);
}

void CNetMessageConnect::Create(SNetMessageConnectData aData)
{
	CNetMessage::Create(aData);
	myData.myID = EMessageType::Connect;
	myClientConnectName = aData.myClientConnectName;
}
