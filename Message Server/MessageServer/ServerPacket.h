#pragma once

enum class ServerPacket : int {
	None,
	Ping,
	Authentication
};