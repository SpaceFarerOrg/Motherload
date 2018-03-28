#pragma once
#include "NetMessage.h"

class CNetMessageRemoveObject : public CNetMessage
{
	friend class CNetMessageManager;

public:

	struct SRemoveObjectData : CNetMessage::SNetMessageData
	{
	};

	CNetMessageRemoveObject();
	~CNetMessageRemoveObject();

private:
	void DoSerialize() override;
	void DoDeserialize() override;

	void Create(SRemoveObjectData aData);

};

