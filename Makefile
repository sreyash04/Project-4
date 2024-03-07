CC = gcc
CXX = g++

INC_DIR = ./include
SRC_DIR = ./src
OBJ_DIR = ./obj
BIN_DIR = ./bin
TEST_SRC_DIR = ./testsrc

CXXFLAGS = -std=c++17 -I $(INC_DIR) -Wall
LDFLAGS = -lpthread -lgtest -lgtest_main -lexpat

all: directories runtests

runtests: teststrutils teststrdatasource teststrdatasink testdsv testxml testcsvbs testosm testfiledatass testkml testdpr testcsvbsi testtpcl testtp

teststrutils: $(BIN_DIR)/teststrutils
	$(BIN_DIR)/teststrutils

teststrdatasource: $(BIN_DIR)/teststrdatasource
	$(BIN_DIR)/teststrdatasource

teststrdatasink: $(BIN_DIR)/teststrdatasink
	$(BIN_DIR)/teststrdatasink

testdsv: $(BIN_DIR)/testdsv
	$(BIN_DIR)/testdsv

testxml: $(BIN_DIR)/testxml
	$(BIN_DIR)/testxml

testcsvbs: $(BIN_DIR)/testcsvbs
	$(BIN_DIR)/testcsvbs

testosm: $(BIN_DIR)/testosm
	$(BIN_DIR)/testosm

testfiledatass: $(BIN_DIR)/testfiledatass
	$(BIN_DIR)/testfiledatass

testkml: $(BIN_DIR)/testkml
	$(BIN_DIR)/testkml

testdpr: $(BIN_DIR)/testdpr
	$(BIN_DIR)/testdpr

testcsvbsi: $(BIN_DIR)/testcsvbsi
	$(BIN_DIR)/testcsvbsi

testtpcl: $(BIN_DIR)/testtpcl
	$(BIN_DIR)/testtpcl

testtp: $(BIN_DIR)/testtp
	$(BIN_DIR)/testtp

# String Utils
$(BIN_DIR)/teststrutils: $(OBJ_DIR)/StringUtils.o $(OBJ_DIR)/StringUtilsTest.o
	$(CXX) -o $(BIN_DIR)/teststrutils $(CXXFLAGS) $(OBJ_DIR)/StringUtils.o $(OBJ_DIR)/StringUtilsTest.o $(LDFLAGS)

$(OBJ_DIR)/StringUtils.o: $(SRC_DIR)/StringUtils.cpp $(INC_DIR)/StringUtils.h
	$(CXX) -o $(OBJ_DIR)/StringUtils.o -c $(CXXFLAGS) $(SRC_DIR)/StringUtils.cpp

$(OBJ_DIR)/StringUtilsTest.o: $(TEST_SRC_DIR)/StringUtilsTest.cpp $(INC_DIR)/StringUtils.h
	$(CXX) -o $(OBJ_DIR)/StringUtilsTest.o -c $(CXXFLAGS) $(TEST_SRC_DIR)/StringUtilsTest.cpp

# StringDataSource
$(BIN_DIR)/teststrdatasource: $(OBJ_DIR)/StringDataSource.o $(OBJ_DIR)/StringDataSourceTest.o
	$(CXX) -o $@ $(OBJ_DIR)/StringDataSource.o $(OBJ_DIR)/StringDataSourceTest.o $(LDFLAGS)

$(OBJ_DIR)/StringDataSource.o: $(SRC_DIR)/StringDataSource.cpp $(INC_DIR)/StringDataSource.h
	$(CXX) -o $@ -c $(SRC_DIR)/StringDataSource.cpp $(CXXFLAGS)

$(OBJ_DIR)/StringDataSourceTest.o: $(TEST_SRC_DIR)/StringDataSourceTest.cpp $(INC_DIR)/StringDataSource.h
	$(CXX) -o $@ -c $(TEST_SRC_DIR)/StringDataSourceTest.cpp $(CXXFLAGS)

$(OBJ_DIR)/StringDataSource.o: $(SRC_DIR)/StringDataSource.cpp $(INC_DIR)/StringDataSource.h
	$(CXX) -o $@ -c $(SRC_DIR)/StringDataSource.cpp $(CXXFLAGS)

