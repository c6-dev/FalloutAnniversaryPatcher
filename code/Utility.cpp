#include "Utility.h"
bool FileStream::Open(const char* filePath)
{
	theFile = CreateFileA(filePath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (theFile == INVALID_HANDLE_VALUE)
		return false;
	streamLength = GetFileSize(theFile, NULL);
	return true;
}

void FileStream::SetOffset(unsigned int inOffset)
{
	if (inOffset > streamLength)
		streamOffset = streamLength;
	else streamOffset = inOffset;
	SetFilePointer(theFile, streamOffset, NULL, FILE_BEGIN);
}

void FileStream::ReadBuf(void* outData, DWORD inLength)
{
	DWORD bytesRead;
	ReadFile(theFile, outData, inLength, &bytesRead, NULL);
	streamOffset += bytesRead;
}
