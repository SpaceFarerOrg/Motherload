#pragma once
#pragma warning(disable:4996)
#define _SCL_SECURE_NO_WARNINGS

#include "CommonNetworkIncludes.h"
#include "NetMessage.h"
#include <vector>
#include <unordered_map>
#include <chrono>

class CNetMessageManager
{
public:
	CNetMessageManager() = default;
	~CNetMessageManager() = default;

	void SetID(size_t aID);

	void Init(size_t aBufferSize, SOCKET aSocketToUse);

	template<typename T, typename SData>
	void CreateMessage(SData aData);

	template<typename T, typename SData>
	void CreateGuaranteedMessage(SData aData);

	void Flush(const std::vector<sockaddr_in>& aAddressList);

	void Update(float aDT);
	int GetSentDataLastSecond();

	void AcceptGuaranteedMessage(unsigned short aToID, unsigned short aFromID, unsigned int aMessageID);
	void VerifyGuaranteedMessage(unsigned int aMessageID);

private:
	void FlushRegularMessages(const std::vector<sockaddr_in>& aAddressList);
	void SendGuaranteedMessages(const std::vector<sockaddr_in>& aAddressList);

	std::vector<CNetMessage*> myMessages;
	std::unordered_map<unsigned int, CNetMessage*> myGuaranteedMessages;
	SOCKET mySocket;

	size_t myID;

	float myTimer;
	int mySentLastSecond;
	int mySentThisSecond;
	unsigned int myAvailableID;
};

template<typename T, typename SData>
inline void CNetMessageManager::CreateMessage(SData aData)
{
	T* newMessage = new T();

	std::chrono::seconds msSinceStartOfTime = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch());
	aData.myTimeStamp = msSinceStartOfTime.count();

	aData.mySenderID = myID;
	newMessage->Create(aData);

	myMessages.push_back(newMessage);
}

template<typename T, typename SData>
inline void CNetMessageManager::CreateGuaranteedMessage(SData aData)
{
	T* newMessage = new T();

	if (aData.myTargetID <= 0)
	{
		PRINT("Do not use targetID TO_ALL with guaranteed messages, instead create a message per reciever.");
	}
	else
	{
		std::chrono::seconds msSinceStartOfTime = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch());
		aData.myTimeStamp = msSinceStartOfTime.count();
		aData.myMessageID = ++myAvailableID;

		aData.mySenderID = myID;
		newMessage->Create(aData);

		myGuaranteedMessages.insert(std::pair<unsigned int, CNetMessage*>(aData.myMessageID, newMessage));
	}
}
