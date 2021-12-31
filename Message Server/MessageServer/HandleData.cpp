#include "ByteBuffer.h"
#include "HandleData.h"
#include "PacketFactory.h"

PacketFactory factory;

void RegisterPackets() {
	factory.RegisterPackets();
}

void HandleData(int header, Client* client, BYTE* data, int length) {
	int count = static_cast<int>(ClientPacket::PacketCount);

	// Se o cabeçalho não estiver dentro do alcance.
	if (header <= 0 || header >= count) {
		return;
	}

	BYTE* buffer = new BYTE[length];
	memcpy(buffer, data, length);

	IRecvPacket* packet = factory.CreatePacket(static_cast<ClientPacket>(header));

	packet->Process(client, buffer, length);

	delete[] buffer;
	delete packet;
}