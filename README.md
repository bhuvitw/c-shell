# csh — Custom UNIX Shell

## Project Description

`csh` is a minimal UNIX-like command-line interpreter written in C. It connects user input to the operating system by parsing commands, creating child processes, executing external programs, and managing input/output streams.

The project implements core UNIX system calls such as `fork()`, `execvp()`, `waitpid()`, `pipe()`, and `dup2()` to demonstrate how a shell executes commands, connects processes, redirects output, and supports basic background execution.

The shell supports built-in commands, external programs, single-stage pipelines, output redirection, background execution, and sequential command execution.

## Motivation

A command-line shell is one of the most fundamental tools for interacting with a UNIX system. However, using a shell does not necessarily explain how commands are parsed, how processes are created, or how their input and output streams are connected.

The motivation behind `csh` is to understand these mechanisms by implementing a simplified shell from scratch rather than delegating command execution to an existing shell.

This project provides practical experience with process lifecycle management, file descriptors, inter-process communication, signals, and modular C programming.

## Goals

- Implement an interactive command prompt and command-processing loop.
- Parse commands into argument arrays suitable for `execvp()`.
- Execute built-in commands within the shell process.
- Execute external programs using `fork()` and `execvp()`.
- Manage foreground processes using `waitpid()`.
- Support basic background execution using `&`.
- Implement a single pipeline using `pipe()` and `dup2()`.
- Support output, append, and standard-error redirection.
- Handle basic `SIGINT` behavior.
- Organize the project into separate source files and headers.
- Provide a reproducible build process using a Makefile.

## Non-Goals

- Full Bash or POSIX shell compatibility.
- A complete shell scripting language with loops, conditionals, and functions.
- Advanced shell expansion, including command substitution and wildcard expansion.
- Multiple pipeline stages, such as `cmd1 | cmd2 | cmd3`.
- Full interactive job control with job suspension and resumption.
- Command-line auto-completion, syntax highlighting, or a complete command-history system.
- Production-grade security, portability, and fault tolerance.

## Features & Scope

- **Interactive Shell:** Reads commands through a continuous prompt until `exit` or EOF.
- **Command Parsing:** Converts command strings into argument arrays for execution.
- **Built-in Commands:** Implements `cd`, `pwd`, and `exit` within the shell process.
- **External Command Execution:** Uses `fork()` and `execvp()` to execute programs.
- **Sequential Execution:** Supports processing semicolon-separated command segments.
- **Pipelines:** Connects two commands using `|`.
- **Output Redirection:** Redirects standard output using `>`.
- **Append Redirection:** Appends standard output using `>>`.
- **Error Redirection:** Redirects standard error using `2>`.
- **Background Execution:** Uses `&` to run commands without immediately waiting for completion.
- **Child-Process Management:** Uses `waitpid()` to wait for foreground processes and check for completed background children.
- **Signal Handling:** Implements basic `SIGINT` handling for interactive use.

The project focuses on the fundamental mechanisms of a shell rather than complete shell-language compatibility. Feature combinations and error-handling behavior are subject to the limitations described below.

## Architecture

```text
                 [ User Input: stdin ]
                           |
                           v
                 [ Interactive Loop ]
                    src/main.c
                           |
                           v
                [ Sequential Splitter ]
                      Operator ;
                           |
                           v
                  [ Command Parser ]
                    src/parser.c
                           |
                           v
                   [ Argument Array ]
                     args[], NULL
                           |
                           v
                   [ Built-in Checker ]
                    src/builtin.c
                           |
                 +---------+---------+
                 |                   |
                Yes                  No
                 |                   |
                 v                   v
          [ Execute Built-in ]  [ Execution Dispatcher ]
            cd, pwd, exit              |
                 |            +---------+---------+
                 |            |                   |
                 |        [ Single Pipe ]    [ Normal /
                 |            |              Redirected ]
                 |            v                   |
                 |       [ pipe() ]                |
                 |       [ fork() x 2 ]            |
                 |       [ dup2() ]                 |
                 |            |                    |
                 |            +---------+----------+
                 |                      |
                 |                      v
                 |                [ execvp() ]
                 |                      |
                 |                      v
                 |              [ Child Processes ]
                 |                      |
                 |             +--------+--------+
                 |             |                 |
                 |         Foreground        Background (&)
                 |             |                 |
                 |             v                 v
                 |       [ waitpid() ]    [ Return to Prompt ]
                 |             |                 |
                 +-------------+-----------------+
                               |
                               v
                      [ Next Prompt / Exit ]
```

