#include "CpLogin.h"

CpLogin::CpLogin() {

}

CpLogin::~CpLogin() {

}

void CpLogin::Process(Client* client, BYTE* bytes, int length) {
	ByteBuffer Buffer = ByteBuffer(bytes, length);
	std::wstring auth_code = Buffer.ReadString();
	std::wstring nome = Buffer.ReadString();
	std::wstring password = Buffer.ReadString();

	MessageBoxExW(NULL, nome.c_str(), L"MSG", 0, 0);
 }