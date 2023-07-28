VERSION = 0.0.1
COMPILER = g++
# Options passed to COMPILER
CFLAGS = -Wall 
ifdef DEBUG
CFLAGS += -g
endif

PLATFORM = standard
# Directories
SRC_DIR := src
OBJ_DIR := obj
BIN_DIR := bin

# Find all CPP files recursively in the source directory
SOURCES := $(Get-ChildItem -path $(SRC_DIR) -Filter *.cpp -Recurse) 
$(info VAR is $(SOURCES))
# Generate a list of object file names by replacing .cpp with .o
OBJECTS := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SOURCES))
$(info VAR is $(OBJECTS))

# Default target
all: $(OBJECTS)

# Rule to build object files from CPP source files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(@D)
	$(COMPILER) $(CFLAGS) -c $< -o $@

# Clean rule to remove object files and the executable
clean:
	rm -rf $(OBJ_DIR)

# PHONY targets to avoid conflicts with file names
.PHONY: all clean