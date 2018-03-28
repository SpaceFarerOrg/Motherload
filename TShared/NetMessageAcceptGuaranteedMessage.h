#pragma once
#include "NetMessage.h"

class CNetMessageAcceptGuaranteedMessage : public CNetMessage
{
	friend class CNetMessageManager;

public:

	struct SAcceptData : public CNetMessage::SNetMessageData
	{
	};

	CNetMessageAcceptGuaranteedMessage();
	~CNetMessageAcceptGuaranteedMessage();

private:

	void Create(SAcceptData aData);
};

