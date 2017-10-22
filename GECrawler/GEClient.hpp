#pragma once

#include "DataTypes.hpp"

#include <winsock2.h>
#include <ws2tcpip.h>

class GEClient
{
public:

	GEClient();
	~GEClient();

	IndexEntry* GetItemInfo(unsigned int itemId);
	std::vector<Item>* GetItemData(unsigned int itemId);

private:

	bool connect();
	void disconnect();

	bool sendItemInfoRequest(unsigned int itemId);
	bool sendItemDataRequest(unsigned int itemId);
	bool receive();

	void error(const char* pTitle);

private:

	static WSADATA WsaData;
	static addrinfo Hints;
	static addrinfo* Result;
	static unsigned int InstanceCount;

	SOCKET mSocket;
	
	char mSendBuffer[256];
	char mRecvBuffer[32768];
};
