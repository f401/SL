#ifndef __C_STRING_H__
#define __C_STRING_H__

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
        unsigned long cap;
        unsigned long strSize;
        char* src;
} sl_CString; 

void sl_CString_initString(sl_CString* string);

void sl_CString_setString(sl_CString* string, const char* src);

void sl_CString_append2String4str(sl_CString* string, const char* src);

void sl_CString_append2String4bytes(sl_CString* string, const char* src, unsigned long int byteCount);

void sl_CString_freeString(sl_CString* string);


#ifdef __cplusplus
}
#endif

#endif
