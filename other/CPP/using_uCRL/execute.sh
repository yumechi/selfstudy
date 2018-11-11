#!/bin/bash

FILENAME="test_curl"
LIBFILE="picojson.h"

if [ ! -e ${LIBFILE} ]; then
    echo "download ${LIBFILE}"
    curl https://raw.githubusercontent.com/kazuho/picojson/master/picojson.h > ${LIBFILE}
fi

g++ -lcurl ${FILENAME}.cpp -o ${FILENAME}
./${FILENAME} > result.txt
cat result.txt | python2 -c 'from sys import stdin; print stdin.readline().decode("unicode-escape")'

