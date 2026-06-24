CXX      = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Wpedantic -I./include
TARGET   = 5g_qos_analyzer

SRC_DIR  = src
BUILD_DIR= build

# Encontra todos os arquivos .cpp dentro da pasta src/
SOURCES  = $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS  = $(SOURCES:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)

all: prepare $(TARGET)

prepare:
	@if not exist $(BUILD_DIR) mkdir $(BUILD_DIR)

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	@if exist $(BUILD_DIR) rmdir /s /q $(BUILD_DIR)
	@if exist $(TARGET).exe del /q $(TARGET).exe

.PHONY: all prepare clean