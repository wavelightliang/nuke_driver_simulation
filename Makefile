# ==============================================================================
# Makefile for Nuke Driver Simulation Project
# Version: 1.0
#
# Targets:
#   all     (default) Compiles the project.
#   run     Compiles and runs the project.
#   clean   Removes all generated files.
# ==============================================================================

# --- Configuration ---
# Compiler
CC = gcc

# Compiler flags:
# -Wall: Enable all standard warnings.
# -Wextra: Enable extra warnings beyond -Wall.
# -std=c11: Use the C11 standard.
# -g: Include debugging information.
CFLAGS = -Wall -Wextra -std=c11 -g

# --- File Definitions ---
# Target executable name
TARGET = nuke_simulation.exe

# Source files
SOURCES = main.c nuke_driver.c

# Object files (auto-generated from SOURCES)
OBJECTS = $(SOURCES:.c=.o)

# --- Build Rules ---

# Default target: 'all'
# This is the rule that runs if you just type 'make'.
all: $(TARGET)

# Rule to link the final executable from object files.
# It depends on all object files being up-to-date.
$(TARGET): $(OBJECTS)
	@echo "Linking..."
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJECTS)
	@echo "Build complete. Executable is '$(TARGET)'."

# Generic rule to compile a .c file into a .o file.
# '$<': The first prerequisite (the .c file).
# '$@': The target name (the .o file).
%.o: %.c nuke_driver.h
	@echo "Compiling $<..."
	$(CC) $(CFLAGS) -c $< -o $@

# Target to run the executable.
# It depends on 'all' to ensure the program is compiled first.
run: all
	@echo "Running application..."
	./$(TARGET)

# Target to clean up the directory.
# Removes object files and the final executable.
clean:
	@echo "Cleaning up project files..."
	rm -f $(OBJECTS) $(TARGET)
	@echo "Cleanup complete."

# Phony targets are not actual files.
# This tells 'make' that 'all', 'run', and 'clean' are command names.
.PHONY: all run clean