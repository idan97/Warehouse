COMPILER_CMD = g++ -g -Wall -Weffc++ -std=c++11 -Iinclude

SRC_DIR=src
BIN_DIR=bin

# Get all .cpp files in the source directory
SOURCES=$(wildcard $(SRC_DIR)/*.cpp)

# Generate corresponding .o files in the bin directory
OBJECTS=$(patsubst $(SRC_DIR)/%.cpp,$(BIN_DIR)/%.o,$(SOURCES))

# Name of the final executable
EXECUTABLE=$(BIN_DIR)/warehouse

all: clean $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(COMPILER_CMD) $^ -o $@		

$(BIN_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(COMPILER_CMD) -c $< -o $@

clean:
	find $(BIN_DIR) -maxdepth 1 -type f -exec rm -f {} \;