#include "TransportationPlannerCommandLine.h"
#include <iostream>
#include <sstream>
#include <vector>
#include <string>

CTransportationPlannerCommandLine::CTransportationPlannerCommandLine(
    std::shared_ptr<CDataSource> cmdsrc,
    std::shared_ptr<CDataSink> outsink,
    std::shared_ptr<CDataSink> errsink,
    std::shared_ptr<CDataFactory> results,
    std::shared_ptr<CTransportationPlanner> planner)
    : cmdSource(cmdsrc), outSink(outsink), errSink(errsink),
      resultFactory(results), planner(planner) {}

CTransportationPlannerCommandLine::~CTransportationPlannerCommandLine() {}

bool CTransportationPlannerCommandLine::ProcessCommands() {
    // Example command processing loop. In a real scenario, commands would be read from cmdSource
    std::string input;
    std::cout << "Enter command (or 'exit' to quit): ";
    while (std::getline(std::cin, input) && input != "exit") {
        ExecuteCommand(input);
        std::cout << "Enter command (or 'exit' to quit): ";
    }

    return true;
}

void CTransportationPlannerCommandLine::ExecuteCommand(const std::string& command) {
    if (command == "help") {
        DisplayHelp();
    } else {
        // Placeholder: Add code to parse and execute other commands here
        outSink->Write("Executing: " + command + "\n");
        // Example: planner->FindShortestPath(src, dest, &path);
    }
}

void CTransportationPlannerCommandLine::DisplayHelp() const {
    std::stringstream ss;
    ss << "Available commands:\n";
    ss << "help - Displays this help message\n";
    ss << "[Other commands specific to your application]\n";
    outSink->Write(ss.str());
}

// Define other methods as needed
