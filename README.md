# C-Shell — A Minimal Unix Shell Built From Scratch in C

## 1. Project Description

C-Shell is a minimal Unix-like shell built in C from scratch. It will allow users to enter and execute commands such as `pwd`, `ls`, `grep`, `cat`, `sleep`, and shell built-ins such as `cd` and `history`.

The shell will read user input as a string, parse it into a command and its arguments, determine how the command should be executed, and manage the resulting processes. For external commands, the shell will create a child process and use the appropriate execution mechanism to run the program while keeping the main shell process alive and ready for the next command.

The project is intended to help me understand how command-line shells work internally, particularly process creation, program execution, file descriptors, pipes, signals, and process management.

## 2. Motivation

By building this project, I want to improve my understanding of C and the underlying operating-system concepts involved in running commands. I use commands such as `ls`, `grep`, and `cat` regularly, but I do not yet fully understand what happens behind the scenes when I enter a command in a terminal.

Through this project, I want to understand how a shell receives and parses commands, creates and manages processes, executes programs, handles files and file descriptors, and allows different processes to communicate. I also want to use this understanding as a foundation for building more advanced and efficient software projects in the future.

### Concepts I expect to learn

**C**;- 

* Pointers
* Pointer-to-pointer
* Strings
* Dynamic memory
* Structs
* Arrays
* Linked lists

**Operating Systems / Systems Programming**

* File I/O
* File descriptors
* Process creation
* Inter-process communication (IPC)
* Signals

**Software Engineering / Tooling**

* Modular C programming
* Makefiles
* Debugging


## 3. Goals

Build a functioning minimal shell capable of interpreting user commands and managing the process required to execute them. 

### Learning Goals

Understand process creation, program execution, file descriptors, inter-process communication, signal, and memory management in C.

## 4. Non-Goals

* Full Bash compatibility
* Shell scripting language
* GUI
* Animations
* Themes

## 5. Features

* Command execution
* Built-in commands
* Arguments
* Input redirection
* Output redirection
* Pipes
* Background processes
* Signals
* History

### Level 1 — Core
    command execution
    arguments
    built-ins

### Level 2 — Systems features
    redirection
    pipes
    background processes
    signals

### Level 3 — Stretch
    history persistence
    environment variable expansion
    wildcard expansion
    command autocomplete

## 6. Project Scope

## 7. High-Level Architecture

Input
  ↓
Parser
  ↓
Command representation
  ↓
Built-in / external?
  ↓
Process management
  ↓
Redirection / pipeline
  ↓
Execution

## 8. Design

## 9. Technologies / APIs

## 10. Testing Strategy

## 11. Milestones

## 12. Expected Learning

## 13. Limitations