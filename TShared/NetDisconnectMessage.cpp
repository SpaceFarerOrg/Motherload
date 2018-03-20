#include "stdafx.h"
#include "NetDisconnectMessage.h"

void CNetDisconnectMessage::Create(SNetMessageData aData)
{
	CNetMessage::Create(aData);
	myData.myID = EMessageType::Disconnect;
}
