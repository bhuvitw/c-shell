#!/bin/bash
# test.sh - Basic automated tests for csh

echo "Running csh tests..."
make clean > /dev/null 2>&1
make > /dev/null 2>&1

PASS=0
FAIL=0

run_test() {
    local name="$1"
    local input="$2"
    local expected="$3"
    
    # Run the shell with the input command, capturing output
    output=$(echo "$input; exit" | ./csh 2>&1)
    
    if echo "$output" | grep -q "$expected"; then
        echo "[PASS] $name"
        ((PASS++))
    else
        echo "[FAIL] $name"
        echo "       Expected to find: $expected"
        echo "       Got: $output"
        ((FAIL++))
    fi
}

run_test "Built-in: pwd" "pwd" "/"
run_test "External: echo" "echo test_string" "test_string"
run_test "Sequential execution" "echo a; echo b" "b"
run_test "Pipeline" "echo 'hello world' | grep world" "hello world"

# Test Output Redirection
echo "echo redirect_test > out.txt; exit" | ./csh > /dev/null 2>&1
if grep -q "redirect_test" out.txt; then
    echo "[PASS] Output redirection (>)"
    ((PASS++))
else
    echo "[FAIL] Output redirection (>)"
    ((FAIL++))
fi

# Cleanup
rm -f out.txt error.txt output.txt
echo "---"
echo "Results: $PASS passed, $FAIL failed."