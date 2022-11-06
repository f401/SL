aux_source_directory(${PROJECT_LIB_PATH}/nbt DIR_SRCS)
aux_source_directory(${PROJECT_LIB_PATH}/nbt/stream STREAM_SRCS)

set(tools_file 
	${PROJECT_LIB_PATH}/nbt/utils/ChangeTypes.c
	${PROJECT_LIB_PATH}/nbt/utils/EndiansUtils.c)

add_library(${TARGET_NBT} STATIC ${DIR_SRCS} ${tools_file} ${STREAM_SRCS})
