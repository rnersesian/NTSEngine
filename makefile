# Compiler and flags
CC = gcc
CFLAGS = -Wall -g -MMD
LDFLAGS = `sdl2-config --libs` -lSDL2_image -lm

# Directories
SRC_DIR = src
OBJ_DIR = build/obj
OUT_DIR = build
OUT = $(OUT_DIR)/game

# Source and object files
SRC = $(wildcard $(SRC_DIR)/*.c)
OBJ = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRC))
DEP = $(OBJ:.o=.d)

# Default target
all: $(OUT)

# Link object files into the final binary
$(OUT): $(OBJ)
	mkdir -p $(OUT_DIR)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# Compile .c to .o and generate .d dependency file
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up build artifacts
clean:
	rm -rf $(OUT_DIR)

# Include dependency files if they exist
-include $(DEP)
