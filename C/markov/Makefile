FLAGS_DEV=-Wall -g -pg -O0
FLAGS_PROD=-Wall -O3

make: markov.c
	gcc $(FLAGS_DEV) markov.c -o markov

perf: markov.c
	gcc $(FLAGS_PROD) markov.c -o perf