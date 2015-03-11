TARGET=delcomment
OBJS=main.o

CC=gcc
CFLAGS= -Wall -O2

.SUFFIXES: .c .o

$(TARGET): $(OBJS)
	$(CC) -o $(TARGET) $^

.c.o:
	$(CC) $(CFLAGS) -c $<

.PHONY: clean
clean:
	$(RM) $(TARGET) $(OBJS)
