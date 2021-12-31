#pragma once
#include <iostream>
#include <tchar.h>
#include <Windows.h>

// typedef char BYTE;

const int BYTE_SIZE = 1;
const int INT16_SIZE = 2;
const int INT32_SIZE = 4;

const int DEFAULT_BUFFER_LENGTH = 128;

class ByteBuffer {
public:
	ByteBuffer(int length);
	ByteBuffer(BYTE* c_buffer, int length);
	~ByteBuffer();

	void Write(BYTE* c_buffer, int length);
	void Write(BYTE value);
	void Write(int16_t value);
	void Write(int32_t  value);
	void Write(std::string value);

	BYTE* ReadBytes(int length, bool peek = true);
	BYTE ReadByte(bool peek = true);
	int16_t ReadInt16(bool peek = true);
	int32_t ReadInt32(bool peek = true);
	std::wstring ReadString(bool peek = true);

	void Trim();
	void Flush();

	int GetCapacity();
	int GetSize();

private:
	int Capacity;
	int Size;

	BYTE* buffer;
	int read_position;
};