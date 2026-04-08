CC = clang
SRCDIR = src
SOURCES = $(addprefix $(SRCDIR)/, main.c ladder_node.c word_node.c print_utils.c helpers.c)
OBJS = $(SOURCES:.c=.o)
TARGET = ladderGame.exe

build:
	rm -f $(OBJS) $(TARGET)
	$(CC) $(SOURCES) -o $(TARGET)


run:
	./$(TARGET) -p ON

run_rand: 
	./$(TARGET) -p ON -n 4 -d dictionary.txt -s RAND -f RAND



run_fun: 
	./$(TARGET) -n 6 -d dictionary.txt -s rubbed -f clayey

clean_all:
	rm -f $(OBJS) $(TARGET)

.PHONY: build run_rand run_fun clean_all


