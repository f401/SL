#/bin/bash

PROJECT_ROOT_PATH=`pwd`

clang -c `find $PROJECT_ROOT_PATH -name "*.c"` -Wall -O3 -g -I/opt/c/projects/sl/include 

clang++ -c `find $PROJECT_ROOT_PATH -name "*.cpp"` -Wall -O3 -g -I/opt/c/projects/sl/include 

clang++ `find $PROJECT_ROOT_PATH -name "*.o"` -lpthread -o main -lstdc++ -lncurses -lz -O3 -g

rm -f `find $PROJECT_ROOT_PATH -name "*.o"`
