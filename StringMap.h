#ifndef StringMap_h
#define StringMap_h

#include "Global.h"

extern vector<map<string, const char *>*> paged_stringmaps;
extern int current_app_page;

map<string, const char *> *current_stringmap();
void insert_string(const char *str);
const char *get_string(const char *str);

#endif StringMap_h