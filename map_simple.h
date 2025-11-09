#ifndef MAP_SIMPLE_H
#define MAP_SIMPLE_H
#include <string>
#include "set.h"

struct MapNode {
    std::string key;
    Set* value;
    MapNode* next;
};

struct SimpleMap {
    MapNode* head;
    int size;
};

SimpleMap* createSimpleMap();

void destroySimpleMap(SimpleMap* map);

Set* simpleMapFind(SimpleMap* map, const std::string& key);

void simpleMapInsert(SimpleMap* map, const std::string& key, Set* value);

bool simpleMapExists(SimpleMap* map, const std::string& key);

Set* simpleMapErase(SimpleMap* map, const std::string& key);

void simpleMapGetKeys(SimpleMap* map, std::string* keys, int& count);

bool simpleMapEmpty(SimpleMap* map);

#endif
