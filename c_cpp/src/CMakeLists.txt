aux_source_directory(${PROJECT_SRC_PATH} DIR_SRCS)
aux_source_directory(${PROJECT_SRC_PATH}/ter TER_SRCS)
aux_source_directory(${PROJECT_SRC_PATH}/screen SCREEN_SRCS)

set(COMPILE_FILES "${DIR_SRCS};${TER_SRCS};${SCREEN_SRCS}")
set(COMPILE_DEPS ${TARGET_NBT} ${TARGET_OTHER_TOOLS} ncurses)

add_executable(gameSL ${COMPILE_FILES})
target_link_libraries(gameSL PRIVATE ${COMPILE_DEPS})
target_link_options(gameSL PUBLIC "-Wl,-rpath,../lib64")

set(header_files 
	${PROJECT_ROOT_PATH}/include/nbt/DataIOStream.h
	${PROJECT_ROOT_PATH}/include/nbt/NBTIOStream.h
	${PROJECT_ROOT_PATH}/include/nbt/NBTTags.h)

#install(FILES ${header_files} DESTINATION ${PROJECT_ROOT_PATH}/build/rel/include/nbt)

#install(TARGETS main nbt other_tools 
#	RUNTIME DESTINATION ${PROJECT_ROOT_PATH}/build/rel/bin
#	ARCHIVE DESTINATION ${PROJECT_ROOT_PATH}/build/rel/lib	
#	LIBRARY DESTINATION ${PROJECT_ROOT_PATH}/build/rel/lib
#)
