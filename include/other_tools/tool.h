#ifndef __TOOL_H__
#define __TOOL_H__ 1

#include <stdarg.h>

#ifdef __cplusplus
extern "C" {
#endif
// remember to free!
char *sl_tools_getsign(unsigned int count, const char need);

int sl_tools_createRandomNumber(int min, int max);

int sl_tools_checkAllIsNumber(const char *str);

char *sl_tools_substring(const char *str, unsigned int start, unsigned int end);

int sl_tools_getNumberBit(int num);

int sl_tools_strEquals(const char *a, const char *b);

char *sl_tools_strFormat(const char *text, ...);

char *sl_tools_vstrformat(const char *text, va_list pointer);
#ifdef __cplusplus
}
#endif
#endif
