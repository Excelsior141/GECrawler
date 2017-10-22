#pragma once

#include "DataTypes.hpp"

#include "GEClient.hpp"
#include "Index.hpp"
#include "ItemData.hpp"
#include "Database.hpp"

class Crawler
{
public:

	Crawler();
	~Crawler();

	void UpdateItemInfo();
	void UpdateItemData(bool bOnlyNew);

	std::vector<IndexEntry> SearchItemInfo(unsigned int itemIdStart = 0, unsigned int itemIdEnd = 15000, bool bOnlyNew = false);

private:

	GEClient* mpClient;
};