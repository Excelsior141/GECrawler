#pragma once

#include "Database.hpp"
#include "Datatypes.hpp"

class Index : public Database<IndexEntry>
{
public:

	static void Save();
	static std::vector<IndexEntry>& GetData();

private:

	Index();

	void save();
	std::vector<IndexEntry>& getData();

	static Index* getInstance();

private:

	static Index* mpIndex;
};
