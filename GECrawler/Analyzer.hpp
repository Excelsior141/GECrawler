#pragma once

#include "DataTypes.hpp"
#include "ItemData.hpp"
#include "Index.hpp"

class Analyzer
{
public:

	Analyzer();
	~Analyzer();

	unsigned int GetAverageItemPrice(unsigned int itemId);

private:

	std::vector<ItemData*> mItemDatabase;

};