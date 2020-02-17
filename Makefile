OUT_FILE = stack
CC = gcc
SOURCE = $(wildcard *.c)
OBJ = ${SOURCE:.c=.o}
CFLAGS = -I ./include


all:
	$(CC) $(CFLAGS) -c $(SOURCE)
	$(CC) $(OBJ) -o $(OUT_FILE)

debug:
	$(CC) -g -DDEBUG $(CFLAGS) -c $(SOURCE)
	$(CC) $(OBJ) -o $(OUT_FILE)


clean:
	- rm -f $(OUT_FILE) *.o
