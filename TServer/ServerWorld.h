#pragma once
#include <vector>
#include "CommonNetworkIncludes.h"

class CNetMessageManager;

struct SBlockPos
{
	float myX = 0.f;
	float myY = 0.f;
};

class CServerWorld
{
public:

	CServerWorld();
	~CServerWorld();

	void SetMessageManager(CNetMessageManager& aManager);

	void Build(unsigned char aWidth, unsigned char aHeight);
	void SendWorldData(unsigned char aTargetID);

	bool RemoveBlock(unsigned short aBlockID);

	unsigned short GetWidth();

	SBlockPos GetBlockPosFromID(unsigned short aBlockID);


	ETileType GetBlockTypeFromID(size_t aID) const;

private:
	unsigned char myWorldWidth;
	unsigned char myWorldHeight;
	unsigned char mySkyCutOff;
	std::array<unsigned short, MAX_ORE_COUNT>myOres;


	std::vector<STile> myWorldData;

	CNetMessageManager* myMessageManager;
};

