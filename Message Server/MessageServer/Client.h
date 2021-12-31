#pragma once
#include "ByteBuffer.h"

class Client {
public:
	// Client Socket.
	SOCKET CSocket;
	// Buffer Data
	ByteBuffer Buffer;

	int Index;
	bool Connected;

	Client();
	Client(SOCKET sock, int index, int bufferSize);
	~Client();
private:

};