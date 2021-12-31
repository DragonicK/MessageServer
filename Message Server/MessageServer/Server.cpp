#include <thread>
#include "ServerSocket.h"
#include "HandleData.h"

ServerSocket server;
std::thread accept_t;
std::thread receive_t;

void AcceptClient();
void ReceiveData();

void ServerInit() {
	server.Port = const_cast<PWSTR>(L"7009");
	server.MaximmumClients = 100;

	if (server.CreateSocket()) {
		server.Running = true;

		RegisterPackets();

		accept_t = std::thread(AcceptClient);
		accept_t.detach();

		receive_t = std::thread(ReceiveData);
		receive_t.detach();
	}
}

void AcceptClient() {
	server.AcceptClient();
}

void ReceiveData() {
	server.ReceiveData();
}

void ServerClose() {
	server.Running = false;
	server.DestroySocket();
}