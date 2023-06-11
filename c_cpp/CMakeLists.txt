CMAKE_MINIMUM_REQUIRED(VERSION 3.0)

project(sl)

include(CheckIncludeFile)
include(CheckIncludeFileCXX)
include(CheckCXXCompilerFlag)

option(ENABLE_PROFILE "should add '-pg' to cpp/c flags" ON)
option(ENABLE_OPTIMIZES "should add '-O3' to cpp/c flags" ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_BUILD_TYPE Debug)
#set(CMAKE_VERBOSE_MAKEFILE ON)
#set(CMAKE_CXX_COMPILER g++)

set(PROJECT_ROOT_PATH ${PROJECT_SOURCE_DIR})
set(PROJECT_SRC_PATH ${PROJECT_ROOT_PATH}/src)
set(PROJECT_LIB_PATH ${PROJECT_ROOT_PATH}/lib)

set(BUILD_VERSION "1")

#checks

macro(do_c_check checkFileName) 
	check_include_file("${checkFileName}" "HAS_${checkFileName}")
	if (NOT HAS_${checkFileName}) 
		message(FATAL_ERROR "Cannot find ${checkFileName}")
	endif()
endmacro()

macro(do_cxx_check checkFileName) 
	check_include_file_cxx("${checkFileName}" "HAS_${checkFileName}")
	if (NOT HAS_${checkFileName}) 
		message(FATAL_ERROR "Cannot find ${checkFileName}")
	endif()
endmacro()

do_c_check(zlib.h)
do_c_check(stdint.h)
do_c_check(ncurses.h)
do_c_check(stdio.h)
do_c_check(stdlib.h)
do_c_check(time.h)
do_c_check(string.h)

do_cxx_check(iostream)
do_cxx_check(ostream)
do_cxx_check(fstream)
do_cxx_check(string)

if (UNIX) 
	message(STATUS "The system is unix, checking for 'fcntl.h/unistd.h' ")
	check_include_file(fcntl.h HAS_FNCTL)
	check_include_file(unistd.h HAS_UNISTD)
	if ( (NOT HAS_UNISTD) OR (NOT HAS_FNCTL))
		message(STATUS "Cannot find unistd.h or fnctl.h")
	else()
		message(STATUS "Found unistd.h and fnctl.h")
	endif()
endif()

check_cxx_compiler_flag("-std=c++17" HAS_CXX17)
if (HAS_CXX17)
	message(STATUS "Compiler supports c++ 17")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
else()
	message(STATUS "Compiler doesn't support c++17, using c++ 11")
	check_cxx_compiler_flag("${CMAKE_CXX_FLAGS} -std=c++11" HAS_CXX_11)
	if(HAS_CXX_11)
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
	else()
		message(FATAL_ERROR "Error: Compiler doesn't support c++ 11 or 17")
	endif()
endif()

# modles

set(TARGET_OTHER_TOOLS other_tools)
set(TARGET_NBT nbt)

#set(THREADS_PREFIX_PTHREAD_FLAG ON)
#find_package(Threads REQUIRED)

find_package(Curses REQUIRED)
find_package(ZLIB REQUIRED)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")

if (ENABLE_OPTIMIZES)
	message(STATUS "Setting -O3 to c and cpp flags")
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
endif()

if (ENABLE_PROFILE) 
	message(STATUS "Setting -pg to cpp and c flags")
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pg")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pg")
endif()

include_directories(
${PROJECT_ROOT_PATH}/include
)

set(LIBRARY_OUTPUT_PATH ${PROJECT_ROOT_PATH}/build/lib64)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_ROOT_PATH}/build)
set(CMAKE_INSTALL_PREFIX ${PROJECT_ROOT_PATH}/build/rel)

configure_file("${PROJECT_ROOT_PATH}/include/config.h.cmake" "${PROJECT_ROOT_PATH}/include/config.h")

add_subdirectory(${PROJECT_LIB_PATH})
add_subdirectory(${PROJECT_SRC_PATH})

#install(FILES ${header_files} DESTINATION ${PROJECT_ROOT_PATH}/build/rel/include/nbt)
install(DIRECTORY include/nbt include/other_tools DESTINATION ${CMAKE_INSTALL_PREFIX}/include/sl)

install(TARGETS gameSL nbt other_tools 
	RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
	ARCHIVE DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
	LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/lib64
)
