#!/bin/bash

# Colors for output
C_RST='\033[0m'
C_BOLD='\033[1m'
C_RED='\033[91m'
C_GREEN='\033[92m'
C_YELLOW='\033[93m'
C_BLUE='\033[94m'
C_MAGENTA='\033[95m'

# Counters
TESTS_RUN=0
TESTS_PASSED=0
TESTS_WARNING=0
TESTS_FAILED=0

# Path to minishell (adjust if needed)
MINISHELL="./minishell"

# FuC_RSTtion to clean minishell output
clean_minishell_output() {
	sed -E '
		# Remove ANSI escape sequeC_RSTes
		s/\x1b\[[0-9;]*[a-zA-Z]//g
		s/\x1b\([0-9;]*[a-zA-Z]//g

		# Remove prompt and everything after it on the same line
		s/\[[0-9]+\] minishell\$.*$//g

		# Remove lines that are only the prompt (empty after removal)
		/^[[:space:]]*$/d

		# Remove exit command line
		/^exit$/d
	'
}

header()
{
	echo -e "${C_BLUE}------------------------------------------------------------${C_RST}"
	echo -e "${C_MAGENTA}▶ $1${C_RST}"
	echo -e "${C_BLUE}------------------------------------------------------------${C_RST}"
}

display_mismatch()
{
	local bash_output="$1"
	local minishell_output="$2"
	local bash_exit="$3"
	local minishell_exit="$4"
	local output_match="$5"
	local exit_match="$6"

	if [ "$output_match" = false ]; then
		echo -e "${C_RED}Output mismatch:${C_RST}"
		echo -e "${C_MAGENTA}Bash output:${C_RST}"
		echo "'$bash_output'"
		echo -e "${C_MAGENTA}Minishell output:${C_RST}"
		echo "'$minishell_output'"
	fi
	if [ "$exit_match" = false ]; then
		echo -e "${C_RED}Exit status mismatch:${C_RST}"
		echo -e "${C_MAGENTA}Bash exit:${C_RST} $bash_exit"
		echo -e "${C_MAGENTA}Minishell exit:${C_RST} $minishell_exit"
	fi
}

display_result_warning()
{
	local output_match="$1"
	local exit_match="$2"
	local bash_output="$3"
	local minishell_output="$4"
	local bash_exit="$5"
	local minishell_exit="$6"

	if [ "$output_match" = true ] && [ "$exit_match" = true ]; then
		echo -e "${C_BOLD}${C_GREEN}✔ PASSED${C_RST}"
		TESTS_PASSED=$((TESTS_PASSED + 1))
	else
		if [ "$exit_match" = true ]; then
			echo -e "${C_BOLD}${C_YELLOW}✔ PASSED (Check output)${C_RST}"
			TESTS_WARNING=$((TESTS_WARNING + 1))
			display_mismatch "$bash_output" "$minishell_output" "$bash_exit" "$minishell_exit" "$output_match" "$exit_match"
		else
			echo -e "${C_BOLD}${C_RED}✖ FAILED${C_RST}"
			TESTS_FAILED=$((TESTS_FAILED + 1))
			display_mismatch "$bash_output" "$minishell_output" "$bash_exit" "$minishell_exit" "$output_match" "$exit_match"
		fi
	fi
}

display_result_normal()
{
	local output_match="$1"
	local exit_match="$2"
	local bash_output="$3"
	local minishell_output="$4"
	local bash_exit="$5"
	local minishell_exit="$6"

	if [ "$output_match" = true ] && [ "$exit_match" = true ]; then
		echo -e "${C_BOLD}${C_GREEN}✔ PASSED${C_RST}"
		TESTS_PASSED=$((TESTS_PASSED + 1))
	else
		echo -e "${C_BOLD}${C_RED}✖ FAILED${C_RST}"
		TESTS_FAILED=$((TESTS_FAILED + 1))
		display_mismatch "$bash_output" "$minishell_output" "$bash_exit" "$minishell_exit" "$output_match" "$exit_match"
	fi
}

run_test()
{
	local test_name="$1"
	local command="$2"
	local warning_test=${3:-false}
	local output_match=false
	local exit_match=false

	TESTS_RUN=$((TESTS_RUN + 1))

	echo -e "\n${C_BLUE}Test #$TESTS_RUN: $test_name${C_RST}"
	echo -e "${C_YELLOW}Command:${C_RST} $command"

	# Run in bash
	local bash_output=$(printf "$command\n" | bash 2>&1)
	printf "$command\n" | bash &> bash_output.tmp
	local bash_exit=$?
	local bash_output=$(cat bash_output.tmp)

	# Run in minishell (send command + exit)
	printf "$command\nexit\n" | $MINISHELL &> minishell_output.tmp
	local minishell_exit=$?
	local minishell_output=$(cat minishell_output.tmp | clean_minishell_output)

	rm bash_output.tmp
	rm minishell_output.tmp

	if [ "$bash_output" = "$minishell_output" ]; then
		output_match=true
	fi
	if [ $bash_exit -eq $minishell_exit ]; then
		exit_match=true
	fi

	# Display results
	if [ "$warning_test" = true ]; then
		display_result_warning "$output_match" "$exit_match" \
			"$bash_output" "$minishell_output" "$bash_exit" "$minishell_exit"
	else
		display_result_normal "$output_match" "$exit_match" \
			"$bash_output" "$minishell_output" "$bash_exit" "$minishell_exit"
	fi
}

# Print
header "MINISHELL TESTER"

# Check if minishell exists
if [ ! -x "$MINISHELL" ]; then
	echo -e "${C_RED}Error: $MINISHELL not found or not executable${C_RST}"
	echo "Please compile minishell first or adjust MINISHELL variable"
	exit 1
fi



header "ECHO TESTS"
run_test "echo simple" "echo hello"
run_test "echo with -n flag" "echo -n hello"
run_test "echo with many -n flags" "echo -n -n -n hello"
run_test "echo with many -n flags with multiple n" "echo -nnnn -nnnnnnn -n hello"
run_test "echo multiple words" "echo hello world test"
run_test "echo empty" "echo"
run_test "echo with quotes" "echo 'hello world'"
run_test "echo with double quotes" 'echo "hello world"'

header "PWD TESTS"
run_test "pwd" "pwd"

header "CD TESTS"
run_test "cd to /tmp" "cd /tmp\npwd"
run_test "cd to home" "cd ~\npwd"
run_test "cd to .." "cd ..\npwd"
run_test "cd to relative path" "cd /tmp\ncd ..\npwd"
run_test "cd to nonexistent" "cd /nonexistent/path" true

header "ENV TESTS"
run_test "env shows variables" "env"
run_test "export variable" "export TEST=hello\necho \$TEST"
run_test "unset variable" "export TEST=hello\nunset TEST\necho \$TEST"

header "REDIRECTIONS TESTS"
# Test syntax errors first
run_test "redirect without file" "echo >" true
run_test "redirect append without file" "echo >>" true
run_test "redirect input without file" "cat <" true
# Test basic redirections (create separate tests for each step)
run_test "output redirection simple" "echo hello > /tmp/test_mini.txt"
run_test "read redirected file" "cat /tmp/test_mini.txt"
run_test "append redirection" "echo world >> /tmp/test_mini.txt"
run_test "read appended file" "cat /tmp/test_mini.txt"
run_test "input redirection" "cat < /tmp/test_mini.txt"
run_test "cleanup test file" "rm /tmp/test_mini.txt"

# Multiple redirections
run_test "multiple output redirections" "echo hello > /tmp/test1.txt > /tmp/test2.txt"
run_test "check first file (should be empty)" "cat /tmp/test1.txt"
run_test "check second file (should have content)" "cat /tmp/test2.txt"
run_test "cleanup multiple files" "rm -f /tmp/test1.txt /tmp/test2.txt"

header "PIPES TESTS"
run_test "simple pipe" "echo hello | cat"
run_test "pipe with wc" "echo hello world | wc -w"
run_test "multiple pipes" "echo hello | cat | cat"
run_test "pipe with grep" "printf 'hello\nworld\ntest' | grep world"

header "BUILTIN TESTS"
run_test "exit with code" "exit 42"
run_test "exit without code" "exit"

header "ERROR HANDLING TESTS"
run_test "command not found" "/bin/nonexistent_command_xyz" true
run_test "invalid option" "ls --invalid-option"

header "QUOTES AND SPECIAL CHARS TESTS"
run_test "single quotes" "echo 'hello world'"
run_test "double quotes" 'echo "hello world"'
run_test "mixed quotes" "echo \"hello 'world'\""
run_test "quotes with spaces" "echo 'hello    world'"
run_test "empty quotes" "echo ''"
run_test "quotes with special chars" "echo 'hello \$USER world'"

header "VARIABLE EXPANSION TESTS"
run_test "expand HOME" "echo \$HOME"
run_test "expand USER" "echo \$USER"
run_test "expand PATH" "echo \$PATH"
run_test "expand nonexistent var" "echo \$NONEXISTENT"
run_test "expand with text" "echo Hello \$USER!"

header "WHITESPACE TESTS"
run_test "multiple spaces" "echo hello    world"
run_test "tabs and spaces" "echo hello		world"
run_test "leading spaces" "   echo hello"
run_test "trailing spaces" "echo hello   "

header "SYNTAX ERROR TESTS"
run_test "unclosed single quote" "echo 'hello" true
run_test "unclosed double quote" 'echo "hello' true
run_test "pipe at end" "echo hello |" true
run_test "pipe at start" "| echo hello" true
run_test "empty pipe" "echo | | cat" true

header "TEST COMMAND"
run_test "Non Existent Command" "aa"

header "SUMMARY"
echo "Total tests run: $TESTS_RUN"
echo -e "${C_GREEN}Passed: $TESTS_PASSED${C_RST}"
echo -e "${C_YELLOW}Passed: $TESTS_WARNING${C_RST}"
echo -e "${C_RED}Failed: $TESTS_FAILED${C_RST}"
echo -e "${C_BLUE}------------------------------------------------------------${C_RST}"

if [ $TESTS_FAILED -eq 0 ]; then
	echo -e "${C_GREEN}All tests passed! 🎉${C_RST}"
	exit 0
else
	echo -e "${C_RED}Some tests failed. Please review the output above.${C_RST}"
	exit 1
fi