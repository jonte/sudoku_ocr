#!/bin/bash

function do_test {
    if ! echo "$1" | grep "$2" > /dev/null;
    then
        echo "Got: $1"
        echo "Expected: $2"
        exit 1
    fi
}

RES=`./sudoku_ocr -w 9 -h 9 -a 18 -b 28 -c 300 -d 300 -f testfiles/sudoku2.tif -t`
EXPECTED=".7...6... 9......41 ..8..9.5. .9...7..2 ..3...8.. 4..8...1. .8.3..9.. 16......7 ...5...8."
do_test "$RES" "$EXPECTED"
