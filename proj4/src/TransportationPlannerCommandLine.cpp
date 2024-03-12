#include "DijkstraTransportationPlanner.h"
#include "TransportationPlannerConfig.h"
#include "TransportationPlanner.h"
#include "TransportationPlannerCommandLine.h"
#include "DijkstraPathRouter.h"
#include "GeographicUtils.h"
#include "BusSystemIndexer.h"
#include "CSVBusSystem.h"
#include "BusSystem.h"
#include "DSVReader.h"
#include <unordered_map>
#include <algorithm>
#include <iostream>
#include <cmath>
#include <sstream>
#include <fstream> 
#include <vector>
#include <iomanip>

using TNodeID = CStreetMap::TNodeID;


struct CTransportationPlannerCommandLine::SImplementation {
    std::shared_ptr<CDataSource> CmdSrc;
    std::shared_ptr<CDataSink> OutSink;
    std::shared_ptr<CDataSink> ErrSink;
    std::shared_ptr<CDataFactory> Results;
    std::shared_ptr<CTransportationPlanner> Planner;
    double time_global = 0.0;

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

    std::string convertDMS1(double degrees, char type) const{
        std::string str = "";
    // Determine the direction based on the type (latitude or longitude) and the value
        std::string direction = "";
        if (type == 'L') { // Latitude
            direction = (degrees >= 0) ? "N" : "S";
        } else if (type == 'O') { // Longitude
            direction = (degrees >= 0) ? "E" : "W";
        }
        degrees = fabs(degrees); // Ensure the degrees are positive for calculation

        // Calculate degrees, minutes, and seconds
        int d = static_cast<int>(degrees);
        double minutesDecimal = (degrees - d) * 60;
        int m = static_cast<int>(minutesDecimal);
        int s = static_cast<int>((minutesDecimal - m) * 60);

        std::string ds = std::to_string(d);
        std::string ms = std::to_string(m);
        std::string ss = std::to_string(s);

        str.append(ds);
        str.append("d ");
        str.append(ms);
        str.append("' ");
        str.append(ss);
        str.append("\" ");
        str.append(direction);

        return str;  
}

std::string convertToHrsMinSec(double totalMinutes) {
    // Calculate hours, minutes, and seconds
    totalMinutes = totalMinutes * 60;
    int hours = totalMinutes / 60;
    totalMinutes = totalMinutes - 60 * hours;
    int minutes = std::floor(totalMinutes);
    totalMinutes = totalMinutes - minutes;
    int seconds = totalMinutes * 60; // This will always be 0 since totalMinutes is an integer

    // Create the resulting string

    std::string result = "";
    if(hours != 0){
        result.append(std::to_string(hours));
        result.append(" hr ");
    }
    result.append(std::to_string(minutes));
    result.append(" min");
    if(seconds != 0){
        result.append(" ");
        result.append(std::to_string(seconds));
        result.append(" sec");
    }

    

    return result;
}

