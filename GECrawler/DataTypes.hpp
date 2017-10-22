#pragma once

#include <ctime>
#include <vector>
#include <string>

struct FileHeader
{
	time_t			timestamp;
	unsigned int	itemCount;
};

struct IndexEntry
{
	unsigned int	itemId;
	char			name[128];
	bool			membersItem;
};

struct Item
{
	time_t timestamp;
	unsigned int price;
};
