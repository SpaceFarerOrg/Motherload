#pragma once
#include "NetMessage.h"

class CNetMessageNewClient : public CNetMessage
{
	friend class CNetMessageManager;

public:

	CNetMessageNewClient();

	struct SNetMessageNewClientData : public SNetMessageData
	{
		size_t myConnectedClient;
	};
public:

	size_t GetConnectedClient() const;

private:
	void DoSerialize() override;
	void DoDeserialize() override;

	void Create(SNetMessageNewClientData aData);

	size_t myConnectedClient;

};