# StringDataSink
$(BIN_DIR)/teststrdatasink: $(OBJ_DIR)/StringDataSink.o $(OBJ_DIR)/StringDataSinkTest.o
	$(CXX) -o $@ $(OBJ_DIR)/StringDataSink.o $(OBJ_DIR)/StringDataSinkTest.o $(LDFLAGS)

$(OBJ_DIR)/StringDataSink.o: $(SRC_DIR)/StringDataSink.cpp $(INC_DIR)/StringDataSink.h
	$(CXX) -o $@ -c $(SRC_DIR)/StringDataSink.cpp $(CXXFLAGS)

$(OBJ_DIR)/StringDataSinkTest.o: $(TEST_SRC_DIR)/StringDataSinkTest.cpp $(INC_DIR)/StringDataSink.h
	$(CXX) -o $@ -c $(TEST_SRC_DIR)/StringDataSinkTest.cpp $(CXXFLAGS)

$(OBJ_DIR)/StringDataSink.o: $(SRC_DIR)/StringDataSink.cpp $(INC_DIR)/StringDataSink.h
	$(CXX) -o $@ -c $(SRC_DIR)/StringDataSink.cpp $(CXXFLAGS)

# DSV reader/writer
$(BIN_DIR)/testdsv: $(OBJ_DIR)/DSVReader.o $(OBJ_DIR)/DSVWriter.o $(OBJ_DIR)/StringDataSource.o $(OBJ_DIR)/StringDataSink.o $(OBJ_DIR)/DSVTest.o
	$(CXX) -o $@ $(OBJ_DIR)/DSVReader.o $(OBJ_DIR)/DSVWriter.o $(OBJ_DIR)/StringDataSource.o $(OBJ_DIR)/StringDataSink.o $(OBJ_DIR)/DSVTest.o $(LDFLAGS)

$(OBJ_DIR)/DSVReader.o: $(SRC_DIR)/DSVReader.cpp $(INC_DIR)/DSVReader.h
	$(CXX) -o $@ -c $(SRC_DIR)/DSVReader.cpp $(CXXFLAGS)

$(OBJ_DIR)/DSVWriter.o: $(SRC_DIR)/DSVWriter.cpp $(INC_DIR)/DSVWriter.h
	$(CXX) -o $@ -c $(SRC_DIR)/DSVWriter.cpp $(CXXFLAGS)

$(OBJ_DIR)/DSVTest.o: $(TEST_SRC_DIR)/DSVTest.cpp $(INC_DIR)/DSVReader.h $(INC_DIR)/DSVWriter.h
	$(CXX) -o $@ -c $(TEST_SRC_DIR)/DSVTest.cpp $(CXXFLAGS)

# XML reader/writer
$(BIN_DIR)/testxml: $(OBJ_DIR)/XMLReader.o $(OBJ_DIR)/XMLWriter.o $(OBJ_DIR)/StringDataSource.o $(OBJ_DIR)/StringDataSink.o $(OBJ_DIR)/XMLTest.o
	$(CXX) -o $@ $(OBJ_DIR)/XMLReader.o $(OBJ_DIR)/XMLWriter.o $(OBJ_DIR)/StringDataSource.o $(OBJ_DIR)/StringDataSink.o $(OBJ_DIR)/XMLTest.o $(LDFLAGS)

$(OBJ_DIR)/XMLReader.o: $(SRC_DIR)/XMLReader.cpp $(INC_DIR)/XMLReader.h
	$(CXX) -o $@ -c $(SRC_DIR)/XMLReader.cpp $(CXXFLAGS)

$(OBJ_DIR)/XMLWriter.o: $(SRC_DIR)/XMLWriter.cpp $(INC_DIR)/XMLWriter.h
	$(CXX) -o $@ -c $(SRC_DIR)/XMLWriter.cpp $(CXXFLAGS)

$(OBJ_DIR)/XMLTest.o: $(TEST_SRC_DIR)/XMLTest.cpp $(INC_DIR)/XMLReader.h $(INC_DIR)/XMLWriter.h
	$(CXX) -o $@ -c $(TEST_SRC_DIR)/XMLTest.cpp $(CXXFLAGS)

#CSVBus
$(BIN_DIR)/testcsvbs: $(OBJ_DIR)/CSVBusSystemTest.o $(OBJ_DIR)/CSVBusSystem.o $(OBJ_DIR)/StringDataSource.o $(OBJ_DIR)/DSVReader.o
	$(CXX) -o $@ $(OBJ_DIR)/CSVBusSystemTest.o $(OBJ_DIR)/CSVBusSystem.o $(OBJ_DIR)/StringDataSource.o $(OBJ_DIR)/DSVReader.o $(LDFLAGS)

