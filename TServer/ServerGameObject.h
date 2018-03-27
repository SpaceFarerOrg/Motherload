#pragma once
#include "CommonNetworkIncludes.h"

class CServerGameObject
{
public:
	CServerGameObject();
	~CServerGameObject();

	void Init(const NetVector2& aPos);
	void Update(float aDT);

	NetVector2 GetPosition();

private:
	NetVector2 myPosition;
	float mySpeed;
	NetVector2 myDirection;
	float myDirectionTimer;
};

