#pragma once

#include "Database.hpp"
#include "Datatypes.hpp"

class ItemData : public Database<Item>
{
public:

	ItemData(unsigned int itemId);

	void Load();
	void Save();
	unsigned int GetSize();
	std::vector<Item>& GetData();

	unsigned int GetItemId() { return mItemId; }

private:

	unsigned int mItemId;
};
