#!/bin/bash

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# Counters
TESTS_RUN=0
TESTS_PASSED=0
TESTS_FAILED=0

# Path to minishell (adjust if needed)
MINISHELL="./minishell"

# Function to clean minishell output
clean_minishell_output() {
    sed -E '
        # Remove ANSI escape sequences
        s/\x1b\[[0-9;]*[a-zA-Z]//g
        s/\x1b\([0-9;]*[a-zA-Z]//g

        # Remove entire lines that contain the prompt (including the command)
        /\[[0-9]+\] minishell\$/d

        # Remove exit command line
        /^exit$/d
    '
}

# Function to run a single test
run_test() {
    local test_name="$1"
    local command="$2"
    local check_exit_status="${3:-true}"  # Default: check exit status

    TESTS_RUN=$((TESTS_RUN + 1))

    echo -e "\n${YELLOW}Test #$TESTS_RUN: $test_name${NC}"
    echo "Command: $command"

    # Run in bash
    local bash_output=$(bash -c "$command" 2>&1)
    local bash_exit=$?

    # Run in minishell (send command + exit)
    local minishell_output=$(echo -e "$command\nexit" | $MINISHELL 2>&1 | clean_minishell_output)
    local minishell_exit=$?

    # Compare outputs
    local output_match=false
    local exit_match=false

    if [ "$bash_output" = "$minishell_output" ]; then
        output_match=true
    fi

    if [ "$check_exit_status" = "true" ]; then
        if [ $bash_exit -eq $minishell_exit ]; then
            exit_match=true
        fi
    else
        exit_match=true  # Don't check exit status
    fi

    # Display results
    if [ "$output_match" = true ] && [ "$exit_match" = true ]; then
        echo -e "${GREEN}✓ PASSED${NC}"
        TESTS_PASSED=$((TESTS_PASSED + 1))
    else
        echo -e "${RED}✗ FAILED${NC}"
        TESTS_FAILED=$((TESTS_FAILED + 1))

        if [ "$output_match" = false ]; then
            echo -e "${RED}Output mismatch:${NC}"
            echo "Bash output:"
            echo "'$bash_output'"
            echo "Minishell output:"
            echo "'$minishell_output'"
        fi

        if [ "$exit_match" = false ]; then
            echo -e "${RED}Exit status mismatch:${NC}"
            echo "Bash exit: $bash_exit"
            echo "Minishell exit: $minishell_exit"
        fi
    fi
}

# Function to run a test without checking exit status
run_test_no_exit() {
    run_test "$1" "$2" "false"
}

# Print header
echo "=========================================="
echo "    MINISHELL UNIT TESTER"
echo "=========================================="

# Check if minishell exists
if [ ! -x "$MINISHELL" ]; then
    echo -e "${RED}Error: $MINISHELL not found or not executable${NC}"
    echo "Please compile minishell first or adjust MINISHELL variable"
    exit 1
fi

# ============================================
# ECHO TESTS
# ============================================
echo -e "\n${YELLOW}=== ECHO TESTS ===${NC}"

run_test "echo simple" "echo hello"
run_test "echo with -n flag" "echo -n hello"
run_test "echo multiple words" "echo hello world test"
run_test "echo empty" "echo"
run_test "echo with quotes" "echo 'hello world'"
run_test "echo with double quotes" 'echo "hello world"'

# ============================================
# PWD TESTS
# ============================================
echo -e "\n${YELLOW}=== PWD TESTS ===${NC}"

run_test "pwd" "pwd"

# ============================================
# CD TESTS
# ============================================
echo -e "\n${YELLOW}=== CD TESTS ===${NC}"

run_test "cd to /tmp" "cd /tmp && pwd"
run_test "cd to home" "cd ~ && pwd"
run_test "cd to .." "cd .. && pwd"

# ============================================
# ENV TESTS
# ============================================
echo -e "\n${YELLOW}=== ENV TESTS ===${NC}"

run_test "env shows variables" "env | grep PATH"
run_test "export variable" "export TEST=hello && echo \$TEST"
run_test "unset variable" "export TEST=hello && unset TEST && echo \$TEST"

# ============================================
# REDIRECTIONS TESTS
# ============================================
echo -e "\n${YELLOW}=== REDIRECTIONS TESTS ===${NC}"

run_test "output redirection" "echo hello > /tmp/test_minishell.txt && cat /tmp/test_minishell.txt && rm /tmp/test_minishell.txt"
run_test "append redirection" "echo hello > /tmp/test_minishell.txt && echo world >> /tmp/test_minishell.txt && cat /tmp/test_minishell.txt && rm /tmp/test_minishell.txt"
run_test "input redirection" "echo hello > /tmp/test_minishell.txt && cat < /tmp/test_minishell.txt && rm /tmp/test_minishell.txt"

# ============================================
# PIPES TESTS
# ============================================
echo -e "\n${YELLOW}=== PIPES TESTS ===${NC}"

run_test "simple pipe" "echo hello | cat"
run_test "pipe with grep" "echo -e 'hello\nworld\ntest' | grep world"
run_test "multiple pipes" "echo hello | cat | cat"

# ============================================
# EXIT STATUS TESTS
# ============================================
echo -e "\n${YELLOW}=== EXIT STATUS TESTS ===${NC}"

run_test "true command" "true"
run_test "false command" "false"
run_test "command not found" "/bin/nonexistent_command_xyz" false  # Don't check exit on this

# ============================================
# QUOTES TESTS
# ============================================
echo -e "\n${YELLOW}=== QUOTES TESTS ===${NC}"

run_test "single quotes" "echo 'hello world'"
run_test "double quotes" 'echo "hello world"'
run_test "mixed quotes" "echo \"hello 'world'\""

# ============================================
# SUMMARY
# ============================================
echo -e "\n=========================================="
echo "           TEST SUMMARY"
echo "=========================================="
echo "Total tests run: $TESTS_RUN"
echo -e "${GREEN}Passed: $TESTS_PASSED${NC}"
echo -e "${RED}Failed: $TESTS_FAILED${NC}"
echo "=========================================="

if [ $TESTS_FAILED -eq 0 ]; then
    echo -e "${GREEN}All tests passed! 🎉${NC}"
    exit 0
else
    echo -e "${RED}Some tests failed. Please review the output above.${NC}"
    exit 1
fi