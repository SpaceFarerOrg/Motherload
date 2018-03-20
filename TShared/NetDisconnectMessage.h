#pragma once
#include "NetMessage.h"

class CNetDisconnectMessage : public CNetMessage
{
	friend class CNetMessageManager;
public:
private:
	void Create(SNetMessageData aData);
};
