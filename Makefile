CC = gcc

CFLAGS = -O3 -fopenmp -Wall

SRC = src/main.c \
      src/matrix.c \
      src/sequential.c \
      src/openmp_naive.c \
      src/openmp_block.c \
      src/benchmark.c

OBJ = $(SRC:.c=.o)

EXEC = matrix

all:
	$(CC) $(CFLAGS) $(SRC) -o $(EXEC)

run:
	./$(EXEC)

clean:
	rm -f $(EXEC) src/*.o