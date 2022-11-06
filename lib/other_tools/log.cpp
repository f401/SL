#include <iostream>
#include <ctime>
#include <fstream>
#include <string>
#include "other_tools/log.h"
#include "other_tools/tool.h"
#include <string.h>

using namespace std;

static fstream stream;

static void write(const char* text, int type, const char* funcName = "", unsigned int line = 0, const char* file = "") {
	string logType;
	if(type == 0) {
		logType = "[INFO]";
	} else {
		logType = "[ERROR]";
	}

	time_t timer = time(NULL);
	struct tm* t_tm = localtime(&timer);
	char* nowTime = asctime(t_tm);
	nowTime[strlen(nowTime) - 1] = '\0';
	stream << "\n[" << nowTime << "], [" << file << "@" << funcName << ": " << line << "] " << logType << ": "<< text;
	stream.flush();
}

void sl_logger_openCppStream() {
	stream.open("/opt/c/projects/sl/sys-log.txt", ios::out);
	if (!stream.is_open()) {
		throw "Log Error: can't open stream";
	}
}

void sl_logger_closeCppStream() {
	stream.close();
}

void sl_logger_log_info(const char* text, const char* funcName, unsigned int line, const char* fileName, ...) {
	va_list pointer;
	va_start(pointer, fileName);
	char* txt = sl_tools_vstrformat(text, pointer);
	write(txt, 0, funcName, line, fileName);
	free(txt);
}

void sl_logger_log_error(const char* text, const char* funcName, unsigned int line, const char* fileName, ...) {
	va_list pointer;
	va_start(pointer, fileName);
	char* txt = sl_tools_vstrformat(text, pointer);
	string toChar(txt);
	write(txt, 1, funcName, line, fileName);
	free(txt);
}
