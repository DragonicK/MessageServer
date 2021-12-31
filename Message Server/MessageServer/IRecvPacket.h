#pragma once
#include <Windows.h>
#include "Client.h"

class IRecvPacket {
public:
	IRecvPacket();
	virtual void Process(Client* client, BYTE* bytes, int length) = 0;
};