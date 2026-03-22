![42](https://img.shields.io/badge/-42-black?style=for-the-badge&logo=42&logoColor=white)

![C](https://img.shields.io/badge/c-%2300599C.svg?style=for-the-badge&logo=c&logoColor=white) ![Bash Script](https://img.shields.io/badge/bash_script-%23121011.svg?style=for-the-badge&logo=gnu-bash&logoColor=white) ![Linux](https://img.shields.io/badge/Linux-FCC624?style=for-the-badge&logo=linux&logoColor=black) ![Git](https://img.shields.io/badge/git-%23F05033.svg?style=for-the-badge&logo=git&logoColor=white) ![Markdown](https://img.shields.io/badge/markdown-%23000000.svg?style=for-the-badge&logo=markdown&logoColor=white)[![Jira](https://img.shields.io/badge/Jira-0052CC?style=for-the-badge&logo=Jira&logoColor=white)]([https://seuprojeto.atlassian.net/jira/software/projects/PROJKEY/boards/1](https://allanaugustocarneirorabelo.atlassian.net/jira/software/projects/SCRUM/boards/1?jql=))

<img width="1200" height="500" alt="minishell banner" src="https://github.com/user-attachments/assets/88b45165-0fd3-4251-aa5a-c2ad56598fb7" />


# Minishell - 42 School

## 📖 Description

Minishell is a project from 42 School focused on implementing a simplified shell inspired by Bash. The goal is to gain a deep understanding of how Unix shells work, including command parsing, process execution, input/output redirection, and variable expansion.

This project helps develop skills in C programming, Unix systems, processes and signals, as well as reinforcing concepts in data structures and string manipulation.

## 🛠 Implemented Features

The Minishell includes:

Execution of built-in commands:

- `cd` – change the current directory.
- `echo` – display messages in the standard output.
- `pwd` – print the current directory.
- `export` – set environment variables.
- `unset` – remove environment variables.
- `env` – list environment variables.
- `exit` – exit the shell.
- Execution of `external commands` (like ls, cat, etc.).
- `Pipelines (|)` to chain commands.

Redirection:
- `>` and `>>` (output to file)
- `<` (input from file)
- Variable `expansion` ($VAR).
- `Signal handling` (CTRL+C, CTRL+\) in a controlled way.
- Command `parsing` with single and double quotes.

## ⚙️ Technologies

- Language: C
- System: Unix/Linux
- Compiler: cc
- Standard C libraries (unistd.h, stdlib.h, stdio.h, etc.)
  
## 📝 Project Structure

```c
minishell/
│
├── src/           # Minishell source code
├── include/       # Headers and structs
├── Makefile       # Compilation rules
├── README.md      # Documentation
└── tests/         # Scripts to test features
```

## 💻 How to Compile

To compile the project, use the included Makefile:

```
make
```

This will generate the executable:

```
./minishell
```
To clean compiled files:

```
make clean
make fclean   # Removes the executable as well
```

## 🚀 Usage

Once the shell starts, you can type commands like in Bash:
```
$ echo "Hello, Minishell!"
Hello, Minishell!
$ pwd
/home/user
$ ls -l | grep ".c"
main.c
utils.c
```
## 📚 References

Unix/Linux Man Pages
Bash documentation for reference
Material provided by 42 School

## 👨‍💻 Author
Allan Rabelo & Miguel Queiros
