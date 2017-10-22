#include "Crawler.hpp"
#include "Logger.hpp"

Crawler::Crawler()
	: mpClient(new GEClient())
{

}

Crawler::~Crawler()
{
	if (mpClient)
		delete mpClient;

	mpClient = nullptr;
}

void Crawler::UpdateItemInfo()
{
	std::vector<IndexEntry>& index = Index::GetData();

	for (IndexEntry& entry : index)
	{
		IndexEntry* pNewEntry = nullptr;

		unsigned int retryCount = 5;
		while ((pNewEntry = mpClient->GetItemInfo(entry.itemId)) == nullptr && retryCount > 0)
		{
			Sleep(2000);
			retryCount -= 1;
		}

		if (pNewEntry)
		{
			Logger::Write("Received item %u data", pNewEntry->itemId);

			if (entry.itemId != pNewEntry->itemId)
			{
				Logger::Write("Item id mismatch, Old:%u, New:%u", entry.itemId, pNewEntry->itemId);
				entry.itemId = pNewEntry->itemId;
			}

			if (strcmp(entry.name, pNewEntry->name) != 0)
			{
				Logger::Write("Item name mismatch, Old:%s, New:%s", entry.name, pNewEntry->name);
				strcpy(entry.name, pNewEntry->name);
			}

			if (entry.membersItem != pNewEntry->membersItem)
			{
				Logger::Write("Item members flag mismatch, Old:%s, New:%s", entry.membersItem ? "true" : "false", pNewEntry->membersItem ? "true" : "false");
				entry.membersItem = pNewEntry->membersItem;
			}

			Index::Save();
			delete pNewEntry;
		}
	}
}

void Crawler::UpdateItemData(bool bOnlyNew)
{
	std::vector<IndexEntry>& index = Index::GetData();

	for (IndexEntry& entry : index)
	{
		std::vector<Item>* pNewData = nullptr;
		ItemData* pItemData = new ItemData(entry.itemId);
		int oldItemCount = pItemData->GetSize();

		if (bOnlyNew && oldItemCount > 0)
		{
			delete pItemData;
			continue;
		}

		unsigned int retryCount = 5;
		while ((pNewData = mpClient->GetItemData(entry.itemId)) == nullptr && retryCount > 0)
		{
			Sleep(2000);
			retryCount -= 1;
		}

		if (pNewData)
		{
			pItemData->Load();
			std::vector<Item>& itemDataList = pItemData->GetData();

			for (Item& newItem : *pNewData)
			{
				bool bItemExists = false;

				for (Item& item : itemDataList)
				{
					if (item.timestamp == newItem.timestamp)
						bItemExists = true;
				}

				if (!bItemExists)
					itemDataList.push_back(newItem);
			}

			Logger::Write("Received item %u data, %u new entries", entry.itemId, itemDataList.size() - oldItemCount);

			pItemData->Save();
			delete pNewData;
			delete pItemData;
		}
	}
}

std::vector<IndexEntry> Crawler::SearchItemInfo(unsigned int itemIdStart, unsigned int itemIdEnd, bool bOnlyNew)
{
	std::vector<IndexEntry> result;

	for (unsigned int itemId = itemIdStart; itemId < itemIdEnd; ++itemId)
	{
		if (bOnlyNew)
		{
			bool bItemExists = false;

			for (const IndexEntry& entry : Index::GetData())
			{
				if (entry.itemId == itemId)
				{
					bItemExists = true;
					break;
				}
			}

			if (bItemExists)
				continue;
		}

		Logger::Write("Testing item id:%d", itemId);

		IndexEntry* pNewEntry = nullptr;

		unsigned int retryCount = 1;
		while ((pNewEntry = mpClient->GetItemInfo(itemId)) == nullptr && retryCount > 0)
		{
			Sleep(2000);
			retryCount -= 1;
		}

		if (pNewEntry)
		{
			Logger::Write("New item:%d, %s, members item:%s", pNewEntry->itemId, pNewEntry->name, pNewEntry->membersItem ? "true" : "false");
			result.push_back(*pNewEntry);
			delete pNewEntry;
		}
	}

	return result;
}