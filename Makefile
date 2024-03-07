CC = gcc
CXX = g++

INC_DIR = ./include
SRC_DIR = ./src
OBJ_DIR = ./obj
BIN_DIR = ./bin
TEST_SRC_DIR = ./testsrc

CXXFLAGS = -std=c++17 -I$(INC_DIR) -Wall
LDFLAGS = -lpthread -lgtest -lgtest_main -lexpat

# Targets for building and running tests
TESTS = teststrutils teststrdatasource teststrdatasink testdsv testxml testcsvbs testosm \
        testfiledatass testkml testdpr testcsvbsi testtpcl testtp

# Final application targets
APPS = transplanner speedtest

all: directories $(TESTS) $(APPS)

directories: 
    @mkdir -p $(OBJ_DIR) $(BIN_DIR)

# Test executables
$(BIN_DIR)/test%: $(OBJ_DIR)/%Test.o $(OBJ_DIR)/%.o
    $(CXX) $^ -o $@ $(LDFLAGS)
    ./$@

# Specific rules for test executables that require linking multiple object files
$(BIN_DIR)/testfiledatass: $(OBJ_DIR)/FileDataSource.o $(OBJ_DIR)/FileDataSink.o $(OBJ_DIR)/FileDataFactory.o $(OBJ_DIR)/FileDataSSTest.o
    $(CXX) $^ -o $@ $(LDFLAGS)
    ./$@

$(BIN_DIR)/testkml: $(OBJ_DIR)/KMLWriter.o $(OBJ_DIR)/KMLTest.o
    $(CXX) $^ -o $@ $(LDFLAGS)
    ./$@

$(BIN_DIR)/testdpr: $(OBJ_DIR)/DijkstraPathRouter.o $(OBJ_DIR)/DijkstraPathRouterTest.o
    $(CXX) $^ -o $@ $(LDFLAGS)
    ./$@

$(BIN_DIR)/testcsvbsi: $(OBJ_DIR)/BusSystemIndexer.o $(OBJ_DIR)/CSVBusSystemIndexerTest.o
    $(CXX) $^ -o $@ $(LDFLAGS)
    ./$@

$(BIN_DIR)/testtpcl: $(OBJ_DIR)/TransportationPlannerCommandLine.o $(OBJ_DIR)/TPCommandLineTest.o
    $(CXX) $^ -o $@ $(LDFLAGS)
    ./$@

$(BIN_DIR)/testtp: $(OBJ_DIR)/CSVOSMTransportationPlannerTest.o $(OBJ_DIR)/DijkstraTransportationPlanner.o # Add other required object files here
    $(CXX) $^ -o $@ $(LDFLAGS)
    ./$@

# Compiling source files to object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
    $(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: $(TEST_SRC_DIR)/%Test.cpp
    $(CXX) $(CXXFLAGS) -c $< -o $@

# Application targets
transplanner: # add object files required for transplanner
    $(CXX) $(CXXFLAGS) $^ -o $(BIN_DIR)/$@ $(LDFLAGS)

speedtest: # add object files required for speedtest
    $(CXX) $(CXXFLAGS) $^ -o $(BIN_DIR)/$@ $(LDFLAGS)

clean:
    rm -rf $(OBJ_DIR) $(BIN_DIR)

.PHONY: all directories clean $(TESTS) $(APPS)
