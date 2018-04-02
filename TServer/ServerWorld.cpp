#include "stdafx.h"
#include "ServerWorld.h"
#include "NetMessageManager.h"
#include "NetMessageWorldData.h"
#include "NetMessageDestroyBlock.h"

CServerWorld::CServerWorld()
{
}


CServerWorld::~CServerWorld()
{
}

void CServerWorld::SetMessageManager(CNetMessageManager & aManager)
{
	myMessageManager = &aManager;
}

void CServerWorld::Build(unsigned char aWidth, unsigned char aHeight)
{
	myWorldData.clear();

	STile defaultTile;
	defaultTile.myIsDestroyed = false;
	defaultTile.myType = ETileType::Ground;

	myWorldWidth = aWidth;
	myWorldHeight = aHeight;

	mySkyCutOff = aHeight / 3;
	int chanceOfOre = 5;
	short oreCount = 0;

	for (int i = 0; i < aWidth * aHeight; ++i)
	{
		myWorldData.push_back(defaultTile);

		unsigned char x = i % aWidth;
		unsigned char y = i / aWidth;

		STile& currentTile = myWorldData.back();

		if (y < mySkyCutOff)
		{
			currentTile.myType = ETileType::Sky;
		}
		else if (y > mySkyCutOff)
		{
			if (rand() % 100 < chanceOfOre && oreCount < MAX_ORE_COUNT)
			{
				currentTile.myType = ETileType::Ore;
				myOres[oreCount] = i;
				oreCount++;
			}
		}
	}
}

void CServerWorld::SendWorldData(unsigned char aTargetID)
{
	CNetMessageWorldData::SWorldData data;
	data.myOres = myOres;
	data.mySkyCutOff = mySkyCutOff;
	data.myWorldHeight = myWorldHeight;
	data.myWorldWidth = myWorldWidth;

	data.mySenderID = 0;
	data.myTargetID = aTargetID;

	int worldSize = sizeof(myWorldData);

	myMessageManager->CreateGuaranteedMessage<CNetMessageWorldData>(data);
}

bool CServerWorld::RemoveBlock(unsigned short aBlockID)
{
	STile& block = myWorldData[aBlockID];

	if (block.myIsDestroyed == false && block.myType != ETileType::Sky)
	{
		block.myIsDestroyed = true;
		return true;
	}

	return false;
}
