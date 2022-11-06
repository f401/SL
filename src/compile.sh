#/bin/bash

PROJECT_ROOT_PATH=`pwd`
COMPILER_OPTIONS="-Wall -O3 -g -I/opt/c/projects/sl/include"
LINK_OPTIONS="-lpthread -lncurses -lz"

gcc -c `find $PROJECT_ROOT_PATH -name "*.c"` ${COMPILER_OPTIONS}
 
g++ -c `find $PROJECT_ROOT_PATH -name "*.cpp"` ${COMPILER_OPTIONS}

g++ `find $PROJECT_ROOT_PATH -name "*.o"` -o main -lstdc++ ${LINK_OPTIONS} ${COMPILER_OPTIONS}

rm -f `find $PROJECT_ROOT_PATH -name "*.o"`
