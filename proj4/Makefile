CC = gcc
CXX = g++

INC_DIR = ./include
SRC_DIR = ./src
OBJ_DIR = ./obj
BIN_DIR = ./bin
TEST_SRC_DIR = ./testsrc

CXXFLAGS = -std=c++17 -I$(INC_DIR) -Wall
LDFLAGS = -lpthread -lgtest -lgtest_main -lexpat

all: directories test_executables transplanner speedtest

directories:
	mkdir -p $(OBJ_DIR) $(BIN_DIR)

# Object files rules
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR)/%Test.o: $(TEST_SRC_DIR)/%Test.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Test executables rules
TEST_EXECUTABLES = teststrutils teststrdatasource teststrdatasink testdsv testxml testkml testcsvbs testosm testdpr testcsvbsi testtpcl testtp testfiledatass

test_executables: $(TEST_EXECUTABLES)

$(TEST_EXECUTABLES): test%: $(BIN_DIR)/test%
	@echo Running $<
	@$<

$(BIN_DIR)/test%: $(OBJ_DIR)/%Test.o $(OBJ_DIR)/%.o
	$(CXX) $^ -o $@ $(LDFLAGS)

# Special cases with multiple dependencies
$(BIN_DIR)/testfiledatass: $(OBJ_DIR)/FileDataSourceTest.o $(OBJ_DIR)/FileDataSource.o $(OBJ_DIR)/FileDataSink.o $(OBJ_DIR)/FileDataFactory.o $(OBJ_DIR)/FileDataSSTest.o
	$(CXX) $^ -o $@ $(LDFLAGS)

$(BIN_DIR)/testtp: $(OBJ_DIR)/CSVOSMTransportationPlannerTest.o $(OBJ_DIR)/DijkstraTransportationPlanner.o # Add other dependencies as needed
	$(CXX) $^ -o $@ $(LDFLAGS)

# Final executables
FINAL_EXECUTABLES = transplanner speedtest

$(FINAL_EXECUTABLES): %: $(BIN_DIR)/%
	@echo Building final executable $<

$(BIN_DIR)/transplanner: # Add dependencies
	# Build command here

$(BIN_DIR)/speedtest: # Add dependencies
	# Build command here

# Ensure tests are run before building final executables
transplanner speedtest: test_executables

clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

.PHONY: all clean directories test_executables $(TEST_EXECUTABLES) $(FINAL_EXECUTABLES)