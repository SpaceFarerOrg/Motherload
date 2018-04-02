#include "stdafx.h"
#include "NetMessageWorldData.h"


CNetMessageWorldData::CNetMessageWorldData()
{
	myData.myID = EMessageType::WorldData;
}

std::array<unsigned short, MAX_ORE_COUNT> CNetMessageWorldData::GetOres()
{
	return std::move(myOres);
}

unsigned char CNetMessageWorldData::GetWorldHeight()
{
	return myWorldHeight;
}

unsigned char CNetMessageWorldData::GetWorldWidth()
{
	return myWorldWidth;
}

unsigned char CNetMessageWorldData::GetSkyCutOff()
{
	return mySkyCutOff;
}

void CNetMessageWorldData::DoSerialize()
{
	CNetMessage::DoSerialize();
	SERIALIZE(myBuffer, myOres);
	SERIALIZE(myBuffer, myWorldHeight);
	SERIALIZE(myBuffer, myWorldWidth);
	SERIALIZE(myBuffer, mySkyCutOff);
}

void CNetMessageWorldData::DoDeserialize()
{
	CNetMessage::DoDeserialize();
	DESERIALIZE(myBuffer, myOres);
	DESERIALIZE(myBuffer, myWorldHeight);
	DESERIALIZE(myBuffer, myWorldWidth);
	DESERIALIZE(myBuffer, mySkyCutOff);
}

void CNetMessageWorldData::Create(CNetMessageWorldData::SWorldData aData)
{
	CNetMessage::Create(aData);
	myData.myID = EMessageType::WorldData;
	myOres = aData.myOres;
	myWorldHeight = aData.myWorldHeight;
	myWorldWidth = aData.myWorldWidth;
	mySkyCutOff = aData.mySkyCutOff;
}