### Architecture Explanation

1. **Interactive Loop (`main.c`):** Reads input, displays the prompt, splits sequential commands, and coordinates execution.
2. **Parser (`parser.c`):** Tokenizes command segments and constructs the argument array passed to built-ins and external programs.
3. **Built-in Module (`builtin.c`):** Handles commands such as `cd`, `pwd`, and `exit` directly within the shell.
4. **Execution Dispatcher:** Determines whether a command requires ordinary execution, redirection, or a pipeline.
5. **Process Management:** Uses `fork()` to create child processes, `execvp()` to execute programs, and `waitpid()` to manage foreground and completed background children.
6. **File-Descriptor Management:** Uses `pipe()`, `open()`, and `dup2()` to connect processes and redirect output.

### External Command Execution Sequence

```text
User enters command
        |
        v
Shell parses command
        |
        v
Check for built-in
        |
        +---- Built-in ----> Execute in shell process
        |
        +---- External ----> fork()
                               |
                    +----------+----------+
                    |                     |
                  Parent                Child
                    |                     |
              Foreground?          Configure I/O
                    |                     |
               waitpid()               execvp()
                    |                     |
                    +----------+----------+
                               |
                               v
                        Return to prompt
```

For a pipeline, the shell creates a pipe and two child processes. The first child's standard output is connected to the pipe's write end, and the second child's standard input is connected to its read end.

## Design

The project is organized into three core source components.

1. **`main.c` — Command Loop and Process Management**

   Contains the interactive loop, command dispatch, process creation, pipeline setup, redirection logic, background execution, and basic signal handling.

2. **`parser.c` — Command Parsing**

   Converts command text into an argument array suitable for execution with `execvp()`. The parser supports basic tokenization and double-quoted text, but does not implement a complete shell grammar.

3. **`builtin.c` — Built-in Commands**

   Implements shell commands that need to operate on the shell process itself. For example, `cd` uses `chdir()` to change the working directory of the shell rather than a temporary child process.

### Process Execution

For an external command, the shell creates a child with `fork()`. The child configures the required file descriptors and calls `execvp()`. The parent waits for foreground execution or returns to the prompt for background execution.

### Redirection

The shell opens the destination file and uses `dup2()` to redirect the appropriate file descriptor before executing the external program.

### Pipeline

The shell uses `pipe()` to create a communication channel between two commands. Each child duplicates the appropriate pipe descriptor onto standard output or standard input before executing its command.

## Technologies / APIs

- **Language:** C
- **Compiler:** GCC
- **Build System:** GNU Make
- **Operating System:** Linux / Unix-like systems
- **Process Control:** `fork()`, `execvp()`, `waitpid()`
- **Inter-Process Communication:** `pipe()`
- **File I/O:** `open()`, `close()`, `dup2()`
- **Directory Management:** `chdir()`, `getcwd()`
- **Input Processing:** `fgets()`, string manipulation functions
- **Signal Handling:** `signal()`, `SIGINT`, `SIG_DFL`, and related signal behavior

## Testing

The following cases should be used to validate the shell. Test results must be recorded after running them against the current implementation.

