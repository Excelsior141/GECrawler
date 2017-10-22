#include "Index.hpp"
#include "Utility.hpp"

Index* Index::mpIndex = nullptr;

Index::Index()
{
	sprintf_s(mFilename, "Index.gex");
	__super::Load();
}

void Index::Save()
{
	Index::getInstance()->save();
}

std::vector<IndexEntry>& Index::GetData()
{
	return Index::getInstance()->getData();
}

void Index::save()
{
	__super::Save();
}

std::vector<IndexEntry>& Index::getData()
{
	return __super::GetData();
}

Index* Index::getInstance()
{
	if (!mpIndex)
		mpIndex = new Index();

	return mpIndex;
}