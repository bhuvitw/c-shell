# ./bin/main: ./src/main.c ./src/builtin.c
# 	gcc -Wall -Wextra ./src/main.c ./src/builtin.c ./src/parser.c -o ./bin/main


./bin/main: ./bin/main.o ./bin/parser.o ./bin/builtin.o
	gcc -Wall -Wextra ./bin/main.o ./bin/parser.o ./bin/builtin.o -o ./bin/main
	

./bin/parser.o: ./src/parser.c
	gcc -Wall -Wextra -c ./src/parser.c -o ./bin/parser.o

./bin/builtin.o: ./src/builtin.c
	gcc -Wall -Wextra -c ./src/builtin.c -o ./bin/builtin.o

./bin/main.o: ./src/main.c
	gcc -Wall -Wextra -c ./src/main.c -o ./bin/main.o
