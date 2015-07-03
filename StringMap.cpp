#include "Global.h"

#include "StringMap.h"

vector<map<string, const char *>*> paged_stringmaps;

map<string, const char *> *current_stringmap()
{
	if ((current_app_page + 1) > paged_stringmaps.size()) {
		paged_stringmaps.push_back(new map<string, const char *>());
	}
	return paged_stringmaps[current_app_page];
}

void insert_string(const char *str)
{
	auto map = *current_stringmap();
	map[str] = str;
}

const char *get_string(const char *str)
{
	//int page = 0;
	//for (auto it = paged_stringmaps.begin(); it != paged_stringmaps.end(); ++it, ++page) {
	//	auto map = (*it);
	//	if (!map[str]) map[str] = str;
	//	return pair<int, const char *>(page, map[str]);
	//}
	auto map_ptr = current_stringmap();
	auto &map = *map_ptr;
	if (!map[str]) map[str] = str;
	return map[str];
}