$(OBJ_DIR)/CSVBusSystemTest.o: $(TEST_SRC_DIR)/CSVBusSystemTest.cpp $(INC_DIR)/CSVBusSystem.h $(INC_DIR)/BusSystem.h
	$(CXX) -o $@ -c $(TEST_SRC_DIR)/CSVBusSystemTest.cpp $(CXXFLAGS)

$(OBJ_DIR)/CSVBusSystem.o: $(SRC_DIR)/CSVBusSystem.cpp $(INC_DIR)/CSVBusSystem.h $(INC_DIR)/BusSystem.h
	$(CXX) -o $@ -c $(SRC_DIR)/CSVBusSystem.cpp $(CXXFLAGS)

#OSM
$(BIN_DIR)/testosm: $(OBJ_DIR)/OSMTest.o $(OBJ_DIR)/OpenStreetMap.o $(OBJ_DIR)/StringDataSource.o $(OBJ_DIR)/XMLReader.o
	$(CXX) -o $@ $(OBJ_DIR)/OSMTest.o $(OBJ_DIR)/OpenStreetMap.o $(OBJ_DIR)/StringDataSource.o $(OBJ_DIR)/XMLReader.o $(LDFLAGS)

$(OBJ_DIR)/OSMTest.o: $(TEST_SRC_DIR)/OSMTest.cpp $(INC_DIR)/StreetMap.h $(INC_DIR)/OpenStreetMap.h $(INC_DIR)/StreetMap.h
	$(CXX) -o $@ -c $(TEST_SRC_DIR)/OSMTest.cpp $(CXXFLAGS)

$(OBJ_DIR)/OpenStreetMap.o: $(SRC_DIR)/OpenStreetMap.cpp $(INC_DIR)/StreetMap.h $(INC_DIR)/OpenStreetMap.h
	$(CXX) -o $@ -c $(SRC_DIR)/OpenStreetMap.cpp $(CXXFLAGS)

# FileDataSource
$(OBJ_DIR)/FileDataSource.o: $(SRC_DIR)/FileDataSource.cpp $(INC_DIR)/FileDataSource.h
	$(CXX) -o $@ -c $(CXXFLAGS) $(SRC_DIR)/FileDataSource.cpp

# FileDataSink
$(OBJ_DIR)/FileDataSink.o: $(SRC_DIR)/FileDataSink.cpp $(INC_DIR)/FileDataSink.h
	$(CXX) -o $@ -c $(CXXFLAGS) $(SRC_DIR)/FileDataSink.cpp

# FileDataFactory
$(OBJ_DIR)/FileDataFactory.o: $(SRC_DIR)/FileDataFactory.cpp $(INC_DIR)/FileDataFactory.h
	$(CXX) -o $@ -c $(CXXFLAGS) $(SRC_DIR)/FileDataFactory.cpp

# FileDataSSTest
$(OBJ_DIR)/FileDataSSTest.o: $(TEST_SRC_DIR)/FileDataSSTest.cpp $(INC_DIR)/FileDataFactory.h
	$(CXX) -o $@ -c $(CXXFLAGS) $(TEST_SRC_DIR)/FileDataSSTest.cpp

# testfiledatass executable
$(BIN_DIR)/testfiledatass: $(OBJ_DIR)/FileDataSource.o $(OBJ_DIR)/FileDataSink.o $(OBJ_DIR)/FileDataFactory.o $(OBJ_DIR)/FileDataSSTest.o
	$(CXX) -o $@ $^ $(LDFLAGS)
# KMLWriter
$(OBJ_DIR)/KMLWriter.o: $(SRC_DIR)/KMLWriter.cpp $(INC_DIR)/KMLWriter.h
	$(CXX) -o $@ -c $(CXXFLAGS) $(SRC_DIR)/KMLWriter.cpp

# KMLTest
$(OBJ_DIR)/KMLTest.o: $(TEST_SRC_DIR)/KMLTest.cpp $(INC_DIR)/KMLWriter.h
	$(CXX) -o $@ -c $(CXXFLAGS) $(TEST_SRC_DIR)/KMLTest.cpp

