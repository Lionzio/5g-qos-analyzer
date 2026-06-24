CXX          = g++
CXXFLAGS     = -std=c++17 -Wall -Wextra -Wpedantic -I./include
TARGET       = 5g_qos_analyzer
TEST_TARGET  = run_tests

SRC_DIR      = src
BUILD_DIR    = build
TEST_DIR     = tests

# Identifica todos os .cpp da src/
ALL_SOURCES  = $(wildcard $(SRC_DIR)/*.cpp)
# Isola o arquivo main.cpp da aplicação
MAIN_SRC     = $(SRC_DIR)/main.cpp
# Filtra: pega todas as classes de lógica, MENOS a main.cpp
CORE_SOURCES = $(filter-out $(MAIN_SRC), $(ALL_SOURCES))

# Gera a lista de objetos esperados
CORE_OBJECTS = $(CORE_SOURCES:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)
MAIN_OBJECT  = $(BUILD_DIR)/main.o
TEST_OBJECT  = $(BUILD_DIR)/test_runner.o

all: prepare $(TARGET)

prepare:
	@if not exist $(BUILD_DIR) mkdir $(BUILD_DIR)

# Binário 1: Aplicação CLI (Lógica + main.cpp)
$(TARGET): $(CORE_OBJECTS) $(MAIN_OBJECT)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Binário 2: Suíte de Testes (Lógica + test_runner.cpp)
$(TEST_TARGET): $(CORE_OBJECTS) $(TEST_OBJECT)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/test_runner.o: $(TEST_DIR)/test_runner.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Alvo utilitário para compilar e já rodar os testes direto
test: prepare $(TEST_TARGET)
	@echo ===================================================
	@echo            EXECUTANDO SUITE DE TESTES
	@echo ===================================================
	@.\$(TEST_TARGET).exe

clean:
	@if exist $(BUILD_DIR) rmdir /s /q $(BUILD_DIR)
	@if exist $(TARGET).exe del /q $(TARGET).exe
	@if exist $(TEST_TARGET).exe del /q $(TEST_TARGET).exe

.PHONY: all prepare test clean