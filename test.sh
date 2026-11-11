#!/bin/bash
# test.sh - Basic automated tests for csh

echo "Running csh tests..."
make clean > /dev/null 2>&1
make > /dev/null 2>&1

PASS=0
FAIL=0

CSH="$(pwd)/csh"
TMP=$(mktemp -d)
trap 'rm -rf "$TMP"' EXIT
cd "$TMP"

run_test() {
    local name="$1"
    local input="$2"
    local expected="$3"
    
    # Run the shell with the input command, capturing output
    output=$(printf '%s\nexit\n' "$input" | timeout 5 "$CSH" 2>&1)
    status=$?

    if [ "$status" -eq 124 ]; then
        echo "[FAIL] $name (HUNG)"
        FAIL=$((FAIL+1))
    elif echo "$output" | grep -qF -- "$expected"; then
        echo "[PASS] $name"
        PASS=$((PASS+1))
    else 
        echo "[FAIL] $name"
        echo "       Expected to find: $expected"
        echo "       Got: $output"
        FAIL=$((FAIL+1))
    fi
}

run_test "Built-in: pwd" "pwd" "/"
run_test "External: echo" "echo test_string" "test_string"
run_test "Sequential execution" "echo a; echo b" "b"
run_test "Pipeline" "echo 'hello world' | grep world" "hello world"
run_test "No-space pipe" 'echo hello|tr a-z A-Z' "HELLO"
run_test "No-space redirect" 'echo zzz>f.txt; wc -c < f.txt' "4"
run_test "No-space background" 'sleep 1&' "started"
run_test "Operators in quotes stay literal" 'echo "a|b"' "a|b"
run_test "stderr redirect" 'cat nosuch 2> e.txt; grep -c "No such" e.txt' "1"
run_test "2 inside a word is not an operator" 'echo 12> f.txt; cat f.txt' "12"

# Test Output Redirection
printf 'echo redirect_test > out.txt\nexit\n' | "$CSH" > /dev/null 2>&1
if grep -q "redirect_test" out.txt; then
    echo "[PASS] Output redirection (>)"
    PASS=$((PASS+1))
else
    echo "[FAIL] Output redirection (>)"
    FAIL=$((FAIL+1))
fi

# Cleanup
echo "---"
echo "Results: $PASS passed, $FAIL failed."
[ "$FAIL" -eq 0 ]