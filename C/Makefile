FLAGS_DEV=-Wall -g -O0 -D DEBUG_FLAG
FLAGS_PROF=-Wall -O3 -pg
FLAGS_PROD=-Wall -O3
FLAGS_DYNLIB=-shared -fpic -D NO_MAIN

SRC=markov.c my_random.o my_string.o args.o
BIN_DIR=../bin/

make: $(SRC)
	gcc $(FLAGS_DEV)  -Wno-pointer-sign $(SRC) -o $(BIN_DIR)markov

prof: $(SRC)
	gcc $(FLAGS_PROF) -Wno-pointer-sign $(SRC) -o $(BIN_DIR)prof

perf: $(SRC)
	gcc $(FLAGS_PROD) -Wno-pointer-sign $(SRC) -o $(BIN_DIR)perf

qshared: args.c my_random.c my_string.c markov.c
	gcc $(FLAGS_DEV) -D NO_MAIN -c args.c -o args.o
	gcc $(FLAGS_DEV) -D NO_MAIN -c my_random.c -o my_random.o
	gcc $(FLAGS_DEV) -D NO_MAIN -c my_string.c -o my_string.o
	gcc $(FLAGS_DEV) -D NO_MAIN -Wno-pointer-sign $(SRC) -o $(BIN_DIR)neogen
	rm args.o my_random.o my_string.o

shared: args.c my_random.c my_string.c markov.c
	gcc $(FLAGS_PROD) $(FLAGS_DYNLIB) -c args.c -o args.o
	gcc $(FLAGS_PROD) $(FLAGS_DYNLIB) -c my_random.c -o my_random.o
	gcc $(FLAGS_PROD) $(FLAGS_DYNLIB) -c my_string.c -o my_string.o
	gcc $(FLAGS_PROD) $(FLAGS_DYNLIB) -Wno-pointer-sign $(SRC) -o $(BIN_DIR)neogen.so
	rm args.o my_random.o my_string.o

shared_debug: args.c my_random.c my_string.c markov.c
	gcc $(FLAGS_DEV) $(FLAGS_DYNLIB) -c args.c -o args.o
	gcc $(FLAGS_DEV) $(FLAGS_DYNLIB) -c my_random.c -o my_random.o
	gcc $(FLAGS_DEV) $(FLAGS_DYNLIB) -c my_string.c -o my_string.o
	gcc $(FLAGS_DEV) $(FLAGS_DYNLIB) -Wno-pointer-sign $(SRC) -o $(BIN_DIR)neogen.so
	rm args.o my_random.o my_string.o

# -----

args.o: args.c args.h
	gcc $(FLAGS_PROD) -c args.c -o args.o

my_random.o: my_random.c my_random.h
	gcc $(FLAGS_PROD) -c my_random.c -o my_random.o

my_string.o: my_string.c my_string.h
	gcc $(FLAGS_PROD) -c my_string.c -o my_string.o

# -----

all: $(SRC)
	gcc $(FLAGS_DEV)  -Wno-pointer-sign $(SRC) -o $(BIN_DIR)markov
	gcc $(FLAGS_PROF) -Wno-pointer-sign $(SRC) -o $(BIN_DIR)prof
	gcc $(FLAGS_PROD) -Wno-pointer-sign $(SRC) -o $(BIN_DIR)perf
	rm *.o

clean:
	rm *.o *.bin *.out