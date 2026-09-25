# C-Shell — Project Behaviour

## 1. Basic command execution

csh> pwd
csh> ls
csh> ls -l
csh> cat file.txt
csh> head file.txt
csh> tail file.txt
csh> ps aux
csh> echo "Hello Handsome"
csh> vim temp.txt


## 2. Built-in commands

csh> cd ..
csh> history
csh> exit


## 3. Pipes

csh> ls | grep ".c"


## 4. Input / Output redirection

csh> cat < input.txt
csh> ls > output.txt
csh> ls >> output.txt
csh> some-command 2> error.txt
csh> some-command > output.txt 2> error.txt
csh> some-command > output.txt 2>&1


## 5. Background execution

csh> sleep 10 &


## 6. Command combinations

csh> cat input.txt | grep "error" > errors.txt
csh> cat input.txt | grep "error" | wc -l