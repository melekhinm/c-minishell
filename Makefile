CC = gcc
CFLAGS = -Wall -Wextra -g -I./src
LDFLAGS = -lreadline

SRC_DIR = src
SRCS = $(wildcard $(SRC_DIR)/*.c)

OBJS = $(SRCS:.c=.o)

TARGET = shell

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(SRC_DIR)/*.o

.PHONY: all clean run