# testkml executable
$(BIN_DIR)/testkml: $(OBJ_DIR)/KMLWriter.o $(OBJ_DIR)/KMLTest.o
	$(CXX) -o $@ $^ $(LDFLAGS)

# DijkstraPathRouter
$(OBJ_DIR)/DijkstraPathRouter.o: $(SRC_DIR)/DijkstraPathRouter.cpp $(INC_DIR)/DijkstraPathRouter.h
	$(CXX) -o $@ -c $(CXXFLAGS) $(SRC_DIR)/DijkstraPathRouter.cpp

# DijkstraPathRouterTest
$(OBJ_DIR)/DijkstraPathRouterTest.o: $(TEST_SRC_DIR)/DijkstraPathRouterTest.cpp $(INC_DIR)/DijkstraPathRouter.h
	$(CXX) -o $@ -c $(CXXFLAGS) $(TEST_SRC_DIR)/DijkstraPathRouterTest.cpp

# testdpr executable
$(BIN_DIR)/testdpr: $(OBJ_DIR)/DijkstraPathRouter.o $(OBJ_DIR)/DijkstraPathRouterTest.o
	$(CXX) -o $@ $^ $(LDFLAGS)

# BusSystemIndexer
$(OBJ_DIR)/BusSystemIndexer.o: $(SRC_DIR)/BusSystemIndexer.cpp $(INC_DIR)/BusSystemIndexer.h
	$(CXX) -o $@ -c $(CXXFLAGS) $(SRC_DIR)/BusSystemIndexer.cpp

# CSVBusSystemIndexerTest
$(OBJ_DIR)/CSVBusSystemIndexerTest.o: $(TEST_SRC_DIR)/CSVBusSystemIndexerTest.cpp $(INC_DIR)/BusSystemIndexer.h
	$(CXX) -o $@ -c $(CXXFLAGS) $(TEST_SRC_DIR)/CSVBusSystemIndexerTest.cpp

# testcsvbsi executable
$(BIN_DIR)/testcsvbsi: $(OBJ_DIR)/BusSystemIndexer.o $(OBJ_DIR)/CSVBusSystemIndexerTest.o
	$(CXX) -o $@ $^ $(LDFLAGS)

# TransportationPlannerCommandLine
$(OBJ_DIR)/TransportationPlannerCommandLine.o: $(SRC_DIR)/TransportationPlannerCommandLine.cpp $(INC_DIR)/TransportationPlannerCommandLine.h
	$(CXX) -o $@ -c $(CXXFLAGS) $(SRC_DIR)/TransportationPlannerCommandLine.cpp

# TPCommandLineTest
$(OBJ_DIR)/TPCommandLineTest.o: $(TEST_SRC_DIR)/TPCommandLineTest.cpp $(INC_DIR)/TransportationPlannerCommandLine.h
	$(CXX) -o $@ -c $(CXXFLAGS) $(TEST_SRC_DIR)/TPCommandLineTest.cpp

# testtpcl executable
$(BIN_DIR)/testtpcl: $(OBJ_DIR)/TransportationPlannerCommandLine.o $(OBJ_DIR)/TPCommandLineTest.o
	$(CXX) -o $@ $^ $(LDFLAGS)

# DijkstraTransportationPlanner
$(OBJ_DIR)/DijkstraTransportationPlanner.o: $(SRC_DIR)/DijkstraTransportationPlanner.cpp $(INC_DIR)/DijkstraTransportationPlanner.h
	$(CXX) -o $@ -c $(CXXFLAGS) $(SRC_DIR)/DijkstraTransportationPlanner.cpp

# CSVOSMTransportationPlannerTest
$(OBJ_DIR)/CSVOSMTransportationPlannerTest.o: $(TEST_SRC_DIR)/CSVOSMTransportationPlannerTest.cpp $(INC_DIR)/CSVOSMTransportationPlanner.h
	$(CXX) -o $@ -c $(CXXFLAGS) $(TEST_SRC_DIR)/CSVOSMTransportationPlannerTest.cpp

# testtp executable
$(BIN_DIR)/testtp: $(OBJ_DIR)/DijkstraTransportationPlanner.o $(OBJ_DIR)/CSVOSMTransportationPlannerTest.o # Add paths to other dependency object files here
	$(CXX) -o $@ $^ $(LDFLAGS)

clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

directories:
	mkdir -p $(OBJ_DIR) $(BIN_DIR)

