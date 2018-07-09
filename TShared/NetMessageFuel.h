#pragma once
#include "NetMessage.h"

class CNetMessageFuel : public CNetMessage
{
	friend class CNetMessageManager;
public:
	CNetMessageFuel();

	struct SFuelMessageData : public SNetMessageData
	{
		float myFuelAmount;
		short myRecieverID;
	};

	short GetReciever() const;
	float GetFuelAmount() const;
private:
	void DoSerialize() override;
	void DoDeserialize() override;

	void Create(SFuelMessageData aData);

	float myFuelAmount;
	short myRecieverID;
};