#include <WinSock2.h>
#include "Client.h"

Client::Client() : Buffer(DEFAULT_BUFFER_LENGTH) {
	Index = 0;
	CSocket = NULL;
	Connected = false;
}

Client::Client(SOCKET sock, int index, int bufferSize) : Buffer(bufferSize) {
	Index = index;
	CSocket = sock;
	Connected = true;
}

Client::~Client() {
	CSocket = NULL;
	Index = 0;
	Connected = false;
}