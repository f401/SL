aux_source_directory(${PROJECT_LIB_PATH}/other_tools DIR_SRCS)

add_library(${TARGET_OTHER_TOOLS} SHARED ${DIR_SRCS})
target_link_libraries(${TARGET_OTHER_TOOLS} PRIVATE ZLIB::ZLIB)

