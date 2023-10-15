CC = gcc
CFLAGS = gcc -Wall -Werror -ansi -o employeelist.o employeelist.c -lm
SRC = man.c
OUT = man

all: compile run

compile: $(SRC)
	$(CC) $(CFLAGS) -o $(OUT) $(SRC)

run: $(OUT)
	./$(OUT)

clean:
	rm -f $(OUT)
