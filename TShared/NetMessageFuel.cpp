#include "stdafx.h"
#include "NetMessageFuel.h"

CNetMessageFuel::CNetMessageFuel()
{
	myData.myID = EMessageType::Fuel;
}

float CNetMessageFuel::GetFuelAmount() const
{
	return myFuelAmount;
}

void CNetMessageFuel::DoSerialize()
{
	CNetMessage::DoSerialize();
	SERIALIZE(myBuffer, myFuelAmount);
}

void CNetMessageFuel::DoDeserialize()
{
	CNetMessage::DoDeserialize();
	DESERIALIZE(myBuffer, myFuelAmount);
}

void CNetMessageFuel::Create(SFuelMessageData aData)
{
	CNetMessage::Create(aData);
	myData.myID = EMessageType::Fuel;
	myFuelAmount = aData.myFuelAmount;
}
