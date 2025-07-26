# Makefile for Plan Recognition Error Detection Project
# Author: Generated for César Ombredane's project

# Compiler and flags
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++11 -O2
DEBUG_FLAGS = -g -DDEBUG
INCLUDE_DIR = src/include
SOURCE_DIR = src/object
BUILD_DIR = build

# Suppress specific warnings for third-party headers
SUPPRESS_WARNINGS = -Wno-unused-parameter -Wno-implicit-fallthrough -Wno-free-nonheap-object

# Target executable
TARGET = parc

# Source files
SOURCES = $(wildcard $(SOURCE_DIR)/*.cpp)
OBJECTS = $(SOURCES:$(SOURCE_DIR)/%.cpp=$(BUILD_DIR)/%.o)
HEADERS = $(wildcard $(INCLUDE_DIR)/*.h)

# Default target
all: $(BUILD_DIR) $(TARGET)

# Create build directory
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Build the main executable
$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^
	@echo "Build complete: $(TARGET)"

# Compile source files to object files
$(BUILD_DIR)/%.o: $(SOURCE_DIR)/%.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) $(SUPPRESS_WARNINGS) -I$(INCLUDE_DIR) -c $< -o $@

# Debug build
debug: CXXFLAGS += $(DEBUG_FLAGS)
debug: clean $(BUILD_DIR) $(TARGET)
	@echo "Debug build complete: $(TARGET)"

# Clean build artifacts
clean:
	rm -rf $(BUILD_DIR)
	rm -f $(TARGET)
	@echo "Clean complete"

# Install (copy to /usr/local/bin - requires sudo)
install: $(TARGET)
	sudo cp $(TARGET) /usr/local/bin/
	@echo "Installed $(TARGET) to /usr/local/bin/"

# Uninstall
uninstall:
	sudo rm -f /usr/local/bin/$(TARGET)
	@echo "Uninstalled $(TARGET) from /usr/local/bin/"

# Run the program
run: $(TARGET)
	./$(TARGET)

# Show help
help:
	@echo "Available targets:"
	@echo "  all      - Build the project (default)"
	@echo "  debug    - Build with debug flags"
	@echo "  clean    - Remove build artifacts"
	@echo "  install  - Install to /usr/local/bin (requires sudo)"
	@echo "  uninstall- Remove from /usr/local/bin (requires sudo)"
	@echo "  run      - Build and run the program"
	@echo "  help     - Show this help message"

# Check dependencies
check-deps:
	@echo "Checking for required tools..."
	@which $(CXX) > /dev/null || (echo "Error: $(CXX) not found" && exit 1)
	@echo "All dependencies satisfied"

# Show project info
info:
	@echo "Project: Plan Recognition Error Detection"
	@echo "Compiler: $(CXX)"
	@echo "Flags: $(CXXFLAGS)"
	@echo "Sources: $(SOURCES)"
	@echo "Target: $(TARGET)"

# Phony targets
.PHONY: all debug clean install uninstall run help check-deps info

# Dependency tracking
-include $(OBJECTS:.o=.d)

# Generate dependency files
$(BUILD_DIR)/%.d: $(SOURCE_DIR)/%.cpp
	@mkdir -p $(BUILD_DIR)
	@$(CXX) -MM -I$(INCLUDE_DIR) $< | sed 's|^|$(BUILD_DIR)/|' > $@
