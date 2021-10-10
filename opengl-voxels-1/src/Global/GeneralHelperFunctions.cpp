#include "../Headers/Global/GeneralHelperFunctions.h"

uint32_t bytesToUint32(std::vector<char>* bytes, size_t* bytePointer) {
	uint32_t uint32 = 0x0000;
	for (size_t i = 0; i < 4; i++) {
		uint32 = uint32 | ((uint8_t)(*bytes)[*bytePointer] << 8 * (*bytePointer));
		(*bytePointer)++;
	}
	return uint32;
}

// TODO: Test this method if it works
int32_t bytesToInt32(std::vector<char>* bytes, size_t* bytePointer) {
	int32_t int32 = 0x0000;
	for (size_t i = 0; i < 4; i++) {
		int32 = int32 | ((uint8_t)(*bytes)[*bytePointer] << 8 * (*bytePointer));
		bytePointer++;
	}
	return int32;
}

bool findWordInBytes(std::vector<char>* bytes, size_t* bytePointer, const char* word, size_t wordLength) {
	while (*bytePointer < bytes->size() - wordLength) {
		if ((*bytes)[*bytePointer] == word[0]) {
			bool foundWord = true;
			for (size_t j = *bytePointer + 1; j < *bytePointer + wordLength; j++) {
				if ((*bytes)[j] != word[j - *bytePointer]) {
					foundWord = false;
					break;
				}
			}
			if (foundWord) {
				(*bytePointer) += wordLength;
				return true;
			}

		}
		(*bytePointer)++;
	}
	return false;
}

uint32_t bytesToInt(uint8_t b0, uint8_t b1, uint8_t b2, uint8_t b3) {
	return b0 + (b1 << 8) + (b2 << 16) + (b3 << 24);
}

uint8_t* positionFromInt(uint32_t position) {
	uint8_t res[] = {
		position & 0xff,
		(position >> 8) & 0xff,
		(position >> 16) & 0xff,
	};

	return res;
}

std::vector<uint8_t> intToBytes(uint32_t intArg) {
	uint8_t b0 = intArg & 0xFF;
	uint8_t b1 = (intArg >> 8) & 0xFF;
	uint8_t b2 = (intArg >> 16) & 0xFF;
	uint8_t b3 = (intArg >> 24) & 0xFF;

	return { b0, b1, b2, b3 };
}