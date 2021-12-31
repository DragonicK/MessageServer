#pragma once
#include "Client.h"

void RegisterPackets();
void HandleData(int header, Client* client, BYTE* data, int pLength);