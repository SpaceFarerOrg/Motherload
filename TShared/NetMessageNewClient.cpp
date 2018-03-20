#include "stdafx.h"
#include "NetMessageNewClient.h"

size_t CNetMessageNewClient::GetConnectedClient() const
{
	return myData.myConnectedClient;
}

void CNetMessageNewClient::DoSerialize()
{
	CNetMessage::DoSerialize();
	SERIALIZE(myBuffer, myData.myConnectedClient);
}

void CNetMessageNewClient::DoDeserialize()
{
	CNetMessage::DoDeserialize();
	DESERIALIZE(myBuffer, myData.myConnectedClient);
}

void CNetMessageNewClient::Create(SNetMessageNewClientData aData)
{
	CNetMessage::Create(aData);
	myData.myID = EMessageType::NewClient;
	myData.myConnectedClient = aData.myConnectedClient;
}
