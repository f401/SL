#/bin/bash

PROJECT_ROOT_PATH=`pwd`

COMPILER_OPTIONS="-Wall -O3 -g -I/opt/c/projects/sl/include"
C_COMPILER_OPTIONS=""
CXX_COMPILER_OPTIONS="-std=gnu++17"
LINK_OPTIONS="-lpthread -lncurses -lz"
SOURCES_DIRS="lib|src"
OUTPUT_NAME="main"

C_COMPILER="gcc"
CXX_COMPILER="g++"

BUILD_DIR="${PROJECT_ROOT_PATH}/compileBuild"

getBuildDirByRelPath() {
	i=$1
	strlen=${#i}
	repath=${i:1:strlen-1}
	unset result
	result="${BUILD_DIR}${repath}"
	unset strlen
	unset repath
}

echo "Checking for BUILD_DIR ${BUILD_DIR} exists"
if [ ! -d ${BUILD_DIR} ];then
	echo "BUILD_DIR dosen't exists, creating"
else
	echo "Removing ${BUILD_DIR}, and create it"
	rm -rf "${BUILD_DIR}"
fi
mkdir -p ${BUILD_DIR}

echo "Creating Build Dir Tree"
for i in `find -type d | grep --color -E '^[./]*('${SOURCES_DIRS}').*'`; do
	getBuildDirByRelPath $i
	if [ ! -d ${result} ];then
		echo "Creating ${result}"
		mkdir -p ${result}
	fi
done

echo "Compiling C Objects"
for i in `find -type f | grep --color -E '^[./]*('${SOURCES_DIRS}').*\.[c]$'`;do
	getBuildDirByRelPath $i 
	echo "Compiling C Object: $i, Output to: ${result}.o"
	$C_COMPILER -c $i -o ${result}.o ${COMPILER_OPTIONS} ${C_COMPILER_OPTIONS}
	if [ ! $? -eq 0 ];then
		echo "Error in compiling C object $i"
		exit 1
	fi
done

echo "Compiling CXX Objects"
for i in `find -type f | grep --color -E '^[./]*('${SOURCES_DIRS}').*\.(cc|cpp)$'`;do
	getBuildDirByRelPath $i 
	echo "Compiling CXX Object: $i, Output to: ${result}.o"
	$CXX_COMPILER -c $i -o ${result}.o ${COMPILER_OPTIONS} ${CXX_COMPILER_OPTIONS}
	if [ ! $? -eq 0 ];then
		echo "Error in compiling CXX object $i"
		exit 1
	fi
done

echo "Linking Exe"
Objects=`find -type f | grep --color -E '^[./].*\.[o]$'`
g++ ${LINK_OPTIONS} ${COMPILER_OPTIONS} ${Objects} -o ${OUTPUT_NAME}
if [ ! $? -eq 0 ];then
	echo "Error in compiling Link Object"
	exit 1
fi
echo "Compile And Link Finish Successful"
exit 0

#gcc -c `find $PROJECT_ROOT_PATH -name "*.c"` ${COMPILER_OPTIONS}
#echo $?
 
#g++ -c `find $PROJECT_ROOT_PATH -name "*.cpp"` ${COMPILER_OPTIONS}

#g++ `find $PROJECT_ROOT_PATH -name "*.o"` -o main -lstdc++ ${LINK_OPTIONS} ${COMPILER_OPTIONS}

#rm -f `find $PROJECT_ROOT_PATH -name "*.o"`
