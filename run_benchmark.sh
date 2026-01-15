#!/bin/bash

# Colors for pretty output
GREEN='\033[0;32m'
RED='\033[0;31m'
NC='\033[0m' # No Color

echo "=============================================================================="
echo "                   GARBAGE COLLECTOR PERFORMANCE REPORT                       "
echo "=============================================================================="
printf "%-25s | %-10s | %-15s\n" "TEST CASE" "RESULT" "TIME (s)"
echo "--------------------------+------------+----------------"

run_test() {
    test_name=$1
    exec_name=$2
    
    # 1. Capture Start Time
    start_time=$(date +%s.%N)
    
    # 2. Run the test and capture output
    # We redirect stderr to stdout to catch all messages
    output=$(./$exec_name 2>&1)
    
    # 3. Capture End Time
    end_time=$(date +%s.%N)
    
    # 4. Calculate Duration
    duration=$(echo "$end_time - $start_time" | bc)

    # 5. Check for "PASSED" in the output
    if echo "$output" | grep -q "PASSED"; then
        printf "%-25s | ${GREEN}%-10s${NC} | %-15s\n" "$test_name" "PASSED" "$duration"
    else
        printf "%-25s | ${RED}%-10s${NC} | %-15s\n" "$test_name" "FAILED" "$duration"
        # Optional: Print failure details
        # echo "$output" 
    fi
}

# Run all the compiled test files
run_test "Basic Reachability" "test_1"
run_test "Unreachable Objects" "test_unreachable"
run_test "Transitive Reachability" "test_transitive"
run_test "Cyclic References" "test_cycles"
run_test "Deep Object Graph" "test_deep"
run_test "Closure Capture" "test_closure"
run_test "Stress Allocation" "test_stress"

echo "=============================================================================="