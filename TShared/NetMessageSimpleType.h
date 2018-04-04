#pragma once
#include "NetMessage.h"

class CNetMessageSimpleType : public CNetMessage
{
	friend class CNetMessageManager;

public:
	CNetMessageSimpleType() = default;

	struct SSimpleTypeData : public CNetMessage::SNetMessageData
	{
		int myInt;
		bool myBool;
		float myFloat;
	};

	int GetInt() const;
	float GetFloat() const;
	bool GetBool() const;

private:
	void DoSerialize() override;
	void DoDeserialize() override;

	void Create(SSimpleTypeData aData);

	int myInt;
	bool myBool;
	float myFloat;
};
