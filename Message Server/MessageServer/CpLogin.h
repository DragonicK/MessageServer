#pragma once
#include "IRecvPacket.h"

class CpLogin : protected IRecvPacket {
public:
	static IRecvPacket* Create() {
		return new CpLogin();
	}

	CpLogin();
	~CpLogin();

	void Process(Client* client, BYTE* bytes, int bytesLength);
};