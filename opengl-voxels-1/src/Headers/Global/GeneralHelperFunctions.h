#pragma once

#include <vector>
#include <array>
#define PRINT(toPrint) std::cout << toPrint << '\n'
#define PRINT_VEC3(v) PRINT(v.x << " - " << v.y << " - " << v.z)
#define PRINT_ARR3(a) PRINT(+a[0] << " - " << +a[1] << " - " << +a[2])
#define PRINT_ARR3_(a) PRINT(a[0] << " - " << a[1] << " - " << a[2])

uint32_t bytesToUint32(std::vector<char>* bytes, size_t* startIndex);
int32_t bytesToInt32(std::vector<char>* bytes, size_t* startIndex);

bool findWordInBytes(std::vector<char>* bytes, size_t* i, const char* word, size_t wordLength);

uint32_t bytesToInt(std::array<uint8_t, 4> bytes);
uint32_t bytesToInt(uint8_t b0, uint8_t b1, uint8_t b2, uint8_t b3);
uint8_t* positionFromInt(uint32_t position);

std::array<uint8_t, 3> intToBytes3(uint32_t intArg);
std::array<uint8_t, 4> intToBytes4(uint32_t intArg);
