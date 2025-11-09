BIN = ladders.exe
DEBUG_BIN = debug.exe
OUTPUT ?= output.log
DEBUG_FLAGS = -g -Og -Wall

build:
	gcc -g main.c -o ${BIN}

build_optimize:
	gcc -pg -g -O2 main.c -o ${BIN}

build_debug: clean_all
	gcc ${DEBUG_FLAGS} main.c -o ${DEBUG_BIN}

run_fun: build
	./${BIN} -p ON -n 4 -d dictionary.txt -s RAND -f RAND

run_gdb: build_debug
	gdb --args ./${DEBUG_BIN} -p ON -n 4 -d dictionary.txt -s data -f code

run_valgrind: build_debug
	valgrind -s --leak-check=full --show-leak-kinds=all --track-origins=yes --log-file=${OUTPUT} ./${DEBUG_BIN} -n 5 -d dictionary.txt -s RAND -f RAND

run_gprof: build_optimize
	./${BIN} -n 6 -d dictionary.txt -s rubbed -f clayey
	gprof ${BIN} gmon.out > ${OUTPUT}

clean_all:
	rm -f ${BIN} ${DEBUG_BIN} *.log *.out


