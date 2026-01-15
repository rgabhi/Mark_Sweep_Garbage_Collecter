#!/bin/bash

# Colors for pretty output
GREEN='\033[0;32m'
RED='\033[0;31m'
NC='\033[0m' # No Color

echo "========================================================================================"
echo "                         GARBAGE COLLECTOR PERFORMANCE REPORT                           "
echo "========================================================================================"
printf "%-25s | %-10s | %-15s | %-15s\n" "TEST CASE" "RESULT" "TIME (s)" "FREED (objs)"
echo "--------------------------+------------+-----------------+----------------"

run_test() {
    test_name=$1
    exec_name=$2
    
    # Run the test and capture output
    output=$(./$exec_name 2>&1)
    
    # Parse Metrics from output
    # Looks for "Execution Time:     0.000000 seconds"
    time_val=$(echo "$output" | grep "Execution Time:" | awk '{print $3}')
    
    # Looks for "Total Objects Freed: 123"
    freed_val=$(echo "$output" | grep "Total Objects Freed:" | awk '{print $4}')

    # Default to 0/N/A if parsing fails
    if [ -z "$time_val" ]; then time_val="N/A"; fi
    if [ -z "$freed_val" ]; then freed_val="0"; fi

    # Check Pass/Fail
    if echo "$output" | grep -q "PASSED"; then
        printf "%-25s | ${GREEN}%-10s${NC} | %-15s | %-15s\n" "$test_name" "PASSED" "$time_val" "$freed_val"
    else
        printf "%-25s | ${RED}%-10s${NC} | %-15s | %-15s\n" "$test_name" "FAILED" "$time_val" "$freed_val"
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

echo "========================================================================================"