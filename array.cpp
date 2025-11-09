#include "array.h"
#include <stdexcept>

Array* create_array(const std::string& name) {
    Array* arr = new Array;
    arr->name = name;
    arr->capacity = 10;
    arr->data = new std::string[arr->capacity];
    arr->size = 0;
    return arr;
}

void array_push_back(Array* arr, const std::string& value) {
    if (arr->size >= arr->capacity) {
        int new_capacity = arr->capacity * 2;
        std::string* new_data = new std::string[new_capacity];
        for (int i = 0; i < arr->size; ++i) {
            new_data[i] = arr->data[i];
        }
        delete[] arr->data;
        arr->data = new_data;
        arr->capacity = new_capacity;
    }
    arr->data[arr->size++] = value;
}

void array_insert(Array* arr, int index, const std::string& value) {
    if (index < 0 || index > arr->size) throw std::out_of_range("Index out of range");
    
    if (index == arr->size) {
        array_push_back(arr, value);
        return;
    }
    
    if (arr->size >= arr->capacity) {
        int new_capacity = arr->capacity * 2;
        std::string* new_data = new std::string[new_capacity];
        for (int i = 0; i < arr->size; ++i) {
            new_data[i] = arr->data[i];
        }
        delete[] arr->data;
        arr->data = new_data;
        arr->capacity = new_capacity;
    }
    
    for (int i = arr->size; i > index; --i) {
        arr->data[i] = arr->data[i - 1];
    }
    arr->data[index] = value;
    arr->size++;
}

std::string array_get(const Array* arr, int index) {
    if (index < 0 || index >= arr->size) throw std::out_of_range("Index out of range");
    return arr->data[index];
}

void array_set(Array* arr, int index, const std::string& value) {
    if (index < 0 || index >= arr->size) throw std::out_of_range("Index out of range");
    arr->data[index] = value;
}

void array_delete(Array* arr, int index) {
    if (index < 0 || index >= arr->size) throw std::out_of_range("Index out of range");
    for (int i = index; i < arr->size - 1; ++i) {
        arr->data[i] = arr->data[i + 1];
    }
    arr->size--;
}

int array_length(const Array* arr) {
    return arr->size;
}

std::string array_read(const Array* arr) {
    std::string result;
    for (int i = 0; i < arr->size; ++i) {
        if (!result.empty()) result += " ";
        result += arr->data[i];
    }
    return result;
}

void destroy_array(Array* arr) {
    delete[] arr->data;
    delete arr;
}
