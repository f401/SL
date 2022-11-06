aux_source_directory(${PROJECT_SRC_PATH}/screen DIR_SRC)

addCompileFiles("${DIR_SRC}")
addCompileDeps("${TARGET_OTHER_TOOLS} ncurses")
