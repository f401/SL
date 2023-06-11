#!/bin/bash

line=0

for i in `find . -type f | grep -E --color '^[./]*(src|lib|include).*\.(cpp|cc|c|h)$'`;do
	echo "Doing $i"
	((line+=`wc -l $i | awk '{print $1}'`))
done

#for i in `find . -name "*.h"`;do ((line+=`cat -n $i | tail -n 1 | awk '{print $1}'`)) ; done
#for i in `find . -name "*.cc"`;do ((line+=`cat -n $i | tail -n 1 | awk '{print $1}'`)) ; done
#for i in `find . -name "*.cpp"`;do ((line+=`cat -n $i | tail -n 1 | awk '{print $1}'`)) ; done
#for i in `find . -name "*.c"`;do ((line+=`cat -n $i | tail -n 1 | awk '{print $1}'`)) ; done

echo "lines: $line"