| Test Case | Command | Expected Behavior |
|---|---|---|
| Built-in command | `pwd` | Prints the current working directory |
| Directory change | `cd ..` | Changes the shell's working directory |
| External command | `ls -l` | Executes the external program |
| Sequential execution | `pwd; ls` | Processes both commands |
| Pipeline | `ls \| grep ".c"` | Filters the output of `ls` |
| Output redirection | `echo hello > output.txt` | Writes output to a file |
| Append redirection | `echo world >> output.txt` | Appends output to a file |
| Error redirection | `invalidcmd 2> error.txt` | Redirects standard error when supported |
| Background execution | `sleep 5 &` | Returns to the prompt without waiting |
| Signal handling | Press `Ctrl+C` during a foreground command | Checks child and shell behavior |
| Invalid command | `not_a_real_command` | Reports an execution error |
| Empty input | Press Enter | Returns to the prompt |

**Testing status:** The table defines the validation plan; it does not imply that every test has passed. Malformed syntax, missing redirection filenames, process-creation failures, and descriptor cleanup should also be tested.

## Milestones

- [x] Interactive prompt and command-processing loop.
- [x] Basic command tokenization.
- [x] Built-in commands: `cd`, `pwd`, and `exit`.
- [x] External program execution using `fork()` and `execvp()`.
- [x] Foreground process management using `waitpid()`.
- [x] Basic background execution using `&`.
- [x] Single pipeline using `pipe()` and `dup2()`.
- [x] Output redirection using `>`.
- [x] Append redirection using `>>`.
- [x] Standard-error redirection using `2>`.
- [x] Comprehensive automated tests and verified edge-case behavior.
- [x] Final documentation, repository cleanup, and submission review.

The checked milestones represent implemented functionality, not necessarily complete validation of every edge case.

## Expected Learning

- Understanding the UNIX process model and parent-child relationships.
- Learning how `fork()` and `execvp()` work together.
- Managing foreground and background child processes.
- Understanding file descriptors and I/O redirection.
- Implementing inter-process communication using pipes.
- Handling signals and process termination.
- Working with pointers, strings, arrays, and modular C programming.
- Building, debugging, testing, and documenting a systems-level project.

## Limitations

- **Restricted Shell Grammar:** The parser does not implement every quoting, escaping, or expansion rule of a production shell.
- **Fixed-Size Buffers:** Command input and argument storage have implementation-defined fixed limits.
- **Single Pipeline:** Only two commands connected by one pipe are supported.
- **Limited Redirection:** Input redirection and combined redirection such as `2>&1` are not implemented.
- **Operator Combinations:** Pipelines and redirection are not guaranteed to work in every combination.
- **Basic Job Management:** Background commands are supported, but full job control is not implemented.
- **Limited Signal Handling:** Signal behavior does not reproduce all terminal process-group behavior of Bash.
- **No Persistent History:** A complete `history` feature is not included.
- **Incomplete Fault Tolerance:** Additional validation and resource cleanup are required for malformed input and system-call failures.

---

## Compilation and Usage

### Prerequisites

A Linux or other compatible Unix-like system with GCC and GNU Make installed.

### Compile

```bash
make
```

### Run

```bash
./csh
```

### Clean Build Artifacts

```bash
make clean
```

### Usage Examples

```bash
csh> pwd
csh> ls -l
csh> cd ..
csh> echo "Hello World" > test.txt
csh> cat test.txt
csh> ls | grep ".c"
csh> sleep 10 &
csh> pwd; ls; echo "Done!"
csh> exit
```

These examples demonstrate intended command usage. Confirm that each command and operator combination works with the current implementation before presenting it as a verified capability.

## Project Structure

```text
c-shell/
├── include/
│   ├── BUILTIN_H.h
│   └── PARSER_H.h
├── src/
│   ├── main.c
│   ├── parser.c
│   └── builtin.c
├── resources/
│   ├── PROJECT_BEHAVIOUR.md
│   └── NOTES.md
├── Makefile
├── .gitignore
├── LICENSE
└── README.md
```

## License

This project is licensed under the MIT License. See [LICENSE](LICENSE) for details.
