# Makefile for Banker's Algorithm Project
# This makes compilation super easy - just type 'make'!

# Compiler to use (g++ for C++)
CXX = g++

# Compiler flags:
# -Wall: Show all warnings (helps catch bugs!)
# -std=c++11: Use C++11 standard
# -g: Include debugging information
CXXFLAGS = -Wall -std=c++11 -g

# Name of our executable
TARGET = banker

# Source files
SOURCES = bankers_algorithm.cpp

# Object files (compiled but not linked)
OBJECTS = $(SOURCES:.cpp=.o)

# Default target - this runs when you just type 'make'
all: $(TARGET)

# Rule to create the executable
$(TARGET): $(OBJECTS)
	@echo "Linking object files to create executable..."
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJECTS)
	@echo "Build complete! Run with: ./$(TARGET)"

# Rule to compile source files into object files
%.o: %.cpp
	@echo "Compiling $<..."
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up compiled files
clean:
	@echo "Cleaning up compiled files..."
	rm -f $(TARGET) $(OBJECTS)
	@echo "Clean complete!"

# Run the program after building
run: $(TARGET)
	@echo "Running Banker's Algorithm..."
	@echo "================================"
	./$(TARGET)

# Create a test run with output redirection
test: $(TARGET)
	@echo "Running test and saving output to test_output.txt..."
	./$(TARGET) > test_output.txt 2>&1
	@echo "Test complete! Check test_output.txt for results."

# Help target - shows available commands
help:
	@echo "Available make commands:"
	@echo "  make        - Build the project"
	@echo "  make clean  - Remove compiled files"
	@echo "  make run    - Build and run the program"
	@echo "  make test   - Run and save output to file"
	@echo "  make help   - Show this help message"

# Declare targets that don't create files
.PHONY: all clean run test help
