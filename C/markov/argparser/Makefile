FLAGS_DEV=-Wall -g -O0 -D DEBUG_FLAG
FLAGS_DEV_NO_DEBUG=-Wall -g -O0
FLAGS_PROD=-Wall -O3

make: argparser.c argparser.h
	gcc $(FLAGS_DEV) argparser.c -o main

nodebug: argparser.c argparser.h
	gcc $(FLAGS_DEV_NO_DEBUG) argparser.c -o main