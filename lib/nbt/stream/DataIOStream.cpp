#include "nbt/stream/DataIOStream.h"
#include <iostream>
#include "nbt/utils/ChangeTypes.h"
#include "nbt/utils/EndiansUtils.h"

using namespace dataIO;

void DataOutputStream::writeByte(uint8_t value) {
	this->stream->put(value);
}

void DataOutputStream::writeShort(int16_t value) {
	int16_t val = this->mLittleEndian ? reverseBytes16(value) : value;
	stream->write((char*)&val, 2);
}

void DataOutputStream::writeInt(int32_t value) {
	int32_t val = this->mLittleEndian ? reverseBytes32(value) : value;
	stream->write((char*)&val, 4);
}

void DataOutputStream::writeLong(int64_t value) {
	int64_t val = this->mLittleEndian ? reverseBytes64(value) : value;
	stream->write((char*)&val, 8);
}

void DataOutputStream::writeString(std::string value) {
	uint16_t strLen = this->mLittleEndian ? reverseBytesu16((uint16_t)value.length()) : value.length();
	stream->write((char*)&strLen, 2);
	if(strLen != 0)
		*(stream) << value;
}

void DataOutputStream::writeFloat(float value) {
	int32_t rval = float2int(value);
	rval = this->mLittleEndian ? reverseBytes32(rval) : rval;
	stream->write((char*)&rval, 4);
}

void DataOutputStream::writeDouble(double value) {
	int64_t rval = double2long(value);
	rval = this->mLittleEndian ? reverseBytes64(rval) : rval;
	stream->write((char*)&rval, 8);
}

uint8_t DataInputStream::readByte() {
	uint8_t ret;
	this->stream->get((char&)ret);
	return ret;
}

int16_t DataInputStream::readShort() {
	int16_t result;
	this->stream->read((char*)&result, 2);
	return this->mLittleEndian ? reverseBytes16(result) : result;
}

int32_t DataInputStream::readInt() {
	int32_t result;
	this->stream->read((char*)&result, 4);
	return this->mLittleEndian ? reverseBytes32(result) : result;
}

int64_t DataInputStream::readLong() {
	int64_t result;
	this->stream->read((char*)&result, 8);
	return this->mLittleEndian ? reverseBytes64(result) : result;
}

float DataInputStream::readFloat() {
	int32_t re;
	this->stream->read((char*)&re, 4);
	return int2float(this->mLittleEndian ? reverseBytes32(re) : re);
}

double DataInputStream::readDouble() {
	int64_t result;
	this->stream->read((char*)&result, 8);
	return long2double(this->mLittleEndian ? reverseBytes64(result) : result);
}

std::string DataInputStream::readString() {
	uint16_t strlen;
	this->stream->read((char*)&strlen, 2);

	if (strlen == 0) return std::string("");

	std::string ret;
	strlen = mLittleEndian ? reverseBytesu16(strlen) : strlen;
	ret.resize(strlen);

	char nb[strlen + 1];
	this->stream->read(nb, strlen);
	nb[strlen] = '\0';
	ret = nb;
	return ret;
}
