#pragma once

#include <vector>

#include "Utility.hpp"

template<typename T>
class Database
{
protected:

	unsigned int GetSize()
	{
		FileHeader* pFileHeader = PeekFileHeader(mFilename);

		if (!pFileHeader)
			return 0;

		return pFileHeader->itemCount;
	}

	bool Load()
	{
		if (!mList.empty())
			return true;

		unsigned int fileBufferSize = 0;
		char* pFileBuffer = ReadFile(mFilename, fileBufferSize);

		if (!pFileBuffer)
			return false;

		mList.clear();

		FileHeader* pFileHeader = reinterpret_cast<FileHeader*>(pFileBuffer);

		const unsigned int startPos = sizeof(FileHeader);
		for (unsigned int i = 0; i < pFileHeader->itemCount; ++i)
		{
			T* pEntry = reinterpret_cast<T*>(&pFileBuffer[startPos + (i * sizeof(T))]);
			mList.push_back(*pEntry);
		}

		return true;
	}

	void Save()
	{
		FILE* pFile = nullptr;
		fopen_s(&pFile, mFilename, "wb");

		FileHeader header{ time(nullptr), mList.size() };
		fwrite(&header, sizeof(FileHeader), 1, pFile);

		for (const T& entry : mList)
			fwrite(&entry, sizeof(T), 1, pFile);

		fclose(pFile);
	}

	std::vector<T>& GetData() 
	{ 
		return mList; 
	}

protected:

	char			mFilename[64];
	std::vector<T>	mList;
};
