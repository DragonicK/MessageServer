#include "CpLogin.h"
#include "PacketFactory.h"

void PacketFactory::RegisterPackets() {
	factorymap[ClientPacket::Authentication] = &CpLogin::Create;
}

IRecvPacket* PacketFactory::CreatePacket(ClientPacket packet) {
	return factorymap[packet]();
}