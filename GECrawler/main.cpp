#include "Logger.hpp"
#include "Index.hpp"
#include "Crawler.hpp"
#include "Analyzer.hpp"
#include "DataTypes.hpp"

int main(int argc, const char* argv[])
{
	Crawler* pCrawler = new Crawler();
	
	pCrawler->UpdateItemData(false);

	pCrawler->SearchItemInfo(0, 15000, true);

	delete pCrawler;
}