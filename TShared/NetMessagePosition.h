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
		unsigned short myObjectID;
	};


	void GetPosition(float& aOutX, float& aOutY);
	unsigned short GetID();

private:
	void DoSerialize() override;
	void DoDeserialize() override;

	void Create(SPositionMessageData aData);

	float myX;
	float myY;
	unsigned short myObjectID;

};