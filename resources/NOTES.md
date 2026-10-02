> Compilation is the process of taking your C source code and turning it into a program that your operating system can execute.

> JavaScript and Python devs aren’t used to a separate compilation step at all–though behind the scenes it’s happening! Python compiles your source code into something called bytecode that the Python virtual machine can execute. Java devs are used to compilation, but that produces bytecode for the Java Virtual Machine.

> Languages that typically aren’t compiled are called interpreted languages. But as we mentioned with Java and Python, they also have a compilation step. And there’s no rule saying that C can’t be interpreted. (There are C interpreters out there!) In short, it’s a bunch of gray areas. Compilation in general is just taking source code and turning it into another, more easily-executed form.

Strings

> Well, turns out strings aren’t actually strings in C. That’s right! They’re pointers! Of course they are!

```c
char *s = "Hello, world!";
char t[] = "Hello, again!";
```

> But these two initializations are subtly different. A string literal, similar to an integer literal, has its memory automatically managed by the compiler for you! With an integer, i.e. a fixed size piece of data, the compiler can pretty easily manage it. But strings are a variable-byte beast which the compiler tames by tossing into a chunk of memory, and giving you a pointer to it.

> This form points to wherever that string was placed. Typically, that place is in a land faraway from the rest of your program’s memory – read-only memory – for reasons related to performance & safety.

> So remember: if you have a pointer to a string literal, don’t try to change it! And if you use a string in double quotes to initialize an array, that’s not actually a string literal.

> The strlen() function returns type size_t, which is an integer type so you can use it for integer math. We print size_t with %zu.

```c
// string termination
char *s = "Hello!";  // Actually "Hello!\0" behind the scenes
```

The FILE* Data Type

> streams

End of File: EOF

> special character defined as a macro: EOF. This is what fgetc() will return when the end of the file has been reached and you’ve attempted to read another character.

Reading a line at time

> So how can we get an entire line at once? fgets() to the rescue! For arguments, it takes a pointer to a char buffer to hold bytes, a maximum number of bytes to read, and a FILE* to read from. It returns NULL on end-of-file or error. fgets() is even nice enough to NUL-terminate the string when its done77.

Running a command

> Function: int **system** (const char *command) ¶

Process Creation Process

> A new processes is created when one of the functions posix_spawn, fork, _Fork, vfork, or pidfd_spawn is called. (The system and popen also create new processes internally.) Due to the name of the fork function, the act of creating a new process is sometimes called forking a process. Each new process (the child process or subprocess) is allocated a process ID, distinct from the process ID of the parent process.

> After forking a child process, both the parent and child processes continue to execute normally. If you want your program to wait for a child process to finish executing before continuing, you must do this explicitly after the fork operation, by calling wait or waitpid (see Process Completion). These functions give you limited information about why the child terminated—for example, its exit status code.

> Having several processes run the same program is only occasionally useful. But the child can execute another program using one of the exec functions; see Executing a File. The program that the process is executing is called its process image. Starting execution of a new program causes the process to forget all about its previous process image; when the new program exits, the process exits too, instead of returning to the previous process image.

Creating a process

> The fork function is the primitive for creating a process. It is declared in the header file unistd.h.

> Function: pid_t **fork** (void)

> what does it return? return child pid to parent pid, 0 to child pid, -1 if fork failed

Executing a File 

> This section describes the exec family of functions, for executing a file as a process image. You can use these functions to make a child process execute a new program after it has been forked.

> The functions in this family differ in how you specify the arguments, but otherwise they all do the same thing. They are declared in the header file unistd.h.

> Function: int **execv** (const char *filename, char *const argv[])

BSD Process Wait Function

> The GNU C Library also provides the wait3 function for compatibility with BSD. This function is declared in sys/wait.h. It is the predecessor to wait4, which is more flexible. wait3 is now obsolete.

> Function: pid_t **wait3** (int *status-ptr, int options, struct rusage *usage)

Process Identification 

> Each process is named by a process ID number, a value of type pid_t. A process ID is allocated to each process when it is created. Process IDs are reused over time. The lifetime of a process ends when the parent process of the corresponding process waits on the process ID after the process has terminated. See Process Completion. (The parent process can arrange for such waiting to happen implicitly.) A process ID uniquely identifies a process only during the lifetime of the process. As a rule of thumb, this means that the process must still be running.

