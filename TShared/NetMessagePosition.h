#pragma once
#include "NetMessage.h"

class CNetMessagePosition : public CNetMessage
{
	friend class CNetMessageManager;

public:

	CNetMessagePosition();

	struct SPositionMessageData : public SNetMessageData
	{ 
		float myX;
		float myY;
	};


	void GetPosition(float& aOutX, float& aOutY);

private:
	void DoSerialize() override;
	void DoDeserialize() override;

	void Create(SPositionMessageData aData);

	SPositionMessageData myData;

};