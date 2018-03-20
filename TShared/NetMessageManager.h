#pragma once
#pragma warning(disable:4996)
#define _SCL_SECURE_NO_WARNINGS

#include "CommonNetworkIncludes.h"
#include <vector>
#include "NetMessage.h"
#include <chrono>

class CNetMessageManager
{
public:
	CNetMessageManager() = default;
	~CNetMessageManager() = default;

	

	void Init(size_t aBufferSize, SOCKET aSocketToUse);

	template<typename T, typename SData>
	void CreateMessage(SData aData);

	void Flush(const std::vector<sockaddr_in>& aAddressList);

private:
	std::vector<CNetMessage*> myMessages;
	SOCKET mySocket;
};

template<typename T, typename SData>
inline void CNetMessageManager::CreateMessage(SData aData)
{
	T* newMessage = new T();

	std::chrono::seconds msSinceStartOfTime = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch());
	aData.myTimeStamp = msSinceStartOfTime.count();

	newMessage->Create(aData);

	myMessages.push_back(newMessage);
}
