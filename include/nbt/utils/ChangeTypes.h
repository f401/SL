#ifndef __CHANGE_TYPES_H__
#define __CHANGE_TYPES_H__ 1

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

int64_t double2long(double value);
double long2double(int64_t value); 

int32_t float2int(float value);
float int2float(int32_t value);

#ifdef __cplusplus
}
#endif

#endif
