VERSION = 0.0.1
COMPILER = g++
# Options passed to COMPILER
CFLAGS = -Wall 
ifdef DEBUG
    CFLAGS += -g
endif


HEADER_DIR := include
SRC_DIR := src

PLATFORM = win
PLATFORM_DIR= platforms
PLATFORMS_HEADER_DIR := $(PLATFORM_DIR)/$(PLATFORM)/$(HEADER_DIR)
PLATFORMS_SRC_DIR := $(PLATFORM_DIR)/$(PLATFORM)/$(SRC_DIR)

TEST_DIR := test
LIB_DIR := libs

ifdef DEBUG
    BUILD_DIR :=  obj/debug
    BIN_DIR := bin/debug
else
    BUILD_DIR :=  obj/release
    BIN_DIR := bin/release
endif

# Recursive wildcard function
rwildcard=$(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2)$(filter $(subst *,%,$2),$d))

# List of source files (using recursive wildcard)
ifeq ($(PLATFORM),win)
    SRC_FILES = $(foreach d,$(SRC_DIR),$(call rwildcard,$d/,*.cpp))
    SRC_FILES += $(foreach d,$(PLATFORMS_SRC_DIR),$(call rwildcard,$d/,*.cpp))
else
    SRC_FILES = $(call rwildcard,$(SRC_DIR)/,*.cpp)
    SRC_FILES += $(call rwildcard,$(PLATFORMS_SRC_DIR)/,*.cpp)
endif

# Generate a list of object file names by replacing .cpp with .o
OBJ_FILES = $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SRC_FILES))
OBJ_FILES += $(patsubst $(PLATFORMS_SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SRC_FILES))

# Platform-specific settings
ifeq ($(PLATFORM),win)
    RM := rmdir /Q /S
    MKDIR := mkdir
else
    RM := rm -f
    MKDIR := mkdir -p
endif

# PHONY targets to avoid conflicts with file names
.PHONY: all clean

# Default target
all: $(OBJ_FILES)
# Rule to build object files from CPP source files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	-$(MKDIR) $(subst /,\,$(@D))
	-$(COMPILER) -I$(HEADER_DIR) -I$(PLATFORMS_HEADER_DIR) $(CFLAGS) -c $< -o $@
    
$(BUILD_DIR)/%.o: $(PLATFORMS_SRC_DIR)/%.cpp
	-$(MKDIR) $(subst /,\,$(@D))
	-$(COMPILER) -I$(HEADER_DIR) -I$(PLATFORMS_HEADER_DIR) $(CFLAGS) -c $< -o $@


# Clean rule to remove object files and the executable
clean:
	-$(RM) $(subst /,\,$(BUILD_DIR))
