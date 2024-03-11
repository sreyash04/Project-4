#include "DijkstraTransportationPlanner.h"
#include "TransportationPlannerConfig.h"
#include "TransportationPlanner.h"
#include "TransportationPlannerCommandLine.h"
#include "DijkstraPathRouter.h"
#include "GeographicUtils.h"
#include "BusSystemIndexer.h"
#include "CSVBusSystem.h"
#include "BusSystem.h"
#include <unordered_map>
#include <algorithm>
#include <iostream>
#include <cmath>
#include <sstream>
#include <fstream> 

using TNodeID = CStreetMap::TNodeID;


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
    std::vector<CTransportationPlanner::TTripStep> path;
    std::vector<TNodeID> path2;
    const std::string& command = tokens[0];
    if (command == "help") {
        // Existing help command implementation
    } else if (command == "exit") {
        // Exit command does not need implementation here as it's handled in ProcessCommands()
    } else if (command == "count") {
        size_t nodeCount = Planner->NodeCount();
        std::string message = std::to_string(nodeCount) + " nodes\n";
        OutSink->Write(std::vector<char>(message.begin(), message.end()));
        
    } else if (command == "node") {
        if (tokens.size() < 2) {
            // Example adjustment for error messages
            std::string errorMessage = "Error: 'node' command requires an index argument.\n";
            ErrSink->Write(std::vector<char>(errorMessage.begin(), errorMessage.end()));
            return;
        }
        size_t index = std::stoul(tokens[1]);
        auto node = Planner->SortedNodeByIndex(index);
        if (node) {
            std::ostringstream nodeText;
            nodeText << "Node " << index << ": id = " << node->ID() << ", location = " << node->Location().first << ", " << node->Location().second << "\n";
            std::string message = nodeText.str();
            OutSink->Write(std::vector<char>(message.begin(), message.end()));
        } else {
            std::string errorMessage = "Error: Invalid node index.\n";
            ErrSink->Write(std::vector<char>(errorMessage.begin(), errorMessage.end()));
        }
    } else if (command == "fastest" || command == "shortest") {
        if (tokens.size() < 3) {
            std::string errorMessage ="Error: '" + command + "' command requires start and end node IDs.\n";
            ErrSink->Write(std::vector<char>(errorMessage.begin(), errorMessage.end()));
            return;
        }
        auto src = std::stoul(tokens[1]);
        auto dest = std::stoul(tokens[2]);
        if (command == "fastest") {
            // std::vector<CTransportationPlanner::TTripStep> path;
            double time = Planner->FindFastestPath(src, dest, path);
            if (time >= 0) {
                std::string outputMessage = "Fastest path takes " + std::to_string(time) + " units of time.\n";
                OutSink->Write(std::vector<char>(outputMessage.begin(), outputMessage.end()));
            } else {
                std::string errorMessage ="No path exists.\n";
                ErrSink->Write(std::vector<char>(errorMessage.begin(), errorMessage.end()));
            }
        } else { // shortest
            // std::vector<TNodeID> path;
            double distance = Planner->FindShortestPath(src, dest, path2);
            if (distance >= 0) {
    
                std::string message = "Shortest path is " + std::to_string(distance) + " units of distance.\n";
                OutSink->Write(std::vector<char>(message.begin(), message.end()));
                
                
            } else {
                std::string errorMessage ="No path exists.\n";
                ErrSink->Write(std::vector<char>(errorMessage.begin(), errorMessage.end()));
            }
        }
    } else if (command == "save") {
        // Example implementation for 'save' - adjust according to your application
        if (path.empty() && path2.empty()) {
            std::string errorMessage ="No path to save.\n";
            ErrSink->Write(std::vector<char>(errorMessage.begin(), errorMessage.end()));
            return;
        }
        std::string filename = "last_path.txt";
        std::ofstream outFile(filename);
        if (!outFile) {
            std::string errorMessage ="Error opening file for saving.\n";
            ErrSink->Write(std::vector<char>(errorMessage.begin(), errorMessage.end()));
            return;
        }
        for (auto& step : path) {
            outFile << "Step: Mode = " << std::to_string(static_cast<int>(step.first)) << ", Node ID = " << step.second << "\n";
        }
        outFile.close();
        std::string pathMessage = "Path saved to " + filename + "\n";
        OutSink->Write(std::vector<char>(pathMessage.begin(), pathMessage.end()));
    } else if (command == "print") {
        // Example implementation for 'print' - adjust according to your application
        if (path.empty()) {
            std::string errorMessage ="No path to print\n";
            ErrSink->Write(std::vector<char>(errorMessage.begin(), errorMessage.end()));
            return;
        }
        for (auto& step : path) {
            
            std::string message = "Step: Mode = " + std::to_string(static_cast<int>(step.first)) + ", Node ID = " + std::to_string(step.second) + "\n";
            OutSink->Write(std::vector<char>(message.begin(), message.end()));

        }
    } else {
        
        std::string errorMessage ="Error: Unknown command '" + command + "'\n";
        ErrSink->Write(std::vector<char>(errorMessage.begin(), errorMessage.end()));
    }
}

    bool ProcessCommands() {
        std::vector<char> command_line;
        while (CmdSrc->Read(command_line, 5)) {
            std::string str(command_line.begin(), command_line.end());
            auto tokens = ParseCommandLine(str);
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
