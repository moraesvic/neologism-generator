FLAGS_DEV=-Wall -g -O0 -D DEBUG_FLAG
FLAGS_PROF=-Wall -O3 -pg
FLAGS_PROD=-Wall -O3

OBJ=markov.c my_random.o my_string.o args.c

make: $(OBJ)
	gcc $(FLAGS_DEV)  -Wno-pointer-sign $(OBJ) -o markov

prof: $(OBJ)
	gcc $(FLAGS_PROF) -Wno-pointer-sign $(OBJ) -o prof

perf: $(OBJ)
	gcc $(FLAGS_PROD) -Wno-pointer-sign $(OBJ) -o perf

# -----

my_random.o: my_random.c my_random.h
	gcc $(FLAGS_PROD) -c my_random.c -o my_random.o

my_string.o: my_string.c my_string.h
	gcc $(FLAGS_PROD) -c my_string.c -o my_string.o

# -----

all:
	gcc $(FLAGS_DEV)  -Wno-pointer-sign markov.c my_random.o -o markov
	gcc $(FLAGS_PROD) -pg -Wno-pointer-sign my_random.o markov.c -o prof
	gcc $(FLAGS_PROD) -Wno-pointer-sign my_random.o markov.c -o perf

clean:
	rm my_random.o my_string.o