> On Linux, threads created by pthread_create also receive a thread ID. The thread ID of the initial (main) thread is the same as the process ID of the entire process. Thread IDs for subsequently created threads are distinct. They are allocated from the same numbering space as process IDs. Process IDs and thread IDs are sometimes also referred to collectively as task IDs. In contrast to processes, threads are never waited for explicitly, so a thread ID becomes eligible for reuse as soon as a thread exits or is canceled. This is true even for joinable threads, not just detached threads. Threads are assigned to a thread group. In the GNU C Library implementation running on Linux, the process ID is the thread group ID of all threads in the process.

> You can get the process ID of a process by calling getpid. The function getppid returns the process ID of the parent of the current process (this is also known as the parent process ID). Your program should include the header files unistd.h and sys/types.h to use these functions.

> Data Type: **pid_t** 

> Function: pid_t **getpid** (void) 

> Function: pid_t getppid (void) -> gets parent id 
> Function: pid_t gettid (void) -> gets thread id

strtok()

chdir()

```c
int chdir(const char *path);
int fchdir(int fd);
```

> chdir() changes the current working directory of the calling process to the directory specified in path.

> fchdir() is identical to chdir(); the only difference is that the directory is given as an open file descriptor.

> return: 0 (success), -1(error)

getcwd()

```c
char *getcwd(size_t size;
char buf[size], size_t size);
char *get_current_dir_name(void);
```

> The getcwd() function copies an absolute pathname of the current working directory to the array pointed to by buf, which is of length size.

> return: pointer to pathname(string)(success), NULL(error)

strcmp()

```c
int strcmp(const char *s1, const char *s2);
```
> return: 0(equal), <0(s1 less than s2), >0(s1 greatere than s2)


Your restructure is right: each built-in runs in the parent and then `continue`s, so nothing forks. Notes first, then the compiler errors.

## Day 4 notes: built-ins vs external commands

**External command**: a separate program file on disk (`/bin/ls`, `/usr/bin/grep`). The shell runs it with `fork()` + `execvp()`, so it lives in its own child process.

**Built-in**: code inside the shell itself. The shell runs it in its own process, with no fork.

**Why `cd` can't be `fork()` + `exec("cd")`**
1. The current working directory is a per-process attribute, and the kernel keeps it. It isn't in your program's memory.
2. `fork()` gives the child its own copy of that attribute. `chdir()` in the child changes only the child's copy.
3. When the child exits, that copy is gone. The parent (the shell) never saw the change and is still in the old directory.
4. So the process that must call `chdir()` is the shell itself. That is the definition of a built-in: a command that has to change the shell's own state.
5. Side facts: `which cd` prints nothing on most systems, so there's no `cd` program for `execvp` to find. And even if a stub existed, it couldn't fix point 3.

**Process model explanation**: parent and child are separate processes with separate kernel-tracked state after `fork()`. Nothing a child does to its own state propagates upward. (Memory is copy-on-write, so it looks separate too.)

**Other built-ins follow the same rule**: `exit` must end the shell process itself. `pwd` could be external, but it's cheap to do in the shell. Later, `history` needs the shell's own memory, and `cd`-like state changes (like `export`) all need built-in treatment.

**Implementation pattern**: parse, check `args[0]` with `strcmp` against each built-in, run it in the parent, then `continue` to skip the fork. Everything else goes to `fork` + `execvp`.

**APIs**: `chdir(path)` returns 0 on success and -1 on failure. `getcwd(buf, size)` fills a `char` buffer. `strcmp` returns 0 on a match, and that's why you write `== 0`.

**Things I got wrong or learned**: `==` on `char *` compares addresses, so use `strcmp`. `=` is assignment, not comparison. fork shares nothing writable after the copy.

Function Prototypes

> You can notify the compiler in advance that you’ll be using a function of a certain type that has a certain parameter list.

> If you don’t declare your function before you use it (either with a prototype or its definition), you’re performing something called an **implicit** declaration. This was allowed in the first C standard (C89), and that standard has rules about it, but is no longer allowed today. And there is no legitimate reason to rely on it in new code.

