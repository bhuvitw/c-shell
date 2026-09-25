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