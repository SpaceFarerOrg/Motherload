#include "stdafx.h"
#include "ServerGameObject.h"


CServerGameObject::CServerGameObject()
{
}


CServerGameObject::~CServerGameObject()
{
}

void CServerGameObject::Init(const NetVector2 & aPos)
{
	myPosition.x = aPos.x;
	myPosition.y = aPos.y;

	float angle = rand();
	myDirection.x = cosf(angle);
	myDirection.y = sinf(angle);

	mySpeed = 200.f;
	myDirectionTimer = (float)(rand() % 100) / 100.f * 2.f;
}

void CServerGameObject::Update(float aDT)
{
	myDirectionTimer += aDT;
	if (myDirectionTimer > 2.f)
	{
		myDirectionTimer = 0.f;
		static int maxRotationAngle = 70;
		float rotation = (float)(rand() % (maxRotationAngle)) / 360.f * 6.28f - ((maxRotationAngle / 2.f / 360.f) * 6.28f);
		NetVector2 currentDirection = myDirection;

		myDirection.x = currentDirection.x * cosf(rotation) - currentDirection.y * sinf(rotation);
		myDirection.y = currentDirection.x * sinf(rotation) + currentDirection.y * cosf(rotation);
	}

	myPosition.x += myDirection.x * mySpeed * aDT;
	myPosition.y += myDirection.y * mySpeed * aDT;

	if (myPosition.x < -32)
		myDirection.x *= -1.f;
	if (myPosition.x > 1600 + 32)
		myDirection.x *= -1.f;
	if (myPosition.y < -32)
		myDirection.y *= -1.f;
	if (myPosition.y > 900 + 32)
		myDirection.y *= -1.f;
}

NetVector2 CServerGameObject::GetPosition()
{
	return myPosition;
}
