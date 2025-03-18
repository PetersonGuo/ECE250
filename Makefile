# Compiler and Flags
CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -pedantic -I../data_structures/include

# Source Files
SRCS := $(wildcard src/*.cpp) $(wildcard ../data_structures/src/*.tpp)

# Executable Name
TARGET := a.out

# Default Target
all: $(TARGET)

# Compile everything in one step (no object files)
$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) -O3 $^ -o $@

# Clean
clean:
	rm -f $(TARGET)

# Run
run: all
	./$(TARGET)

debug:
	$(CXX) $(CXXFLAGS) $(SRCS) -o $(TARGET) -g
