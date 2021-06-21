#pragma once

#include <vector>

uint32_t bytesToUint32(std::vector<char>* bytes, size_t* startIndex);
int32_t bytesToInt32(std::vector<char>* bytes, size_t* startIndex);

bool findWordInBytes(std::vector<char>* bytes, size_t* i, const char* word, size_t wordLength);