#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "other_tools/CString.h"

#define INIT_CAP 10

static void resizeAndRecap(sl_CString* string, size_t addCount, int shouldSaveOldData) {
        if (shouldSaveOldData ? 
                        string->strSize + addCount > string->cap :
                        string->strSize < addCount) {
                register size_t recap = shouldSaveOldData ? (string->cap + addCount) * 2 : addCount * 2;
                string->src = (char* ) realloc(string->src, recap); 

                memset(string->src + string->strSize , 0, recap - string->strSize);
                string->cap = recap;
        }       
}

void sl_CString_initString(sl_CString* string) {
        string->cap = INIT_CAP;
        string->strSize = 0;
        string->src = (char* ) calloc(1, INIT_CAP);
                string->src[0] = '\0';
}

void sl_CString_setString(sl_CString* string, const char* src) {
        register size_t stringLen = strlen(src);
        resizeAndRecap(string, stringLen, 0);
        string->strSize = stringLen;

        strcpy(string->src, src);
}

void sl_CString_append2String4str(sl_CString* string, const char* src) {
        register size_t stringLen = strlen(src);
        resizeAndRecap(string, stringLen, 1);
        string->strSize += stringLen;
        strcat(string->src, src);
}

void sl_CString_append2String4bytes(sl_CString* string, const char* src, size_t byteCount) {
        resizeAndRecap(string, byteCount, 1);
        memcpy(string->src + string->strSize, src, byteCount);
        string->strSize += byteCount;
}

void sl_CString_freeString(sl_CString* string) {
        free(string->src);
        string->cap = string->strSize = 0;
}

