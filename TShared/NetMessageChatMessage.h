#pragma once
#include "CommonNetworkIncludes.h"
#include "NetMessage.h"

struct SNetMessageChatMessageData : public CNetMessage::SNetMessageData
{
	SMessage myMessage;
};

class CNetMessageChatMessage : public CNetMessage
{

	friend class CNetMessageManager;

public:
	CNetMessageChatMessage();
	~CNetMessageChatMessage();

	std::string GetMessage();

private:
	void DoSerialize() override;
	void DoDeserialize() override;

	void Create(SNetMessageChatMessageData aData);
	SMessage myMessage;
};

