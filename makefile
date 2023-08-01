VERSION = 0.0.1
COMPILER = g++
# Options passed to COMPILER
CFLAGS = -Wall 
ifdef DEBUG
CFLAGS += -g
endif

PLATFORM := standard

#/:= $(strip \)
SRC_DIR = src
PLATFORMS_DIR = platforms
TEST_DIR = test
HEADER_DIR = include
LIB_DIR = libs
BUILD_DIR = obj

# Recursive wildcard function
rwildcard=$(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2)$(filter $(subst *,%,$2),$d))

# List of source files (using recursive wildcard)
ifeq ($(OS),Windows_NT)
    SRC_FILES := $(foreach d,$(SRC_DIR),$(call rwildcard,$d/,*.cpp))
else
    SRC_FILES := $(call rwildcard,$(SRC_DIR)/,*.cpp)
endif

# Generate a list of object file names by replacing .cpp with .o
OBJ_FILES = $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SRC_FILES))

# Platform-specific settings
ifeq ($(OS),Windows_NT)
    RM := del /Q
    MKDIR := mkdir
else
    RM := rm -f
    MKDIR := mkdir -p
endif

# Default target
all: $(OBJ_FILES)

# Rule to build object files from CPP source files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	-$(MKDIR) $(subst /,\,$(@D))
	$(COMPILER) -I$(HEADER_DIR) -I$(PLATFORMS_DIR) $(CFLAGS) -c $< -o $@

# Clean rule to remove object files and the executable
clean:
	rm -rf $(BUILD_DIR)

# PHONY targets to avoid conflicts with file names
.PHONY: all clean