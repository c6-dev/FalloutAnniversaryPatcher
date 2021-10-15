#pragma once
#include <Windows.h>
class FileStream
{
protected:
	HANDLE		theFile;
	unsigned int		streamLength;
	unsigned int		streamOffset;

public:
	FileStream() : theFile(INVALID_HANDLE_VALUE), streamLength(0), streamOffset(0) {}
	~FileStream() { if (theFile != INVALID_HANDLE_VALUE) Close(); }


	HANDLE GetHandle() const { return theFile; }
	bool HitEOF() const { return streamOffset >= streamLength; }

	bool Open(const char* filePath);
	void SetOffset(unsigned int inOffset);

	void Close()
	{
		CloseHandle(theFile);
		theFile = INVALID_HANDLE_VALUE;
	}

	void ReadBuf(void* outData, DWORD inLength);

};