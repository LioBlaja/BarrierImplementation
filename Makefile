CC = gcc
OBJ = func

all:
	$(CC) -Wall -O3 main.c myBarrier.c -o $(OBJ)
exec:
	./$(OBJ)
clean:
	rm -rf $(OBJ)
