#ifndef __FILE_BRIDGE_H__
#define __FILE_BRIDGE_H__ 1

#ifdef __cplusplus
extern "C" {
#endif

	void sl_FileBridge_save(int saveStatus);//save all data	
        void sl_FileBridge_loadArgsFromFile(const char* path);
	void sl_FileBridge_loadDataFromFile();


#ifdef __cplusplus
}
#endif

#endif
