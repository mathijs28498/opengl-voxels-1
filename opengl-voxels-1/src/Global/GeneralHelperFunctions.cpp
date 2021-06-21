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