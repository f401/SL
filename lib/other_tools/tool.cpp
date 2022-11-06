#include "other_tools/tool.h"
#include <iostream>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <time.h>

#define USE_STD_FORMAT

using namespace std;

extern "C" {

char *sl_tools_getsign(unsigned int count, const char need) {
  char *va = (char *)malloc(sizeof(char) * (count + 1));
  for (unsigned int i = 0; i < count; i++) {
    va[i] = need;
  }
  va[count] = '\0';
  return va;
}

int sl_tools_createRandomNumber(int min, int max) {
  static bool first = true;
  if (first) {
    srand((unsigned)time(NULL));
    first = false;
  }
  return rand() % (max - min) + min;
}

int sl_tools_checkAllIsNumber(const char *str) {
  return strspn(str, "0123456789 ") == strlen(str);
}

char *sl_tools_substring(const char *str, unsigned int start, unsigned int end) {
  char *dest = (char *)malloc(end - start + 2);
  strncpy(dest, str + start, end - start + 1);
  dest[end - start] = '\0';
  return dest;
}

int sl_tools_getNumberBit(int number) {
  int count = 0;
  while (number != 0) {
    number = number / 10;
    count++;
  }
  return count;
}

int sl_tools_strEquals(const char *a, const char *b) { return strcmp(a, b) == 0; }

char *sl_tools_vstrformat(const char *text, va_list pointer) {

#ifndef USE_STD_FORMAT
  string cppText = text;

  int strLen = cppText.size();
  bool isHasTemp = false;
  string returnStr = "";

  for (int i = 0; i < strLen; i++) {
    char sign = cppText.at(i);
    if (isHasTemp) {
      isHasTemp = false;
      switch (sign) {
      case 'd': {
        int s = va_arg(pointer, int);
        returnStr.append((to_string(s)));
        break;
      }
      case 's': {
        string str = va_arg(pointer, char *);
        returnStr.append(str);
        break;
      }
      case 'l': {
        long s = va_arg(pointer, long);
        returnStr.append(to_string(s));
        break;
      }
      case 'f': {
        double f = va_arg(pointer, double);
        returnStr.append(to_string(f));
        break;
      }
      case 'c': {
        char c = (char)va_arg(pointer, int);
        returnStr.push_back(c);
        break;
      }
      }
      continue;
    }
    if (sign == '%') {
      isHasTemp = true;
    } else {
      returnStr.push_back(sign);
    }
  }

  va_end(pointer);

  char *returnChar = (char *)calloc(sizeof(char), returnStr.size() + 1);
  strcpy(returnChar, returnStr.c_str());

  return returnChar;
#else
  char *buffer = (char *)calloc(2, strlen(text));
  size_t len = vsprintf(buffer, text, pointer);
  buffer = (char *)realloc(buffer, len);
  return buffer;
#endif
}

char *sl_tools_strFormat(const char *text, ...) {
  va_list pointer;
  va_start(pointer, text);
  return sl_tools_vstrformat(text, pointer);
}
}
