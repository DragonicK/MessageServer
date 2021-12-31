#include "ServerSocket.h"
#include "ByteBuffer.h"
#include "HandleData.h"

#include <Windows.h>

ServerSocket::ServerSocket() {
	Listener = NULL;
	Running = false;
	Port = const_cast<PWSTR>(L"7009");
	MaximmumClients = 100;
}

ServerSocket::~ServerSocket() {
	delete[] Clients;
}

BOOL ServerSocket::CreateSocket() {
	WSAData wsaData;

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		return FALSE;
	}

	PADDRINFOW result;
	addrinfoW hints;

	ZeroMemory(&hints, sizeof(hints));

	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	int iResult = GetAddrInfoW(NULL, Port, &hints, &result);

	if (iResult != 0) {
		FreeAddrInfoW(result);
		WSACleanup();
		return FALSE;
	}

	Listener = socket(result->ai_family, result->ai_socktype, result->ai_protocol);

	if (Listener == INVALID_SOCKET) {
		FreeAddrInfoW(result);
		WSACleanup();
		return FALSE;
	}

	u_long iMode = 1;
	ioctlsocket(Listener, FIONBIO, &iMode);

	iResult = bind(Listener, result->ai_addr, result->ai_addrlen);

	if (iResult == SOCKET_ERROR) {
		FreeAddrInfoW(result);
		closesocket(Listener);
		WSACleanup();
		return FALSE;
	}

	FreeAddrInfoW(result);

	if (listen(Listener, SOMAXCONN) == SOCKET_ERROR) {
		closesocket(Listener);
		WSACleanup();
		return FALSE;
	}

	CreateClients();

	return TRUE;
}

void ServerSocket::DestroySocket() {
	closesocket(Listener);
	WSACleanup();
}

void ServerSocket::AcceptClient() {
	while (Running) {
		SOCKET sock = accept(Listener, NULL, NULL);

		if (sock != INVALID_SOCKET) {		
			int index = FindFreeSlot();

			if (index >= 0) {
				Clients[index].CSocket = sock;
				Clients[index].Connected = true;
			}
			else {
				closesocket(sock);
			}
  		}

		Sleep(1);
	}
}

void ServerSocket::ReceiveData() {
	unsigned char recvbuf[DEFAULT_BUFFER_LENGTH + 1];
	int recvbuflen = DEFAULT_BUFFER_LENGTH;
	int iResult = 0;
	Client* p;

	while (Running) {
		for (int i = 0; i < MaximmumClients; i++) {
			p = &Clients[i];

			if (!p->Connected) {
				continue;
			}

			do {
				ZeroMemory(&recvbuf, DEFAULT_BUFFER_LENGTH);
				iResult = recv(p->CSocket, (char*)recvbuf, recvbuflen, 0);

				if (iResult > 0) {
					p->Buffer.Write((BYTE*)recvbuf, iResult);

					int pLength = 0;
					if (p->Buffer.GetSize() >= 4) {
						pLength = p->Buffer.ReadInt32(false);

						if (pLength < 0) {
							return;
						}
					}

					while (pLength > 0 && pLength <= p->Buffer.GetSize() - 4) {
						if (pLength <= p->Buffer.GetSize() - 4) {
							//Remove the first packet (packet size).
							p->Buffer.ReadInt32();
							// Remove the header.
							int header = p->Buffer.ReadInt32();
							// Decrease 4 bytes of header.
							pLength -= 4;

							BYTE* bytes = p->Buffer.ReadBytes(pLength);

							HandleData(header, p, bytes, pLength);

							delete[] bytes;
						}

						pLength = 0;

						if (p->Buffer.GetSize() >= 4) {
							pLength = p->Buffer.ReadInt32(false);

							if (pLength < 0) {
								return;
							}
						}
					}

					p->Buffer.Trim();
				}

			} while (iResult > 0);
		}

		Sleep(1);
	}
}

int ServerSocket::FindFreeSlot() {
	for (int i = 0; i < MaximmumClients; i++) {
		if (!Clients[i].Connected) {
			return i;
		}
	}

	return -1;
}

void ServerSocket::CreateClients() {
	Clients = new Client[MaximmumClients];

	for (int i = 0; i < MaximmumClients; i++) {
		Clients[i].Index = i;
	}
}