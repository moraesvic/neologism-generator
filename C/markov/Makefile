FLAGS_DEV=-Wall -g -O0
FLAGS_PROD=-Wall -O3

make: markov.c markov.h my_random.o
	gcc $(FLAGS_DEV)  -Wno-pointer-sign markov.c my_random.o -o markov

prof: markov.c markov.h my_random.o
	gcc $(FLAGS_PROD) -pg -Wno-pointer-sign my_random.o markov.c -o prof

perf: markov.c markov.h my_random.o
	gcc $(FLAGS_PROD) -Wno-pointer-sign my_random.o markov.c -o perf

# -----

my_random.o: my_random.c my_random.h
	gcc $(FLAGS_PROD) -c my_random.c -o my_random.o

# -----

all:
	gcc $(FLAGS_DEV)  -Wno-pointer-sign markov.c my_random.o -o markov
	gcc $(FLAGS_PROD) -pg -Wno-pointer-sign my_random.o markov.c -o prof
	gcc $(FLAGS_PROD) -Wno-pointer-sign my_random.o markov.c -o perf

clean:
	rm my_random.o