# Compiler and default flags
CC      ?= gcc
CFLAGS  := -O3 -Wall -Wextra -pedantic
LDFLAGS := -lm

# OS Detection magic (True UNIX Hacker style)
UNAME := $(shell uname)
ifeq ($(UNAME), Darwin)
	LDFLAGS += -lncurses
	CC = clang
else ifeq ($(UNAME), Linux)
	CFLAGS  += -march=native
	LDFLAGS += -lncurses -ldl -lpthread
else ifeq ($(UNAME), Windows_NT)
	LDFLAGS += -lncurses
else
	# BSD fallbacks
	LDFLAGS += -lncurses -lpthread
	CC = clang
endif

# Directories
SRC     := src
OBJ     := obj
BINDIR  := bin
ASSETS  := assets

# Files
BIN     := $(BINDIR)/xtetris
SRCS    := $(wildcard $(SRC)/*.c)
OBJS    := $(patsubst $(SRC)/%.c, $(OBJ)/%.o, $(SRCS))

.PHONY: all clean run

# Default target
all: $(BIN)

# Link the final binary
$(BIN): $(OBJS)
	@mkdir -p $(BINDIR)
	$(CC) $(CFLAGS) $^ $(LDFLAGS) -o $@

# Compile object files
$(OBJ)/%.o: $(SRC)/%.c
	@mkdir -p $(OBJ)
	$(CC) $(CFLAGS) -c $< -o $@

# Run the game with the audio asset
run: $(BIN)
	./$(BIN) $(ASSETS)/title.mp3

# Clean build artifacts
clean:
	rm -rf $(OBJ) $(BINDIR)
