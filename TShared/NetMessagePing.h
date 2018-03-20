#pragma once
#include "NetMessage.h"

class CNetMessagePing : public CNetMessage
{
	friend class CNetMessageManager;

public:
	CNetMessagePing();
	~CNetMessagePing();

private:
	void DoSerialize() override;
	void DoDeserialize() override;

	void Create(CNetMessage::SNetMessageData aData);
};

