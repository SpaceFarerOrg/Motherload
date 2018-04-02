#pragma once
#include "NetMessage.h"
#include "CommonNetworkIncludes.h"

class CNetMessageWorldData : public CNetMessage
{
	friend class CNetMessageManager;

public:

	CNetMessageWorldData();

	struct SWorldData : public SNetMessageData
	{
		unsigned char myWorldWidth;
		unsigned char myWorldHeight;
		unsigned char mySkyCutOff;
		std::array<unsigned short, MAX_ORE_COUNT> myOres;
	};

	std::array<unsigned short, MAX_ORE_COUNT> GetOres();
	unsigned char GetWorldHeight();
	unsigned char GetWorldWidth();
	unsigned char GetSkyCutOff();

private:
	void DoSerialize() override;
	void DoDeserialize() override;

	void Create(SWorldData aData);

	unsigned char myWorldWidth;
	unsigned char myWorldHeight;
	unsigned char mySkyCutOff;
	std::array<unsigned short, MAX_ORE_COUNT> myOres;

};