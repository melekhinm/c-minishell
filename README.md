# C-minishell

## Description
A simple Unix-like shell, created with an idea of learning something new, using instructions from Codecrafters as a guide.

**Disclaimer**: It should not be used as a reference or as learning material itself. Even though techniques used in creating it might be suboptimal, it is still a fully functional shell with fairly complex features written in C without significant performance issues.

## Overview
#### The general idea
shell prompts ($) the user for input and waits, when the input is received it is converted into list of arguments and executed either as a native shell builtin present in this source code, or one of the local executables within the user's path.

#### Input handling
It supports three modes for handling user input:


- **Regular mode**, where every argument is separated by a whitespace


- **Single quotes mode**, where everything inside is treated literally


- **Double quotes mode**, where everything inside except special characters is treated literally

#### Autocompletion
It uses readline library to support \<TAB\> autocompletion for builtins, local executables

#### Redirection
It supports redirecting output of STDOUT or STDERR into a file.


- **[>; 1>]** overwrite file with STDOUT


- **[>>; 1>>]** append STDOUT to file


- **[2>]** overwrite file with STDERR


- **[2>>]** append STDERR to file

#### Piping
It can redirect output of one program into the input of another using pipe operator |. (e.g. "ls | grep .c | wc") 

#### History
It saves command history inside **".shell_history"** in your home directory, so be aware of "garbage" file being created.

## Installation

This shell was written for Linux

#### Linux
#### Prerequisites
GNU readline is used throughout the project so we need to install it.
```bash
sudo apt install libreadline6 libreadline6-dev
```
#### Clone repo and build
```bash
git clone https://github.com/melekhinm/c-minishell && cd c-minishell
make && make clean
./shell
```
Afterwards, you can use **"./shell"** command for execution.


When inside, type exit to exit, Ctrl+C will not free memory or save history.

## Examples
Here are some silly, and not so silly examples:

**Basic commands**
```bash
$ echo hello world
hello world
$ echo hello     world
hello world
$ echo 'hello     world'
hello     world
$ echo "hello     world"
hello     world
$ echo hello\ \ \ \ world
hello    world
```
**Navigation**
```bash
$ ls
LICENSE  Makefile  README.md  shell  src
$ cd src/
$ ls
builtins.c    execute.c  main.c    pipe.c
completion.c  history.c  parser.c  shell.h
```
**Redirection**
```bash
$ echo hello > log
$ cat log
hello
$ echo another hello > log
$ cat log
another hello
$ echo even more hellos >> log
$ cat log
another hello
even more hellos
```
**Piping**
```bash
$ head pipe.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include "shell.h"

typedef enum {
$ cat pipe.c | grep dup2
                dup2(prev_pipe_read, STDIN_FILENO);
                dup2(pipefd[1], STDOUT_FILENO);
$ cat pipe.c | grep dup2 | wc
      2       4     100
```
