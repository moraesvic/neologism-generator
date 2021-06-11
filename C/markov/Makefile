FLAGS_DEV=-Wall -g -pg -O0
FLAGS_PROD=-Wall -O3

make: markov.c my_random.o
	gcc $(FLAGS_DEV) my_random.o markov.c -o markov

perf: markov.c my_random.o
	gcc $(FLAGS_PROD) my_random.o markov.c -o perf

my_random.o: my_random.c my_random.h
	gcc $(FLAGS_PROD) -c my_random.c -o my_random.o