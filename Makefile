# Makefile for Library Management System (Reorganized)

# Compiler
CXX = g++

# Compiler flags
# -Iinclude: Look for header files in the include directory
CXXFLAGS = -std=c++14 -Wall -Wextra -g -Iinclude

# Directories
SRC_DIR = src
OBJ_DIR = obj
INC_DIR = include
TEST_DIR = tests
DATA_DIR = data

# Target executable
TARGET = library_system

# Source files
SOURCES = $(SRC_DIR)/main.cpp $(SRC_DIR)/Book.cpp $(SRC_DIR)/Student.cpp $(SRC_DIR)/Library.cpp $(SRC_DIR)/FileStore.cpp
TEST_SOURCES = $(TEST_DIR)/test_system.cpp $(SRC_DIR)/Book.cpp $(SRC_DIR)/Student.cpp $(SRC_DIR)/Library.cpp

# Object files
OBJECTS = $(SOURCES:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)
TEST_OBJECTS = $(TEST_SOURCES:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

# Default target
all: $(OBJ_DIR) $(TARGET)

# Create object directory if it doesn't exist
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR) || mkdir $(OBJ_DIR)

# Build the executable
$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(TARGET)

# Compile source files to object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Test target
test: $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) $(TEST_SOURCES) -o test_system

# Clean build files
clean:
	rm -rf $(OBJ_DIR) $(TARGET) test_system || del /q $(OBJ_DIR)\* $(TARGET) test_system 2>nul && rmdir $(OBJ_DIR) 2>nul

# Run the program
run: all
	./$(TARGET)

# Help target
help:
	@echo "Available targets:"
	@echo "  all          - Build the library system"
	@echo "  test         - Build and run tests"
	@echo "  clean        - Remove build files"
	@echo "  run          - Build and run the program"
	@echo "  help         - Show this help message"

.PHONY: all clean run test help
