#include "ByteBuffer.h"
#include <string>

ByteBuffer::ByteBuffer(int length) {
	buffer = new BYTE[length];
	Capacity = length;
	Size = 0;
	read_position = 0;
}

ByteBuffer::ByteBuffer(BYTE* c_buffer, int length) {
	buffer = new BYTE[length];
	memcpy(buffer, c_buffer, sizeof(char) * length);
	Capacity = length;
	Size = length;
	read_position = 0;
}

ByteBuffer::~ByteBuffer() {
	delete[] buffer;
}

void ByteBuffer::Write(BYTE* c_buffer, int length) {
	// Cria um novo buffer quando não há mais espaços.
	if (Size + length > Capacity) {
		// Cria um novo buffer para backup.
		BYTE* backup = new BYTE[Size];
		// Copia os dados para o backup.
		memcpy(backup, buffer, Size);

		// Deleta o ponteiro e aloca novamente com o tamanho necessário.
		delete[] buffer;
		buffer = new BYTE[(double)Size + length];

		// Copia o backup para o buffer.
		memcpy(buffer, backup, Size);
		// Adiciona os novos dados.
		memcpy((BYTE*)(buffer + Size), c_buffer, length);

		delete[] backup;

		Capacity = Size + length;
	}
	else {
		memcpy((BYTE*)(buffer + Size), c_buffer, length);
	}

	Size += length;
}

void ByteBuffer::Write(BYTE value) {
	BYTE values[BYTE_SIZE];
	values[0] = value;

	Write(values, BYTE_SIZE);
}

void ByteBuffer::Write(int16_t value) {
	BYTE values[INT16_SIZE];
	memcpy(&values, &value, INT16_SIZE);

	Write(values, INT16_SIZE);
}

void ByteBuffer::Write(int32_t value) {
	BYTE values[INT32_SIZE];
	memcpy(values, &value, INT32_SIZE);

	Write(values, INT32_SIZE);
}

void ByteBuffer::Write(std::string value) {
	int length = value.length();

	if (length > 0) {
		BYTE* values = new BYTE[length];

		for (int i = 0; i < length; i++) {
			values[i] = (unsigned char)value[i];
		}

		Write(length);
		Write(values, length);

		delete[] values;
	}
}

BYTE* ByteBuffer::ReadBytes(int length, bool peek) {
	if (read_position + length > Size) {
		return NULL;
	}

	BYTE* values = new BYTE[length];

	memcpy(values, (BYTE*)buffer + read_position, length);

	if (peek) {
		read_position += length;
	}

	return values;
}

BYTE ByteBuffer::ReadByte(bool peek) {
	if (read_position + BYTE_SIZE > Size) {
		return 0;
	}

	BYTE value = *(buffer + read_position);

	if (peek) {
		read_position += BYTE_SIZE;
	}

	return value;
}

int16_t ByteBuffer::ReadInt16(bool peek) {
	if (read_position + INT16_SIZE > Size) {
		return 0;
	}

	int16_t value;
	memcpy(&value, (BYTE*)(buffer + read_position), INT16_SIZE);

	if (peek) {
		read_position += INT16_SIZE;
	}

	return value;
}

int32_t ByteBuffer::ReadInt32(bool peek) {
	if (read_position + INT32_SIZE > Size) {
		return 0;
	}

	int32_t value;
	memcpy(&value, (BYTE*)(buffer + read_position), INT32_SIZE);

	if (peek) {
		read_position += INT32_SIZE;
	}

	return value;
}

std::wstring ByteBuffer::ReadString(bool peek) {
	int length = ReadInt32(peek);

	if (length > 0) {
		std::wstring s;

		char* text = new char[length + 1.0];
		*(text + length) = '\0';

		memcpy(text, (char*)(buffer + read_position), length);

		int wchars_num = MultiByteToWideChar(CP_UTF7, 0, text, -1, NULL, 0);
		wchar_t* wstr = new wchar_t[wchars_num];

		MultiByteToWideChar(CP_UTF7, 0, text, -1, wstr, wchars_num);

		s.append(wstr);

		delete[] wstr;
		delete[] text;

		if (peek) {
			read_position += length;
		}

		return s;
	}

	return L"";
}

void ByteBuffer::Trim() {
	if (read_position >= Size) {
		Flush();
	}
}

void ByteBuffer::Flush() {
	read_position = 0;
	Size = 0;
}

int ByteBuffer::GetCapacity() {
	return Capacity;
}

int ByteBuffer::GetSize() {
	return Size;
}