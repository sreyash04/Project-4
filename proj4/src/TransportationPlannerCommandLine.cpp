#ifndef CTRANSPORTATIONPLANNERCOMMANDLINE_H
#define CTRANSPORTATIONPLANNERCOMMANDLINE_H

#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>
#include "DataFactory.h"
#include "DataSource.h"
#include "DataSink.h"
#include "TransportationPlanner.h" // Ensure you have this header defined

class CTransportationPlannerCommandLine {
// public:
//     CTransportationPlannerCommandLine(std::shared_ptr<CDataSource> cmdsrc,
//                                       std::shared_ptr<CDataSink> outsink,
//                                       std::shared_ptr<CDataSink> errsink,
//                                       std::shared_ptr<CDataFactory> results,
//                                       std::shared_ptr<CTransportationPlanner> planner)
//     : cmdSource(cmdsrc), outSink(outsink), errSink(errsink), resultFactory(results), planner(planner) {}

//     ~CTransportationPlannerCommandLine() {}

//     bool ProcessCommands() {
//         std::vector<char> line;
//         char ch;
//         while (!cmdSource->End()) {
//             line.clear();
//             while (cmdSource->Get(ch) && ch != '\n') {
//                 line.push_back(ch);
//             }
//             line.push_back('\0'); // Null-terminate the string

//             std::istringstream iss(std::string(line.begin(), line.end()));
//             std::string command;
//             iss >> command;

//             if (command == "plan") {
//                 std::string source, destination;
//                 iss >> source >> destination;
//                 if (!source.empty() && !destination.empty()) {
//                     // Assuming planner->PlanRoute is a method to plan the route
//                     auto result = planner->PlanRoute(source, destination);
//                     if (result) {
//                         // Assuming resultFactory->CreateResult() method to format the result
//                         auto formattedResult = resultFactory->CreateResult(result);
//                         // Convert formattedResult to vector<char>
//                         std::vector<char> outBuf(formattedResult.begin(), formattedResult.end());
//                         outSink->Write(outBuf);
//                     } else {
//                         std::vector<char> errMsg = {'E', 'r', 'r', 'o', 'r', ':', ' ', 'F', 'a', 'i', 'l', 'e', 'd', ' ', 't', 'o', ' ', 'p', 'l', 'a', 'n', ' ', 'r', 'o', 'u', 't', 'e', '.'};
//                         errSink->Write(errMsg);
//                     }
//                 } else {
//                     std::vector<char> errMsg = {'E', 'r', 'r', 'o', 'r', ':', ' ', 'I', 'n', 'v', 'a', 'l', 'i', 'd', ' ', 'p', 'l', 'a', 'n', ' ', 'c', 'o', 'm', 'm', 'a', 'n', 'd', ' ', 'f', 'o', 'r', 'm', 'a', 't', '.'};
//                     errSink->Write(errMsg);
//                 }
//             } else if (command == "exit") {
//                 break; // Exit the loop and end the program
//             } else {
//                 std::vector<char> errMsg = {'E', 'r', 'r', 'o', 'r', ':', ' ', 'U', 'n', 'k', 'n', 'o', 'w', 'n', ' ', 'c', 'o', 'm', 'm', 'a', 'n', 'd', '.'};
//                 errSink->Write(errMsg);
//             }
//         }

//         return true;
//     }

// private:
//     std::shared_ptr<CDataSource> cmdSource;
//     std::shared_ptr<CDataSink> outSink;
//     std::shared_ptr<CDataSink> errSink;
//     std::shared_ptr<CDataFactory> resultFactory;
//     std::shared_ptr<CTransportationPlanner> planner;
};

#endif // CTRANSPORTATIONPLANNERCOMMANDLINE_H
