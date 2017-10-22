#include "Analyzer.hpp"

Analyzer::Analyzer()
{
	for (IndexEntry& entry : Index::GetData())
	{
		ItemData* itemData = new ItemData(entry.itemId);
		mItemDatabase.push_back(itemData);
	}
}

Analyzer::~Analyzer()
{
	for (ItemData* pItemData : mItemDatabase)
		delete pItemData;

	mItemDatabase.clear();
}

unsigned int Analyzer::GetAverageItemPrice(unsigned int itemId)
{
	unsigned int result = 0;
	ItemData* pItemToFind = nullptr;

	for (ItemData* pItemData : mItemDatabase)
	{
		if (pItemData->GetItemId() == itemId)
		{
			pItemToFind = pItemData;
			break;
		}
	}

	if (pItemToFind)
	{
		pItemToFind->Load();

		double sum = 0.0;

		for (const Item& item : pItemToFind->GetData())
			sum += item.price;

		sum /= pItemToFind->GetData().size();

		result = sum;
	}

	return result;
}