#pragma once

#include <stdio.h>
#include "DataTypes.hpp"

static FileHeader* PeekFileHeader(const char* pFilename)
{
	FileHeader* pFileHeader = nullptr;
	char* pHeaderBuffer = nullptr;
	FILE* pFileHandle = nullptr;
	unsigned int fileSize = 0;
	fopen_s(&pFileHandle, pFilename, "rb");

	if (!pFileHandle)
		return nullptr;

	fseek(pFileHandle, 0, SEEK_END);
	fileSize = ftell(pFileHandle);
	fseek(pFileHandle, 0, SEEK_SET);

	if (fileSize < sizeof(FileHeader))
		return nullptr;

	pHeaderBuffer = new char[sizeof(FileHeader)];

	fread(pHeaderBuffer, sizeof(FileHeader), sizeof(char), pFileHandle);
	fclose(pFileHandle);

	pFileHeader = new FileHeader;
	memcpy(pFileHeader, pHeaderBuffer, sizeof(FileHeader));
	delete[] pHeaderBuffer;

	return pFileHeader;
}

static char* ReadFile(const char* pFilename, unsigned int& rFileBufferSize)
{
	char* pFileBuffer = nullptr;
	FILE* pFileHandle = nullptr;
	fopen_s(&pFileHandle, pFilename, "rb");

	if (!pFileHandle)
		return nullptr;

	fseek(pFileHandle, 0, SEEK_END);
	rFileBufferSize = ftell(pFileHandle);
	fseek(pFileHandle, 0, SEEK_SET);

	pFileBuffer = new char[rFileBufferSize];

	fread(pFileBuffer, rFileBufferSize, sizeof(char), pFileHandle);
	fclose(pFileHandle);

	return pFileBuffer;
}

static void WriteFile(char* pFilebuffer, unsigned int fileBufferSize, const char* pFilename, const char* pMode)
{
	FILE* pFileHandle = nullptr;
	fopen_s(&pFileHandle, pFilename, "wb");

	fwrite(pFilebuffer, sizeof(char), fileBufferSize, pFileHandle);

	fclose(pFileHandle);
}
