#include "map_simple.h"
#include <iostream>

SimpleMap* createSimpleMap() {
    SimpleMap* map = new SimpleMap;
    map->head = nullptr;
    map->size = 0;
    return map;
}

void destroySimpleMap(SimpleMap* map) {
    if (!map) return;
    
    MapNode* current = map->head;
    while (current != nullptr) {
        MapNode* temp = current;
        current = current->next;
        delete temp;
    }
    delete map;
}

Set* simpleMapFind(SimpleMap* map, const std::string& key) {
    if (!map) return nullptr;
    
    MapNode* current = map->head;
    while (current != nullptr) {
        if (current->key == key) {
            return current->value;
        }
        current = current->next;
    }
    return nullptr;
}

void simpleMapInsert(SimpleMap* map, const std::string& key, Set* value) {
    if (!map) return;
    
    MapNode* current = map->head;
    while (current != nullptr) {
        if (current->key == key) {
            current->value = value; 
            return;
        }
        current = current->next;
    }
    
    MapNode* newNode = new MapNode;
    newNode->key = key;
    newNode->value = value;
    newNode->next = map->head;
    map->head = newNode;
    map->size++;
}

bool simpleMapExists(SimpleMap* map, const std::string& key) {
    return simpleMapFind(map, key) != nullptr;
}

Set* simpleMapErase(SimpleMap* map, const std::string& key) {
    if (!map) return nullptr;
    
    MapNode* current = map->head;
    MapNode* prev = nullptr;
    
    while (current != nullptr) {
        if (current->key == key) {
            Set* value = current->value;
            
            if (prev == nullptr) {
                map->head = current->next;
            } else {
                prev->next = current->next;
            }
            
            delete current;
            map->size--;
            return value;
        }
        prev = current;
        current = current->next;
    }
    
    return nullptr;
}

void simpleMapGetKeys(SimpleMap* map, std::string* keys, int& count) {
    if (!map) {
        count = 0;
        return;
    }
    
    count = 0;
    MapNode* current = map->head;
    while (current != nullptr) {
        keys[count++] = current->key;
        current = current->next;
    }
}

bool simpleMapEmpty(SimpleMap* map) {
    return (!map || map->head == nullptr);
}
