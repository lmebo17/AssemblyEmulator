#!/bin/bash

# Compile the emulator
gcc -w emulator.c -o emulator

# Test function
run_test() {
    local test_file=$1
    local expected_output=$2

    ./emulator "$test_file" > output.txt

    if diff output.txt "$expected_output"; then
        echo "$test_file: PASSED"
    else
        echo "$test_file: FAILED"
        diff output.txt "$expected_output"
    fi

    rm output.txt
}

# Test cases
for ((i=1; i <= 11; i++)); do
    run_test "test/test${i}.asm" "cmp/expected${i}.txt"
done