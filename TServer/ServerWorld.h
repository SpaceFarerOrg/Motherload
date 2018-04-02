#pragma once
#include <vector>
#include "CommonNetworkIncludes.h"

class CNetMessageManager;

class CServerWorld
{
public:

	CServerWorld();
	~CServerWorld();

	void SetMessageManager(CNetMessageManager& aManager);

	void Build(unsigned char aWidth, unsigned char aHeight);
	void SendWorldData(unsigned char aTargetID);

	bool RemoveBlock(unsigned short aBlockID);

private:
	unsigned char myWorldWidth;
	unsigned char myWorldHeight;
	unsigned char mySkyCutOff;
	std::array<unsigned short, MAX_ORE_COUNT>myOres;


	std::vector<STile> myWorldData;

	CNetMessageManager* myMessageManager;
};

