#pragma once

#include <map>
#include "ClientPacket.h"
#include "IRecvPacket.h"

class PacketFactory {

public:	
	void RegisterPackets();
	IRecvPacket* CreatePacket(ClientPacket packet);

private:
	typedef IRecvPacket* (*CreateFunc)();
	std::map<ClientPacket, CreateFunc> factorymap;
};