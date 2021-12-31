#pragma once
#pragma comment(lib, "Ws2_32.lib")

#include <WinSock2.h>
#include <ws2tcpip.h>
#include <tchar.h>
#include "Client.h"

class ServerSocket {

public:
	PWSTR Port;
	BOOL Running;
	Client* Clients;
	int MaximmumClients;

	ServerSocket();
	~ServerSocket();

	int CreateSocket();
	void DestroySocket();
	void AcceptClient();
	void ReceiveData();

	int FindFreeSlot();
	void CreateClients();
private:
	SOCKET Listener;
};