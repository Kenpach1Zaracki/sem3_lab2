#ifndef ARRAY_H
#define ARRAY_H

#include <string>

struct Array {
    std::string name;
    std::string* data;
    int capacity;
    int size;
};

Array* create_array(const std::string& name);
void array_push_back(Array* arr, const std::string& value);
void array_insert(Array* arr, int index, const std::string& value);
std::string array_get(const Array* arr, int index);
void array_set(Array* arr, int index, const std::string& value);
void array_delete(Array* arr, int index);
int array_length(const Array* arr);
std::string array_read(const Array* arr);
void destroy_array(Array* arr);

#endif
