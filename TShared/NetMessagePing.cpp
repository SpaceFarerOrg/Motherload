#include "stdafx.h"
#include "NetMessagePing.h"


CNetMessagePing::CNetMessagePing()
{
}


CNetMessagePing::~CNetMessagePing()
{
}

void CNetMessagePing::DoSerialize()
{
	CNetMessage::DoSerialize();
}

void CNetMessagePing::DoDeserialize()
{
	CNetMessage::DoDeserialize();
}

void CNetMessagePing::Create(CNetMessage::SNetMessageData aData)
{
	CNetMessage::Create(aData);
	myData.myID = EMessageType::Ping;
}