    // Adapted FindShortestPathCommand to include parsing and handling for all required commands
    void HandleCommand(const std::vector<std::string>& tokens) {
    if (tokens.empty()) return;
    //help text adding here 
    const std::string& command = tokens[0];
    std::vector<CTransportationPlanner::TTripStep> path;
    std::vector<TNodeID> path2;
    if (command == "help") { 
        
       std::string helpText = 
                                    "------------------------------------------------------------------------\n"
                                    "help     Display this help menu\n"
                                    "exit     Exit the program\n"
                                    "count    Output the number of nodes in the map\n"
                                    "node     Syntax \"node [0, count)\" \n"
                                    "         Will output node ID and Lat/Lon for node\n"
                                    "fastest  Syntax \"fastest start end\" \n"
                                    "         Calculates the time for fastest path from start to end\n"
                                    "shortest Syntax \"shortest start end\" \n"
                                    "         Calculates the distance for the shortest path from start to end\n"
                                    "save     Saves the last calculated path to file\n"
                                    "print    Prints the steps for the last calculated path\n"
                                    "> ";
                                
        OutSink->Write(std::vector<char>(helpText.begin(), helpText.end()));
        return;
        }
    //const std::string& command = tokens[0];
   else if (command == "exit") {
        // Exit command does not need implementation here as it's handled in ProcessCommands()
    } else if (command == "count") {
        size_t nodeCount = Planner->NodeCount();

        std::string message = std::to_string(nodeCount) + " nodes\n> ";
        OutSink->Write(std::vector<char>(message.begin(), message.end()));
        
    } else if (command == "node") {
        if (tokens.size() < 2) {
            // Example adjustment for error messages
            std::string errorMessage = "Error: 'node' command requires an index argument.\n> ";
            ErrSink->Write(std::vector<char>(errorMessage.begin(), errorMessage.end()));
            return;
        }// index here
        size_t index = std::stoul(tokens[1]);
        auto node = Planner->SortedNodeByIndex(index);
        if (node) {
            std::string nodeText;
            //nodeText << "> \n"<< "Node " << index << ": id = " << node->ID() << ", location = " << node->Location().first << ", " << node->Location().second << "\n" << "< ";
            nodeText = "Node " + std::to_string(index) + ": id = " +std::to_string(node->ID()) + " is at " + convertDMS1((node->Location().first), 'L') + ", " + convertDMS1((node->Location().second), 'O') +"\n> ";
            //std::string message = nodeText.str();
            OutSink->Write(std::vector<char>(nodeText.begin(), nodeText.end()));
        } else {
            std::string errorMessage = "Error: Invalid node index.\n> ";
            ErrSink->Write(std::vector<char>(errorMessage.begin(), errorMessage.end()));
        }// checking for fastest or shortest
    } else if (command == "fastest" || command == "shortest") {
        if (tokens.size() < 3) {
            std::string errorMessage ="Error: '" + command + "' command requires start and end node IDs.\n> ";
            ErrSink->Write(std::vector<char>(errorMessage.begin(), errorMessage.end()));
            return;
        }//declaring for tokens
        auto src = std::stoul(tokens[1]);
        auto dest = std::stoul(tokens[2]);
        if (command == "fastest") {
            // std::vector<CTransportationPlanner::TTripStep> path;
            double time = Planner->FindFastestPath(src, dest, path);
            time_global = time;
            std::string t = convertToHrsMinSec(time);
            // int t = static_cast<int>(time);
            if (time >= 0) {
                std::string outputMessage = "Fastest path takes " + t + ".\n> ";
                OutSink->Write(std::vector<char>(outputMessage.begin(), outputMessage.end()));
            } else {
                std::string errorMessage ="No path exists.\n> ";
                ErrSink->Write(std::vector<char>(errorMessage.begin(), errorMessage.end()));
            }
        } else { // shortest
            // std::vector<TNodeID> path;
            double distance = Planner->FindShortestPath(src, dest, path2);
            if (distance >= 0) {
    
                std::ostringstream stream;
                stream << std::fixed << std::setprecision(1) << distance;
                std::string message = "Shortest path is " + stream.str() + " mi.\n> ";
                OutSink->Write(std::vector<char>(message.begin(), message.end()));
                
                
            } else {
                std::string errorMessage ="No path exists.\n> ";
                ErrSink->Write(std::vector<char>(errorMessage.begin(), errorMessage.end()));
            }
        }
    } else if (command == "save") {
        // Example implementation for 'save' - adjust according to your application
        if (path.empty() && path2.empty()) {
            std::string errorMessage ="No path to save.\n> ";
            ErrSink->Write(std::vector<char>(errorMessage.begin(), errorMessage.end()));
            return;
        }
        std::string filename = tokens[1] + "_" + tokens[2] + "_" + std::to_string(time_global) + "hr.csv";
        std::ofstream outFile(filename);
        if (!outFile) {
            std::string errorMessage ="Error opening file for saving.\n> ";
            ErrSink->Write(std::vector<char>(errorMessage.begin(), errorMessage.end()));
            return;
        }
        for (auto& step : path) {
            outFile << "Step: Mode = " << std::to_string(static_cast<int>(step.first)) << ", Node ID = " << step.second << "\n";
        }
        outFile.close();
        std::string pathMessage = "\nPath saved to " + filename + "\n> ";
        OutSink->Write(std::vector<char>(pathMessage.begin(), pathMessage.end()));
    } else if (command == "print") {
        // Example implementation for 'print' - adjust according to your application
        if (path.empty()) {
            std::string errorMessage ="No path to print\n> ";
            ErrSink->Write(std::vector<char>(errorMessage.begin(), errorMessage.end()));
            return;
        }
        for (auto& step : path) {
            
            std::string message = "Step: Mode = " + std::to_string(static_cast<int>(step.first)) + ", Node ID = " + std::to_string(step.second) + "\n";
            OutSink->Write(std::vector<char>(message.begin(), message.end()));

        }
    } else if(command == "error") {
        
        std::string errorMessage ="Error: Unknown command '" + command + "'\n> ";
        ErrSink->Write(std::vector<char>(errorMessage.begin(), errorMessage.end()));
    }
}

    bool ProcessCommands() {
        //added here 
        std::string initialPrompt = "> ";
        OutSink->Write(std::vector<char>(initialPrompt.begin(), initialPrompt.end()));

        auto command_line = CmdSrc;
        CDSVReader reader(CmdSrc, '\n');
        std::vector<std::string> row;
        reader.ReadRow(row);

        for(int i = 0; i < row.size(); i++){
            auto tokens = ParseCommandLine(row[i]);
            // if (tokens.empty() && tokens[0] == "exit") {
            //     return false;
            // }
            HandleCommand(tokens);
        }
        //while (CmdSrc->Read(command_line, 5)) {
            // std::string str(command_line);
            // auto tokens = ParseCommandLine(str);
            // if (!tokens.empty() && tokens[0] == "exit") {
            //     return false;
            // }
            // HandleCommand(tokens);
       // }
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
