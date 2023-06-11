#include "nbt/utils/EndiansUtils.h"
#include <stdint.h>
#include <stdbool.h>

inline static bool isLittleEndian() {
	int ch = 1;
	char cha = (char)ch;
	return cha == 1;
}

int16_t reverseBytes16(int16_t value) {
	if (!isLittleEndian()) {
		int16_t rval;
		int8_t* data = (int8_t*)&rval;
		data[0] = value >> 8;
		data[1] = value >> 0;
		return rval;
	}
	return value;
}

int32_t reverseBytes32(int32_t value) {
	if(!isLittleEndian()) {
		int32_t rval;
		int8_t* data = (int8_t*)&rval;
		data[0] = value >> 24;
		data[1] = value >> 16;
		data[2] = value >> 8;
		data[3] = value >> 0;
		return rval;
	}
	return value;
}

int64_t reverseBytes64(int64_t value) {
	if (!isLittleEndian()) {
		int64_t rval;
		int8_t* data = (int8_t *)&rval;
		data[0] = value >> 56;
		data[1] = value >> 48;
		data[2] = value >> 40;
		data[3] = value >> 32;
		data[4] = value >> 24;
		data[5] = value >> 16;
		data[6] = value >> 8;
		data[7] = value >> 0;
		return rval;
	}
	return value;
}

uint16_t reverseBytesu16(uint16_t value) {
	if (!isLittleEndian()) {
		uint16_t rval;
		int8_t* data = (int8_t*)&rval;
		data[0] = value >> 8;
		data[1] = value >> 0;
		return rval;
	}
	return value;
}

