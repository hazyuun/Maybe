CC = gcc
CCFLAGS = -g -O2 
INCLUDE = -Iinclude

TARGET = bin/main

SRC = $(shell find ./src -name "*.c")
OBJ = $(patsubst ./src/%.c, ./obj/%.o, $(SRC))

.phony: all
all: dirs $(OBJ)
	@$(CC) $(OBJ) -o $(TARGET) $(CCFLAGS) $(INCLUDE) -lm

.phony: dirs
dirs:
	@mkdir -p $(dir $(OBJ)) bin

obj/%.o: src/%.c
	@echo [CC] $@
	@$(CC) -c $< -o $@ $(CCFLAGS) $(INCLUDE) $(DEFINES)

.phony: clean
clean:
	@rm -r obj

.phony: clean_all
clean_all:
	@rm -r obj lib bin
