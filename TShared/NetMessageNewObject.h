#pragma once
#include "NetMessage.h"

class CNetMessageNewObject : public CNetMessage
{
	friend class CNetMessageManager;

public:

	struct SNewObjectData : CNetMessage::SNetMessageData
	{
		float myX;
		float myY;
	};

	CNetMessageNewObject();
	~CNetMessageNewObject();

	void GetPosition(float& aOutX, float& aOutY);

private:
	void DoSerialize() override;
	void DoDeserialize() override;

	void Create(SNewObjectData aData);

	float myX;
	float myY;
};

