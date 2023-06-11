#ifndef __ENDIANS_UTILS_H__
#define __ENDIANS_UTILS_H__

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

	int16_t reverseBytes16(int16_t value);
	int32_t reverseBytes32(int32_t value);
	int64_t reverseBytes64(int64_t value);
	uint16_t reverseBytesu16(uint16_t value);

#ifdef __cplusplus
}
#endif

#endif
