CC = gcc
CFLAGS =-Wall -Werror -ansi -lm
SRC = main.c compress_and_encrypt.c xor_cipher.c huffman_coding.c interface.c database.c
OUT = main.o

all: compile run

compile: $(SRC)
	$(CC) $(CFLAGS) -o $(OUT) $(SRC)

run: $(OUT)
	./$(OUT)

clean:
	rm -f $(OUT)

