#include "ItemData.hpp"
#include "Utility.hpp"

ItemData::ItemData(unsigned int itemId)
	: mItemId(itemId)
{
	sprintf_s(mFilename, "Item%04d.gec", itemId);
}

void ItemData::Load()
{
	if (!__super::Load())
	{
		__super::Save();
		__super::Load();
	}
}

void ItemData::Save()
{
	__super::Save();
}

unsigned int ItemData::GetSize()
{
	return __super::GetSize();
}

std::vector<Item>& ItemData::GetData()
{
	return __super::GetData();
}