LIBB = mythread.a

CC = gcc

SRC = mymutex.c mycond.c mybarrier.c

TEST_SRC = mytest.c

TEST_OBJ = $(TEST_SRC:.c=.o)

SRC_OBJ = $(SRC:.c=.o)

INCLUDE = myatomic.h mythread.h mymutex.h myqueue.h mycond.h mybarrier.h

CFLAGS = -g

.PHONY: all lib clean test

all: lib test

lib: $(LIBB)

%.o: %.c $(INCLUDES)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f *.o *.~ a.out main

test: $(TEST_OBJ) $(SRC_OBJ) $(INCLUDE) lib
	$(CC) -o test $(CFLAGS) $(TEST_OBJ) $(SRC_OBJ) $(LIBB)