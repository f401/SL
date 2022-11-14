#ifndef __LOG_H__
#define __LOG_H__ 1

#ifdef __cplusplus
extern "C" {
#endif

#define sl_logger_info(text,...) (sl_logger_log_info(text, __func__, __LINE__, __FILE__, ##__VA_ARGS__)) 
#define sl_logger_error(text,...) (sl_logger_log_error(text, __func__, __LINE__, __FILE__, ##__VA_ARGS__)) 

void sl_logger_log_info(const char* text, const char* funcName, unsigned int line, const char* fileName, ...);
void sl_logger_log_error(const char* text, const char* funcName, unsigned int line, const char* fileName, ...);

void sl_logger_openCppStream();
void sl_logger_closeCppStream();

#ifdef __cplusplus
}
#endif

#endif
