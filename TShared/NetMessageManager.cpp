#include "stdafx.h"
#include "NetMessageManager.h"



void CNetMessageManager::Init(size_t aBufferSize, SOCKET aSocketToUse)
{
	myMessages.reserve(aBufferSize);
	mySocket = aSocketToUse;
}

void CNetMessageManager::Flush(const std::vector<sockaddr_in>& aAddressList)
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
			if(j == excludedTarget)
				continue;

			short target = myMessages[i]->GetData().myTargetID;


			if ((toSpecific && j == target) || toAll)
			{
				int sendResult = sendto(mySocket, myMessages[i]->GetBufferStart(), myMessages[i]->GetBufferSize(), 0, (sockaddr*)(&aAddressList[j-1]), sizeof(aAddressList[j-1]));
			}
		}
		delete myMessages[i];
	}
	myMessages.clear();
}
