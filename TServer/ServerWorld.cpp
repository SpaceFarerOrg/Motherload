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

	mySkyCutOff = 5;
	int chanceOfOre = 0;
	short oreCount = 0;

	for (int i = 0; i < aWidth * aHeight; ++i)
	{
		myWorldData.push_back(defaultTile);

		unsigned char x = i % aWidth;
		unsigned char y = i / aWidth;

		chanceOfOre = ((float)y / myWorldHeight) * 10;

		STile& currentTile = myWorldData.back();

		if (y < mySkyCutOff)
		{
			currentTile.myType = ETileType::Sky;
		}
		else if (y == mySkyCutOff && x > 4 && x < myWorldWidth - 5)
		{
			currentTile.myType = ETileType::Rock;
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

	for (int i = 0; i < myWorldData.size(); ++i)
	{
		if (myWorldData[i].myIsDestroyed)
		{
			CNetMessageDestroyBlock::SDestroyBlockData destroyData;
			destroyData.myBlockID = i;
			destroyData.myTargetID = aTargetID;
			destroyData.mySenderID = 0;
			
			myMessageManager->CreateGuaranteedMessage<CNetMessageDestroyBlock>(destroyData);
		}
	}
}

bool CServerWorld::RemoveBlock(unsigned short aBlockID)
{
	if (aBlockID < 0 || aBlockID >= myWorldData.size())
		return false;

	STile& block = myWorldData[aBlockID];

	if (block.myIsDestroyed == false && block.myType != ETileType::Sky && block.myType != ETileType::Rock)
	{
		block.myIsDestroyed = true;
		return true;
	}

	return false;
}

unsigned short CServerWorld::GetWidth()
{
	return (unsigned short)myWorldWidth;
}

SBlockPos CServerWorld::GetBlockPosFromID(unsigned short aBlockID)
{
	SBlockPos returnPos;
	returnPos.myX = (float)(aBlockID % myWorldWidth) * 64.f;
	returnPos.myY = (float)(aBlockID / myWorldWidth) * 64.f;

	return std::move(returnPos);
}

ETileType CServerWorld::GetBlockTypeFromID(size_t aID) const
{
	return myWorldData[aID].myType;
}
