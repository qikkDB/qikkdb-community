//
// Created by Martin Staňo on 2019-01-15.
//

#ifndef DROPDBASE_INSTAREA_MEMORYSTREAM_H
#define DROPDBASE_INSTAREA_MEMORYSTREAM_H

#include <vector>
#include <string>
#include <iostream>

class MemoryStream
{

private:
    std::vector<char> buffer;

public:
    MemoryStream(){
        buffer.reserve(8192);
    }

    template<typename T>
    void insert(T value)
    {
        char *valuePtr = reinterpret_cast<char *>(&value);
        std::copy(valuePtr, valuePtr + sizeof(T), std::back_inserter(buffer));
    }

    template<typename T>
    T read()
    {
        T value = *reinterpret_cast<T *>(buffer.data());
        buffer.erase(buffer.begin(), buffer.begin() + sizeof(T));
        return value;
    }

};

template<>
void MemoryStream::insert(const std::string &value);

template<>
std::string MemoryStream::read();
#endif //DROPDBASE_INSTAREA_MEMORYSTREAM_H