#include "stdafx.h"
#include "NetMessageNewClient.h"

size_t CNetMessageNewClient::GetConnectedClient() const
{
	return myData.aConnectedClient;
}

void CNetMessageNewClient::DoSerialize()
{
	CNetMessage::DoSerialize();
	SERIALIZE(myBuffer, myData.aConnectedClient);
}

void CNetMessageNewClient::DoDeserialize()
{
}

void CNetMessageNewClient::Create(SNetMessageNewClientData aData)
{
	CNetMessage::Create(aData);
	myData.myID = EMessageType::NewClient;
}
