#include "stdafx.h"
#include "NetMessageManager.h"
#include "NetMessageAcceptGuaranteedMessage.h"


void CNetMessageManager::SetID(size_t aID)
{
	myID = aID;
}

void CNetMessageManager::Init(size_t aBufferSize, SOCKET aSocketToUse)
{
	myMessages.reserve(aBufferSize);
	mySocket = aSocketToUse;

	myTimer = 0.f;
	myAvailableID = 0;
}

void CNetMessageManager::Flush(const std::vector<sockaddr_in>& aAddressList)
{
	FlushRegularMessages(aAddressList);
	SendGuaranteedMessages(aAddressList);
}

void CNetMessageManager::Update(float aDT)
{
	myTimer += aDT;

	if (myTimer >= 1.f)
	{
		mySentLastSecond = mySentThisSecond;
		mySentThisSecond = 0;
		myTimer = 0.f;
	}
}

int CNetMessageManager::GetSentDataLastSecond()
{
	return mySentLastSecond;
}

void CNetMessageManager::AcceptGuaranteedMessage(unsigned short aToID, unsigned short aFromID, unsigned int aMessageID)
{
	CNetMessageAcceptGuaranteedMessage::SAcceptData data;

	data.myTargetID = aToID;
	data.mySenderID = aFromID;
	data.myMessageID = aMessageID;

	CreateMessage<CNetMessageAcceptGuaranteedMessage>(data);
}

void CNetMessageManager::VerifyGuaranteedMessage(unsigned int aMessageID)
{
	if (myGuaranteedMessages.find(aMessageID) != myGuaranteedMessages.end())
	{
		delete myGuaranteedMessages[aMessageID];
		myGuaranteedMessages.erase(aMessageID);
		//PRINT("Verified a guaranteed message");
	}
}

void CNetMessageManager::FlushRegularMessages(const std::vector<sockaddr_in>& aAddressList)
{
	for (int i = 0; i < myMessages.size(); ++i)
	{
		myMessages[i]->PackMessage();
		short excludedTarget = myMessages[i]->GetData().myTargetID < 0 ? abs(myMessages[i]->GetData().myTargetID) : 0;
		bool toAll = excludedTarget >= 0;
		bool toSpecific = myMessages[i]->GetData().myTargetID > 0;

		if (toSpecific)
		{
			toAll = false;
		}

		for (int j = 1; j <= aAddressList.size(); ++j)
		{
			if (j == excludedTarget)
				continue;

			short target = myMessages[i]->GetData().myTargetID;


			if ((toSpecific && j == target) || toAll)
			{
				int sendResult = sendto(mySocket, myMessages[i]->GetBufferStart(), myMessages[i]->GetBufferSize(), 0, (sockaddr*)(&aAddressList[j - 1]), sizeof(aAddressList[j - 1]));
				mySentThisSecond += sendResult;
			}
		}
		delete myMessages[i];
	}
	myMessages.clear();
}

void CNetMessageManager::SendGuaranteedMessages(const std::vector<sockaddr_in>& aAddressList)
{
	for (auto& pair : myGuaranteedMessages)
	{
		CNetMessage& message = *pair.second;

		message.PackMessage();

		int target = message.GetData().myTargetID;
		if (target > 0) target -= 1;

		if (aAddressList.size() > target)
		{
			int sendResult = sendto(mySocket, message.GetBufferStart(), message.GetBufferSize(), 0, (sockaddr*)(&aAddressList[target]), sizeof(sockaddr_in));
			mySentThisSecond += sendResult;
		}
	}

	//if (myGuaranteedMessages.size() > 0)
		//PRINT("Sent " + std::to_string(myGuaranteedMessages.size()) + " guaranteed messages.");
}
