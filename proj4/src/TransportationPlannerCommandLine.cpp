#include "TransportationPlannerCommandLine.h"
#include <iostream>
#include <sstream>
#include <string>

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

    // Helper function to parse a command line
    std::vector<std::string> ParseCommandLine(const std::string& line) {
        std::istringstream iss(line);
        std::vector<std::string> tokens;
        std::string token;
        while (iss >> token) {
            tokens.push_back(token);
        }
        return tokens;
    }

    // Process a 'find shortest path' command
    void FindShortestPathCommand(const std::vector<std::string>& tokens) {
        if (tokens.size() != 3) {
            ErrSink->Write("Error: Expected 2 arguments for 'shortest_path'\n");
            return;
        }
        TNodeID src = std::stoull(tokens[1]);
        TNodeID dest = std::stoull(tokens[2]);
        std::vector<TNodeID> path;
        double path_length = Planner->FindShortestPath(src, dest, path);
        if (path_length < 0) {
            OutSink->Write("No path exists\n");
        } else {
            OutSink->Write("Shortest path length: " + std::to_string(path_length) + "\n");
            // Optionally: output the actual path
        }
    }

    // Process commands read from the command source
    bool ProcessCommands() {
        std::string command_line;
        while (CmdSrc->Read(command_line)) {
            auto tokens = ParseCommandLine(command_line);
            if (tokens.empty()) continue;
            if (tokens[0] == "exit") {
                break;
            } else if (tokens[0] == "shortest_path") {
                FindShortestPathCommand(tokens);
            } else {
                ErrSink->Write("Error: Unknown command '" + tokens[0] + "'\n");
            }
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