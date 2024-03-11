#include <memory> 
#include "TransportationPlannerCommandLine.h"
#include <iostream>
#include "GeographicUtils.h"
#include <sstream>
#include "StringDataSink.h"
#include "StringUtils.h"
#include "OpenStreetMap.h"
#include "BusSystem.h"
#include "StringDataSource.h"
#include "StandardDataSource.h"
#include "StandardDataSink.h" 

struct CTransportationPlannerCommandLine::SImplementation {
    std::shared_ptr<CDataSource> CmdSrc;
    std::shared_ptr<CDataSink> OutSink;
    std::shared_ptr<CDataSink> ErrSink;
    std::shared_ptr<CDataFactory> Results;
    std::shared_ptr<CTransportationPlanner> Planner;

    SImplementation(std::shared_ptr<CDataSource> cmdsrc,
                    std::shared_ptr<CDataSink> outsink,
                    std::shared_ptr<CDataSink> errsink,
                    std::shared_ptr<CDataFactory> results,
                    std::shared_ptr<CTransportationPlanner> planner)
        : CmdSrc(cmdsrc), OutSink(outsink), ErrSink(errsink),
          Results(results), Planner(planner) {}

    std::vector<std::string> ParseCommandLine(const std::string& line) {
        std::istringstream iss(line);
        std::vector<std::string> tokens;
        std::string token;
        while (iss >> token) {
            tokens.push_back(token);
        }
        return tokens;
    }

    // Adapted FindShortestPathCommand to include parsing and handling for all required commands
    void HandleCommand(const std::vector<std::string>& tokens) {
    if (tokens.empty()) return;

    const std::string& command = tokens[0];
    if (command == "help") {
        // Existing help command implementation
    } else if (command == "exit") {
        // Exit command does not need implementation here as it's handled in ProcessCommands()
    } else if (command == "count") {
        size_t nodeCount = Planner->NodeCount();
        OutSink->Write(std::to_string(nodeCount) + " nodes\n");
    } else if (command == "node") {
        if (tokens.size() < 2) {
            ErrSink->Write("Error: 'node' command requires an index argument.\n");
            return;
        }
        size_t index = std::stoul(tokens[1]);
        auto node = Planner->SortedNodeByIndex(index);
        if (node) {
            std::ostringstream nodeText;
            nodeText << "Node " << index << ": id = " << node->ID() << ", location = " << node->Location().first << ", " << node->Location().second << "\n";
            OutSink->Write(nodeText.str());
        } else {
            ErrSink->Write("Error: Invalid node index.\n");
        }
    } else if (command == "fastest" || command == "shortest") {
        if (tokens.size() < 3) {
            ErrSink->Write("Error: '" + command + "' command requires start and end node IDs.\n");
            return;
        }
        TNodeID src = std::stoul(tokens[1]);
        TNodeID dest = std::stoul(tokens[2]);
        if (command == "fastest") {
            std::vector<CTransportationPlanner::TTripStep> path;
            double time = Planner->FindFastestPath(src, dest, path);
            if (time >= 0) {
                OutSink->Write("Fastest path takes " + std::to_string(time) + " units of time.\n");
            } else {
                ErrSink->Write("No path exists.\n");
            }
        } else { // shortest
            std::vector<TNodeID> path;
            double distance = Planner->FindShortestPath(src, dest, path);
            if (distance >= 0) {
                OutSink->Write("Shortest path is " + std::to_string(distance) + " units of distance.\n");
            } else {
                ErrSink->Write("No path exists.\n");
            }
        }
    } else if (command == "save") {
        // Example implementation for 'save' - adjust according to your application
        if (path.empty()) {
            ErrSink->Write("No path to save.\n");
            return;
        }
        std::string filename = "last_path.txt";
        std::ofstream outFile(filename);
        if (!outFile) {
            ErrSink->Write("Error opening file for saving.\n");
            return;
        }
        for (auto& step : path) {
            outFile << "Step: Mode = " << std::to_string(static_cast<int>(step.first)) << ", Node ID = " << step.second << "\n";
        }
        outFile.close();
        OutSink->Write("Path saved to " + filename + "\n");
    } else if (command == "print") {
        // Example implementation for 'print' - adjust according to your application
        if (path.empty()) {
            ErrSink->Write("No path to print.\n");
            return;
        }
        for (auto& step : path) {
            OutSink->Write("Step: Mode = " + std::to_string(static_cast<int>(step.first)) + ", Node ID = " + std::to_string(step.second) + "\n");
        }
    } else {
        ErrSink->Write("Error: Unknown command '" + command + "'\n");
    }
}

    bool ProcessCommands() {
        std::string command_line;
        while (CmdSrc->Read(command_line)) {
            auto tokens = ParseCommandLine(command_line);
            if (!tokens.empty() && tokens[0] == "exit") {
                break;
            }
            HandleCommand(tokens);
        }
        return true;
    }
};

CTransportationPlannerCommandLine::CTransportationPlannerCommandLine(std::shared_ptr<CDataSource> cmdsrc,
                                                                     std::shared_ptr<CDataSink> outsink,
                                                                     std::shared_ptr<CDataSink> errsink,
                                                                     std::shared_ptr<CDataFactory> results,
                                                                     std::shared_ptr<CTransportationPlanner> planner)
    : DImplementation(new SImplementation(cmdsrc, outsink, errsink, results, planner)) {}

CTransportationPlannerCommandLine::~CTransportationPlannerCommandLine() {}

bool CTransportationPlannerCommandLine::ProcessCommands() {
    return DImplementation->ProcessCommands();
}



