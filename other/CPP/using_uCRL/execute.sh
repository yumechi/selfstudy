#!/bin/bash

FILENAME="test_curl"
LIBFILE="picojson.h"

if [ ! -e ${LIBFILE} ]; then
    echo "download ${LIBFILE}"
    curl https://raw.githubusercontent.com/kazuho/picojson/master/picojson.h > ${LIBFILE}
fi

g++ ${FILENAME}.cpp -std=c++11 -o ${FILENAME} -lcurl
./${FILENAME} 

