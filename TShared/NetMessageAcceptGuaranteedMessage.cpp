#include "stdafx.h"
#include "NetMessageAcceptGuaranteedMessage.h"


CNetMessageAcceptGuaranteedMessage::CNetMessageAcceptGuaranteedMessage()
{
}


CNetMessageAcceptGuaranteedMessage::~CNetMessageAcceptGuaranteedMessage()
{
}

void CNetMessageAcceptGuaranteedMessage::Create(SAcceptData aData)
{
	CNetMessage::Create(aData);
	myData.myID = EMessageType::AcceptGuaranteed;
}
