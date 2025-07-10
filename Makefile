CC = gcc
CFLAGS = -Wall -Wextra -O2 -std=c99
TARGET = malloc_benchmark
SIMPLE_TARGET = simple_malloc_benchmark
SOURCE = malloc_benchmark.c
SIMPLE_SOURCE = simple_malloc_benchmark.c

all: $(TARGET) $(SIMPLE_TARGET)

$(TARGET): $(SOURCE)
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCE)

$(SIMPLE_TARGET): $(SIMPLE_SOURCE)
	$(CC) $(CFLAGS) -o $(SIMPLE_TARGET) $(SIMPLE_SOURCE)

run: $(TARGET)
	./$(TARGET)

run-simple: $(SIMPLE_TARGET)
	./$(SIMPLE_TARGET)

clean:
	rm -f $(TARGET) $(SIMPLE_TARGET)

.PHONY: all run run-simple clean 