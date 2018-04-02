#include "stdafx.h"
#include "NetMessageDestroyBlock.h"


CNetMessageDestroyBlock::CNetMessageDestroyBlock()
{
}


CNetMessageDestroyBlock::~CNetMessageDestroyBlock()
{
}

unsigned short CNetMessageDestroyBlock::GetBlockID()
{
	return myBlockID;
}

void CNetMessageDestroyBlock::DoSerialize()
{
	CNetMessage::DoSerialize();
	SERIALIZE(myBuffer, myBlockID);
}

void CNetMessageDestroyBlock::DoDeserialize()
{
	CNetMessage::DoDeserialize();
	DESERIALIZE(myBuffer, myBlockID);
}

void CNetMessageDestroyBlock::Create(SDestroyBlockData aData)
{
	CNetMessage::Create(aData);
	myData.myID = EMessageType::DestroyBlock;
	myBlockID = aData.myBlockID;
}