```c
int foo(void);  // This is the prototype!

int main(){
    i = foo();
}

int foo(void)  // This is the definition, just like the prototype!
{
    return 3490;
}
```

Mutlifile Projects

1. Includes and Function Prototypes
```c
// File foo.c

#include <stdio.h>

#include "bar.h"  // Include from current directory

int main(void)
{
    printf("%d\n", add(2, 3));  // 5!
}
```
dealing with repeated includes -> 
```c
#ifndef BAR_H   // If BAR_H isn't defined...
#define BAR_H   // Define it (with no particular value)

// File bar.h

int add(int, int);

#endif          // End of the #ifndef BAR_H
````
```c
// File bar.c

int add(int x, int y)
{
    return x + y;
}
```
```bash
gcc -o foo foo.c bar.c
```

4. Comipling with Object files

> Let’s say you had a thousand C files. You could compile them all to object files to start (slowly) and then combine all those object files into an executable (fast).

```bash
gcc -c foo.c     # produces foo.o
gcc -c bar.c     # produces bar.o
```
```bash
gcc -o foo foo.o bar.o
```

Include 

> In summary, used angle brackets (< and >) for the system includes, and use double quotes (") for your personal includes.

Makefile

Stream Level I/O

```text
why use it? 
•For reading binary files in large chunks.
• For reading an entire file into core before parsing it.
• To perform operations other than data transfer, which can only be done with a descriptor. (You can use fileno to get the descriptor corresponding to a stream.)
• To pass descriptors to a child process. (The child can create its own stream to use a
descriptor that it inherits, but cannot inherit a stream directly.)
```

Opening and closing Files

> open, create -> fctnl.h
> close -> unistd.h

```c
int open (const char *filename, int flags[, mode_t mode])
```
FLAGS in open
```text
O_CREAT: if pathname doesn't exist it creates new file -> mode_t mode set the permision of the file 
O_WRONLY: it'll give permissions to write
O_APPEND: it appends the file, file offset set to lseek(2)
O_TRUNC: it will truncate the length to 0 if(O_WRONLY & O_RDWR) is allowd 
```

```c
int creat (const char *filename, mode_t mode)
```
```text
This function is obsolete. The call:
    creat (filename, mode)
is equivalent to:
    open (filename, O_WRONLY | O_CREAT | O_TRUNC, mode)
```

```c
int close (int filedes)
```

read, write, and lseek

```c
ssize_t //[Data Type]
```
> This data type is used to represent the sizes of blocks that can be read or written in a single operation. It is similar to size_t, but must be a signed type.

```c
ssize_t read (int filedes, void *buffer, size_t size) 
```
```c
ssize_t write (int filedes, const void *buffer, size_t size)
```
```c
off_t lseek (int filedes, off_t offset, int whence)
```

Duplicate descriptor

> You can duplicate a file descriptor, or allocate another file descriptor that refers to the same open file as the original. Duplicate descriptors share one file position and one set of file status flags (see Section 13.15 [File Status Flags], page 405), but each has its own set of file descriptor flags (see Section 13.14 [File Descriptor Flags], page 404).

```c
int dup (int old) [Function]
```
>This function copies descriptor old to the first available descriptor number (the first number not currently open). It is equivalent to fcntl (old, F_DUPFD, 0).
```c
int dup2 (int old, int new) [Function]
```
> This function copies the descriptor old to descriptor number new. If old is an invalid descriptor, then dup2 does nothing; it does not close new. Otherwise, the new duplicate of old replaces any previous meaning of descriptor new, as if new were closed first. If old and new are different numbers, and old is a valid descriptor number, then dup2 is equivalent to:
```c
close (new);
fcntl (old, F_DUPFD, new)
```
> However, dup2 does this atomically; there is no instant in the middle of calling dup2 at which new is closed and not yet a duplicate of old.

File Descriptor

> a file descriptor is a small non-negative integer that a process uses to refer to an open file (or pipe, or socket, or terminal). The kernel keeps a table, per-process, mapping each of these integers to an actual open-file entry. The integer itself has no meaning outside that process — fd 3 in your shell and fd 3 in ls are completely unrelated open files

```text
fd 0 = stdin
fd 1 = stdout
fd 2 = stderr
```





















