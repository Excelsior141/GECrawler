#include "GEClient.hpp"

WSADATA			GEClient::WsaData {};
addrinfo		GEClient::Hints {};
addrinfo*		GEClient::Result = nullptr;
unsigned int	GEClient::InstanceCount = 0;

GEClient::GEClient()
{
	if (InstanceCount == 0)
	{
		Hints.ai_family = AF_INET;
		Hints.ai_socktype = SOCK_STREAM;
		Hints.ai_protocol = IPPROTO_TCP;

		WSAStartup(MAKEWORD(2, 2), &WsaData);
		getaddrinfo("www.runescape.com", "http", &Hints, &Result);

		InstanceCount += 1;
	}
}

GEClient::~GEClient()
{
	InstanceCount -= 1;
	closesocket(mSocket);

	if (InstanceCount == 0)
	{
		freeaddrinfo(Result);
		WSACleanup();
	}
}

IndexEntry* GEClient::GetItemInfo(unsigned int itemId)
{
	IndexEntry* pResult = nullptr;

	if (connect() && sendItemInfoRequest(itemId) && receive())
	{
		std::string response = mRecvBuffer;

		const std::string idStart = "\"id\":";
		const std::string idEnd = ",";
		const std::string nameStart = "\"name\":\"";
		const std::string nameEnd = "\",";
		const std::string membersStart = "\"members\":\"";
		const std::string membersEnd = "\",";

		unsigned int startPos = 0;
		unsigned int endPos = 0;
		unsigned int sliceLength = 0;

		if (response.find(idStart, endPos) != std::string::npos)
		{
			pResult = new IndexEntry();

			startPos = response.find(idStart, endPos) + idStart.length();
			endPos = response.find(idEnd, startPos);
			sliceLength = endPos - startPos;

			std::string idToken = response.substr(startPos, sliceLength);
			pResult->itemId = atoi(idToken.c_str());

			startPos = response.find(nameStart, endPos) + nameStart.length();
			endPos = response.find(nameEnd, startPos);
			sliceLength = endPos - startPos;

			std::string nameToken = response.substr(startPos, sliceLength);
			strcpy_s(pResult->name, nameToken.c_str());

			startPos = response.find(membersStart, endPos) + membersStart.length();
			endPos = response.find(membersEnd, startPos);
			sliceLength = endPos - startPos;

			std::string membersToken = response.substr(startPos, sliceLength);
			pResult->membersItem = (membersToken == "true");
		}
	}

	disconnect();

	return pResult;
}

std::vector<Item>* GEClient::GetItemData(unsigned int itemId)
{
	std::vector<Item>* pResult = nullptr;

	if (connect() && sendItemDataRequest(itemId) && receive())
	{
		pResult = new std::vector<Item>;

		std::string response = mRecvBuffer;

		const std::string dailyStart = "{\"daily\":{";
		const std::string dailyEnd = "},\"average\"";

		const std::string timestampStart = "\"";
		const std::string timestampEnd = "000\"";
		const std::string priceStart = ":";
		const std::string priceEnd = ",";

		unsigned int dailyChunkStart = response.find(dailyStart, 0) + dailyStart.length();
		unsigned int dailyChunkEnd = response.find(dailyEnd, dailyChunkStart);
		unsigned int dailyChunkLength = dailyChunkEnd - dailyChunkStart;

		std::string dailyChunk = response.substr(dailyChunkStart, dailyChunkLength) + ",";

		unsigned int startPos = 0;
		unsigned int endPos = 0;
		unsigned int sliceLength = 0;

		while (dailyChunk.find(timestampStart, endPos) != std::string::npos)
		{
			Item newItem {};

			startPos = dailyChunk.find(timestampStart, endPos) + timestampStart.length();
			endPos = dailyChunk.find(timestampEnd, startPos);
			sliceLength = endPos - startPos;

			std::string timestampToken = dailyChunk.substr(startPos, sliceLength);
			newItem.timestamp = atoi(timestampToken.c_str());

			startPos = dailyChunk.find(priceStart, endPos) + priceStart.length();
			endPos = dailyChunk.find(priceEnd, startPos);
			sliceLength = endPos - startPos;

			std::string priceToken = dailyChunk.substr(startPos, sliceLength);
			newItem.price = atoi(priceToken.c_str());

			pResult->push_back(newItem);
		}
	}

	disconnect();

	return pResult;
}

bool GEClient::connect()
{
	mSocket = socket(Result->ai_family, Result->ai_socktype, Result->ai_protocol);

	if (::connect(mSocket, Result->ai_addr, (int)Result->ai_addrlen) != 0)
		return false;

	return true;
}

void GEClient::disconnect()
{
	shutdown(mSocket, SD_BOTH);
	closesocket(mSocket);
}

bool GEClient::sendItemInfoRequest(unsigned int itemId)
{
	ZeroMemory(mSendBuffer, sizeof(mSendBuffer));
	ZeroMemory(mRecvBuffer, sizeof(mRecvBuffer));

	sprintf(mSendBuffer, "GET http://services.runescape.com/m=itemdb_oldschool/api/catalogue/detail.json?item=%d\r\n HTTP/1.1\r\n\r\n", itemId);

	if (::send(mSocket, mSendBuffer, (int)strlen(mSendBuffer), 0) <= 0)
		return false;

	return true;
}

bool GEClient::sendItemDataRequest(unsigned int itemId)
{
	ZeroMemory(mSendBuffer, sizeof(mSendBuffer));
	ZeroMemory(mRecvBuffer, sizeof(mRecvBuffer));

	sprintf(mSendBuffer, "GET http://services.runescape.com/m=itemdb_oldschool/api/graph/%d.json\r\n HTTP/1.1\r\n\r\n", itemId);

	if (::send(mSocket, mSendBuffer, (int)strlen(mSendBuffer), 0) <= 0)
		return false;

	return true;
}

bool GEClient::receive()
{
	memset(mRecvBuffer, 0, sizeof(mRecvBuffer));

	if (recv(mSocket, mRecvBuffer, sizeof(mRecvBuffer), MSG_WAITALL) <= 0)
		return false;

	std::string response = mRecvBuffer;

	if (response.find("404 - Page not found", 0) != std::string::npos)
		return false;
	
	return true;
}

void GEClient::error(const char* pTitle)
{
	int error = WSAGetLastError();
	MessageBoxA(0, std::to_string(error).c_str(), pTitle, 0);
}