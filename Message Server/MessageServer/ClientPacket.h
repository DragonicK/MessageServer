#pragma once

enum class ClientPacket : int {
	None,
	Ping,
	Authentication,
	Login,
	PacketCount
};