#pragma once
#include "NetMessage.h"

struct SNetMessageConnectData : public CNetMessage::SNetMessageData
{
	SMessage myClientConnectName;
};

class CNetMessageConnect : public CNetMessage
{
	friend class CNetMessageManager;

public:
	CNetMessageConnect();
	~CNetMessageConnect();

	std::string GetClientName() const;
private:
	void DoSerialize() override;
	void DoDeserialize() override;

	void Create(SNetMessageConnectData aData);

	SMessage myClientConnectName;
};