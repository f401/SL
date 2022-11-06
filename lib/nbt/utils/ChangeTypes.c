#include "nbt/utils/ChangeTypes.h"

union {
	float a;
	int32_t b;
} fai;

union {
	int64_t b;
	double a;
} dal;

int64_t double2long(double value) {
	dal.a = value;
	return dal.b;
}

double long2double(int64_t value) {
	dal.b = value;
	return dal.a;
}

int32_t float2int(float value) {
	fai.a = value;
	return fai.b;
}

float int2float(int32_t value) {
	fai.b = value;
	return fai.a;
}

