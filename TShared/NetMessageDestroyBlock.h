#pragma once
#include "NetMessage.h"

class CNetMessageDestroyBlock : public CNetMessage
{
	friend class CNetMessageManager;

public:

	struct SDestroyBlockData : public SNetMessageData
	{
		unsigned short myBlockID;
	};

	CNetMessageDestroyBlock();
	~CNetMessageDestroyBlock();

	unsigned short GetBlockID();


private:
	void DoSerialize() override;
	void DoDeserialize() override;

	void Create(SDestroyBlockData aData);

	unsigned short myBlockID;
};

