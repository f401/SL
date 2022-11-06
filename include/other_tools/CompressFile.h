#ifndef __COMPRESS_FILE_H__
#define __COMPRESS_FILE_H__ 1

#ifdef __cplusplus
extern "C" {
#endif

int sl_compress_compressWithGzip(const char* filePath);
int sl_compress_decompressWithGzip(const char* filePath);

#ifdef __cplusplus
}
#endif

#endif
