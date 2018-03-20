#include "stdafx.h"
#include "NetMessageNewClient.h"

CNetMessageNewClient::CNetMessageNewClient()
{
	myData.myID = EMessageType::NewClient;
}

size_t CNetMessageNewClient::GetConnectedClient() const
{
	return myConnectedClient;
}

void CNetMessageNewClient::DoSerialize()
{
	CNetMessage::DoSerialize();
	SERIALIZE(myBuffer, myConnectedClient);
}

void CNetMessageNewClient::DoDeserialize()
{
	CNetMessage::DoDeserialize();
	DESERIALIZE(myBuffer, myConnectedClient);
}

void CNetMessageNewClient::Create(SNetMessageNewClientData aData)
{
	CNetMessage::Create(aData);
	myData.myID = EMessageType::NewClient;
	myConnectedClient = aData.myConnectedClient;
}
