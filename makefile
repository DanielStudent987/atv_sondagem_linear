CXX := g++

CPPFLAGS := -Iheader -Ilib -MMD -MP
CXXFLAGS := -std=c++20 -Wall -Wextra -Wpedantic -g3 -O0 \
	-fno-omit-frame-pointer -fdiagnostics-color=always -fsanitize=address,undefined
LDFLAGS := -fsanitize=address,undefined

SRC_DIR := src
TEST_DIR := test
BIN_DIR := bin

SOURCES := $(wildcard $(SRC_DIR)/*.cpp)
TEST_SOURCES := $(wildcard $(TEST_DIR)/*.cpp)
APP_OBJECTS := $(patsubst $(SRC_DIR)/%.cpp,$(BIN_DIR)/obj/src/%.o,$(SOURCES))
TEST_OBJECTS := $(patsubst $(TEST_DIR)/%.cpp,$(BIN_DIR)/obj/test/%.o,$(TEST_SOURCES))
DEPENDENCIES := $(APP_OBJECTS:.o=.d) $(TEST_OBJECTS:.o=.d)

EXECUTABLE := $(BIN_DIR)/main
FUNCTIONAL_TEST_EXECUTABLES := $(BIN_DIR)/test-buscar $(BIN_DIR)/test-inserir \
	$(BIN_DIR)/test-redimensionar $(BIN_DIR)/test-remover
PERFORMANCE_TEST_EXECUTABLE := $(BIN_DIR)/test-desempenho

.DEFAULT_GOAL := all
.PRECIOUS: $(TEST_OBJECTS)
.PHONY: all test run run-test run-test-inserir run-test-buscar \
	run-test-remover run-test-redimensionar run-test-desempenho clean

all: $(EXECUTABLE) $(FUNCTIONAL_TEST_EXECUTABLES)
test: $(FUNCTIONAL_TEST_EXECUTABLES)

$(EXECUTABLE): $(APP_OBJECTS)
	@mkdir -p $(@D)
	$(CXX) $(LDFLAGS) $^ -o $@

$(BIN_DIR)/test-buscar: $(BIN_DIR)/obj/test/TesteBuscar.o
	$(CXX) $(LDFLAGS) $^ -o $@
$(BIN_DIR)/test-desempenho: $(BIN_DIR)/obj/test/TesteDesempenho.o
	$(CXX) $(LDFLAGS) $^ -o $@
$(BIN_DIR)/test-inserir: $(BIN_DIR)/obj/test/TesteInserir.o
	$(CXX) $(LDFLAGS) $^ -o $@
$(BIN_DIR)/test-redimensionar: $(BIN_DIR)/obj/test/TesteRedimensionar.o
	$(CXX) $(LDFLAGS) $^ -o $@
$(BIN_DIR)/test-remover: $(BIN_DIR)/obj/test/TesteRemover.o
	$(CXX) $(LDFLAGS) $^ -o $@

$(BIN_DIR)/obj/src/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(@D)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

$(BIN_DIR)/obj/test/%.o: $(TEST_DIR)/%.cpp
	@mkdir -p $(@D)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

run: $(EXECUTABLE)
	./$(EXECUTABLE) $(ARGS)

run-test: test
	@set -e; for executable in $(FUNCTIONAL_TEST_EXECUTABLES); do ./$$executable $(ARGS); done

run-test-inserir: $(BIN_DIR)/test-inserir
	./$< $(ARGS)
run-test-buscar: $(BIN_DIR)/test-buscar
	./$< $(ARGS)
run-test-remover: $(BIN_DIR)/test-remover
	./$< $(ARGS)
run-test-redimensionar: $(BIN_DIR)/test-redimensionar
	./$< $(ARGS)
run-test-desempenho: $(PERFORMANCE_TEST_EXECUTABLE)
	./$< $(ARGS)

clean:
	@mkdir -p "$(BIN_DIR)"
	@find "$(BIN_DIR)" -mindepth 1 -maxdepth 1 ! -name .gitkeep -exec rm -rf {} +

-include $(DEPENDENCIES)
