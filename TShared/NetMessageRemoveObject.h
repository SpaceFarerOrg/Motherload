#pragma once
#include "NetMessage.h"

class CNetMessageRemoveObject : public CNetMessage
{
	friend class CNetMessageManager;

public:

	struct SRemoveObjectData : CNetMessage::SNetMessageData
	{
		unsigned short myObjectID;
	};

	CNetMessageRemoveObject();
	~CNetMessageRemoveObject();
	unsigned short GetID();

private:
	void DoSerialize() override;
	void DoDeserialize() override;

	void Create(SRemoveObjectData aData);

	unsigned short myObjectID;
};

