#include <zlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "other_tools/CompressFile.h"
#include "other_tools/CString.h"

#include <config.h>

#if HAS_FNCTL && HAS_UNISTD
#include <fcntl.h>
#include <unistd.h>
#endif

static void write2file(const char* path, const char* src, size_t inputDataSize) {
	FILE* file = fopen(path, "w");
	for (unsigned int i = 0; i < inputDataSize; i++) {
		fputc(src[i], file);
	}
	fclose(file);
}

static unsigned char* readFromFile(const char* filePath, size_t* fileSize) {
	FILE* file = fopen(filePath, "r");
	size_t readCount = 0;
	int tmp;

#if HAS_FNCTL && HAS_UNISTD
	int fd = open(filePath, O_RDWR);
	size_t size = lseek(fd, 0, SEEK_END);
	unsigned char* ret = (unsigned char* ) calloc(1, size);
#else
	unsigned char* ret = (unsigned char* ) calloc(1, 1);
#endif
	while((tmp = fgetc(file)) != EOF) {
		ret[readCount] = tmp;
		readCount += 1;
#if HAS_FNCTL && HAS_UNISTD
#else
		ret = (unsigned char* ) realloc(ret, readCount + 1);
#endif
	}
	
	*fileSize = readCount;
	fclose(file);

	return ret;
}

int sl_compress_compressWithGzip(const char* filePath) {
	size_t fileSize;
	unsigned char* compressData = readFromFile(filePath, &fileSize);
	unsigned char bufferData[fileSize];
	sl_CString outputData;
	sl_CString_initString(&outputData);

	z_stream stream;
	printf("start to compressData\n");

	stream.zalloc = Z_NULL;
	stream.zfree = Z_NULL;
	stream.opaque = Z_NULL;
	
	int resultCode = deflateInit2(&stream, Z_DEFAULT_COMPRESSION, Z_DEFLATED, MAX_WBITS | 16, 8, Z_DEFAULT_STRATEGY), flush = Z_NO_FLUSH;

	if (resultCode != Z_OK) {
		printf("deflateInit2 error with code %d\n", resultCode);
		free(compressData);
		return -1;
	}
	stream.avail_in = fileSize;
	stream.next_in = compressData;
	do {
		stream.next_out = bufferData;
		stream.avail_out = fileSize;
		resultCode = deflate(&stream, flush);
		sl_CString_append2String4bytes(&outputData, (char* )bufferData, fileSize - stream.avail_out);
		if (stream.avail_out != 0) {
			flush = Z_FINISH;
		}

	} while(resultCode != Z_STREAM_END);

	resultCode = deflateEnd(&stream);

	if (resultCode != Z_OK) {
		printf("deflateEnd error with code %d\n", resultCode);
		free(compressData);
		return -1;
	}

	free(compressData);
	printf("writing data\n");
	write2file(filePath, outputData.src, outputData.strSize);
	printf("CompressFile finish\n");
	sl_CString_freeString(&outputData);
	return 0;
}

int sl_compress_decompressWithGzip(const char* filePath) {
	size_t decompressDataSize;
	unsigned char* decompressData = readFromFile(filePath, &decompressDataSize);
	unsigned char bufferData[decompressDataSize];
	sl_CString outputData;
	sl_CString_initString(&outputData);
	z_stream stream;

	stream.zfree = Z_NULL;
	stream.zalloc = Z_NULL;
	stream.opaque = Z_NULL;
	stream.avail_in = 0;
	stream.next_in = Z_NULL;

	int resultCode = inflateInit2(&stream, MAX_WBITS | 16), flush = Z_NO_FLUSH;

	if (resultCode != Z_OK) {
		printf("inflateInit2 error with code %d\n", resultCode);
		free(decompressData);
		return -1;
	}
	stream.avail_in = decompressDataSize;
	stream.next_in = decompressData;

	do {
		stream.avail_out = decompressDataSize;
		stream.next_out = bufferData;

		resultCode = inflate(&stream, flush);
		
		switch (resultCode) {
			case Z_NEED_DICT:
			case Z_DATA_ERROR:
			case Z_STREAM_ERROR:
				free(decompressData);
				inflateEnd(&stream);
				printf("inflate FAIL! flush: %d, code: %d\n", flush, resultCode);
				return -1;
		}

		if (stream.avail_out != 0) {
			flush = Z_FINISH;
		}
		sl_CString_append2String4bytes(&outputData, (char* )bufferData, decompressDataSize - stream.avail_out);

	} while(resultCode != Z_STREAM_END);

	resultCode = inflateEnd(&stream);

	if (resultCode != Z_OK) {
		printf("inflateEnd error with code %d\n", resultCode);
		free(decompressData);
		return -1;
	}
	write2file(filePath, outputData.src, outputData.strSize);
	sl_CString_freeString(&outputData);
	free(decompressData);
	return 0;
}
