#pragma once
#include "NetMessage.h"

class CnetMessageRemoveObject : public CNetMessage
{
	friend class CNetMessageManager;

public:

	struct SRemoveObjectData : CNetMessage::SNetMessageData
	{
	};

	CnetMessageRemoveObject();
	~CnetMessageRemoveObject();

private:
	void DoSerialize() override;
	void DoDeserialize() override;

	void Create(SRemoveObjectData aData);

};

