OUT_FILE = stack
CC = gcc
SOURCE = $(wildcard *.c)
OBJ = ${SOURCE:.c=.o}
CFLAGS = -g -I ./include


all:
	$(CC) $(CFLAGS) -c $(SOURCE)
	$(CC) $(OBJ) -o $(OUT_FILE)

clean:
	- rm -f $(OUT_FILE) *.o
