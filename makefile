.PHONY: all clean

CC=gcc
CFLAGS=-Wall -Wextra -g -std=c23
SRC_DIR=src
OBJ_DIR=obj
BIN_DIR=bin

SRCS=$(wildcard $(SRC_DIR)/*.c)
OBJS=$(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRCS))
TARGET=$(BIN_DIR)/imgstitch

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm $(OBJ_DIR)/*.o $(TARGET)