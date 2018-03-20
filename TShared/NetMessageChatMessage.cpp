#include "stdafx.h"
#include "NetMessageChatMessage.h"


CNetMessageChatMessage::CNetMessageChatMessage()
{
}


CNetMessageChatMessage::~CNetMessageChatMessage()
{
}

std::string CNetMessageChatMessage::GetMessage()
{
	return std::move(myMessage.GetString());
}

void CNetMessageChatMessage::DoSerialize()
{
	CNetMessage::DoSerialize();
	SERIALIZE(myBuffer, myMessage);
}

void CNetMessageChatMessage::DoDeserialize()
{
	CNetMessage::DoDeserialize();
	DESERIALIZE(myBuffer, myMessage);
}

void CNetMessageChatMessage::Create(SNetMessageChatMessageData aData)
{
	CNetMessage::Create(aData);
	myData.myID = EMessageType::Chat;
	myMessage = aData.myMessage;
}
