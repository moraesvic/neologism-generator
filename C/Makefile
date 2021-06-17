FLAGS_DEV=-Wall -g -O0 -D DEBUG_FLAG
FLAGS_PROF=-Wall -O3 -pg
FLAGS_PROD=-Wall -O3

OBJ=markov.c my_random.o my_string.o args.o

make: $(OBJ)
	gcc $(FLAGS_DEV)  -Wno-pointer-sign $(OBJ) -o markov

prof: $(OBJ)
	gcc $(FLAGS_PROF) -Wno-pointer-sign $(OBJ) -o prof

perf: $(OBJ)
	gcc $(FLAGS_PROD) -Wno-pointer-sign $(OBJ) -o perf

# -----

args.o: args.c args.h
	gcc $(FLAGS_PROD) -c args.c -o args.o

my_random.o: my_random.c my_random.h
	gcc $(FLAGS_PROD) -c my_random.c -o my_random.o

my_string.o: my_string.c my_string.h
	gcc $(FLAGS_PROD) -c my_string.c -o my_string.o

# -----

all: $(OBJ)
	gcc $(FLAGS_DEV)  -Wno-pointer-sign $(OBJ) -o markov
	gcc $(FLAGS_PROF) -Wno-pointer-sign $(OBJ) -o prof
	gcc $(FLAGS_PROD) -Wno-pointer-sign $(OBJ) -o perf

clean:
	rm *.o *.bin *.out