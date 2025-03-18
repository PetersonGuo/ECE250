#!/bin/bash
# run_tests.sh
# This script runs each test file from the tests/ directory using a.out.
# It compares the program's output to the expected output in validate/,
# and only if there is a difference does it save the output and a side-by-side diff
# in the output/ directory.

EXEC="./a.out"
TEST_DIR="tests"
VALIDATE_DIR="validate"
OUTPUT_DIR="output"

# Create the output directory if it doesn't exist.
mkdir -p "$OUTPUT_DIR"

rm -f "$OUTPUT_DIR"/*

# Check if the executable exists and is executable.
if [ ! -x "$EXEC" ]; then
    echo "Error: Executable a.out not found or not executable."
    exit 1
fi

pass_count=0
fail_count=0

# Iterate over each test file in the tests/ directory.
for test_file in "$TEST_DIR"/*; do
    base=$(basename "$test_file")
    
    # Extract the test number from the file name (expects test_input#.in).
    # For example, test_input1.in will extract "1".
    test_num=$(echo "$base" | sed -E 's/^test_input([0-9]+)\.in/\1/')
    expected_file="$VALIDATE_DIR/${test_num}.out"

    if [ ! -f "$expected_file" ]; then
        echo "Expected output file for '$base' not found in $VALIDATE_DIR. Skipping test."
        continue
    fi

    echo "Running test: $base (Test number: $test_num)"

    # Create a temporary file to store the output.
    temp_output=$(mktemp)

    # Run the executable using the test file as input and save output.
    $EXEC < "$test_file" > "$temp_output"

    # Check if the output differs from the expected output.
    if ! diff -q "$temp_output" "$expected_file" > /dev/null; then
        # If there is a diff, save the output and the side-by-side diff.
        cp "$temp_output" "$OUTPUT_DIR/${test_num}.out"
        diff -y "$temp_output" "$expected_file" > "$OUTPUT_DIR/${test_num}_diff.txt"
        echo "Test $base FAILED. See $OUTPUT_DIR/${test_num}_diff.txt for details."
        fail_count=$((fail_count + 1))
    else
        echo "Test $base PASSED."
        pass_count=$((pass_count + 1))
    fi

    # Remove the temporary output file.
    rm "$temp_output"

    echo "---------------------------------------"
done

echo "Total Tests Passed: $pass_count"
echo "Total Tests Failed: $fail_count"
