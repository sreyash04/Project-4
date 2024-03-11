# Project-4
ECS 34 - Project 4
------------------------
STUDENT 1: Arnav Suden 918550383

STUDENT 2: Sreyash Ravinuthala 920484337

#Code References:

[gtest syntax](https://github.com/google/googletest/blob/main/googletest/samples/sample6_unittest.cc)

[pimpl implementation](https://www.geeksforgeeks.org/pimpl-idiom-in-c-with-examples/)

[$@ in makefile](https://stackoverflow.com/questions/3220277/what-do-the-makefile-symbols-and-mean)

[Makefile syntax](https://web.stanford.edu/class/archive/cs/cs107/cs107.1174/guide_make.html)

[C++ switch](https://www.w3schools.com/cpp/cpp_switch.asp)

PROJECT STATUS
Everything works need to test out TransportationCommandLine.cpp file and have attempted the extra credit Get Path Description, and tried to optimize DijkstraPathRouter
------------------------
KNOWN ISSUES
GetPathDescription doesnt fully work
------------------------
GENERATIVE AI USE CHAT GPT
------------------------
PROMT 1
Asked Chat gpt why am I getting out of bound error
My promt // std::string Slice(const std::string &str, ssize_t start, ssize_t end) noexcept{
//     return str.substr(start,str.length()-start);
// }

RESPONSE 1
std::string Slice(const std::string &str, ssize_t start, ssize_t end) noexcept {
    // Adjust negative indices
    ssize_t strLen = static_cast<ssize_t>(str.length());
    if (start < 0) start = strLen + start;
    if (end <= 0) end = strLen + end;

    // Ensure start and end are within bounds
    start = std::max(start, static_cast<ssize_t>(0));
    end = std::min(end, strLen);

    if (start > end) return ""; // Return empty string if start is beyond end

    ssize_t len = end - start;
    return str.substr(start, len);
}
------------------------------
PROMT 2
Asked chat gpt to fix error in my previous expand tabs
My promt 

RESPONSE 2
std::string ExpandTabs(const std::string &str, int tabsize) noexcept{
    std::string result = str;
    std::string::size_type pos = 0;
    while ((pos = result.find('\t', pos)) != std::string::npos) {
        int spaces = tabsize - (pos % tabsize);
        result.replace(pos, 1, std::string(spaces, ' '));
        pos += spaces;
    }
    return result;
}
--------------------------------
PROMT 3
Asked Chat gpt how to 
I have latitude and longitude as follows: lat="38.5178523" lon="-121.7712408" how do I convert it to a format like: 38d 23' 60\" N, 121d 43' 12\" W"

RESPONSE 3
To convert latitude and longitude from decimal degrees to degrees, minutes, and seconds (DMS) format in C++, you can use the following code snippet. This code will also handle the direction (N, S, E, W) based on whether the coordinates are positive or negative:

```cpp
#include <iostream>
#include <cmath> // For floor function

void convertDMS(double degrees, char type) {
    // Determine the direction based on the type (latitude or longitude) and the value
    char direction = ' ';
    if (type == 'L') { // Latitude
        direction = (degrees >= 0) ? 'N' : 'S';
    } else if (type == 'O') { // Longitude
        direction = (degrees >= 0) ? 'E' : 'W';
    }
    degrees = fabs(degrees); // Ensure the degrees are positive for calculation

    // Calculate degrees, minutes, and seconds
    int d = static_cast<int>(degrees);
    double minutesDecimal = (degrees - d) * 60;
    int m = static_cast<int>(minutesDecimal);
    double s = (minutesDecimal - m) * 60;

    // Output the result
    std::cout << d << "d " << m << "' " << std::fixed << s << "\" " << direction << std::endl;
}

int main() {
    double latitude = 38.5178523;
    double longitude = -121.7712408;

    std::cout << "Latitude: ";
    convertDMS(latitude, 'L');

    std::cout << "Longitude: ";
    convertDMS(longitude, 'O');

    return 0;
}
```

This code defines a `convertDMS` function that converts decimal degrees to DMS format, handling both latitude and longitude. The `type` parameter (`'L'` for latitude and `'O'` for longitude) is used to determine the appropriate direction (N, S, E, W) based on the sign of the decimal degree value. The `main` function demonstrates using this function to convert specific latitude and longitude values.
-----------------------------------
PROMT 4
standard sort algo for vector in cpp
RESPONSE 4
In C++, the standard sorting algorithm for vectors and other containers is provided by the <algorithm> header, specifically through the std::sort function. This function is highly efficient and adaptable, being able to sort almost any type of data for which a comparison is defined.
Here is a simple usage example for sorting a std::vector<int>:
cpp

Copy code
#include <algorithm> // For std::sort
#include <vector>
#include <iostream>

int main() {
    std::vector<int> vec = {4, 1, 3, 5, 2};

    // Sorting in ascending order
    std::sort(vec.begin(), vec.end());

    // Print the sorted vector
    for(int i : vec) {
        std::cout << i << ' ';
    }
    std::cout << std::endl;

    return 0;
}
In this example, std::sort sorts the elements of vec in ascending order. By default, std::sort uses the operator < to compare elements, so the elements of the vector need to support this operator or you need to provide a custom comparison function.
If you want to sort in descending order or according to some other criteria, you can pass a comparison function as the third argument to std::sort:
cpp

Copy code
std::sort(vec.begin(), vec.end(), std::greater<int>()); // For descending order
Or, for custom sorting criteria:
cpp

Copy code
std::sort(vec.begin(), vec.end(), [](int a, int b) {
    return a > b; // Example of custom sorting criterion (descending order)
});
The underlying algorithm of std::sort is not specified by the C++ standard; it is left to the implementation. However, most implementations use introsort, which is a hybrid sorting algorithm combining quicksort, heapsort, and insertion sort, allowing for both fast average performance and optimal worst-case performance.
-----------------------------------
PROMT 5
Asked Chat gpt why am I getting out of bound error
My promt // std::string Slice(const std::string &str, ssize_t start, ssize_t end) noexcept{
//     return str.substr(start,str.length()-start);
// }
RESPONSE 5
What chat gpt gave 
std::string Slice(const std::string &str, ssize_t start, ssize_t end) noexcept {
    // Adjust negative indices
    ssize_t strLen = static_cast<ssize_t>(str.length());
    if (start < 0) start = strLen + start;
    if (end <= 0) end = strLen + end;

    // Ensure start and end are within bounds
    start = std::max(start, static_cast<ssize_t>(0));
    end = std::min(end, strLen);

    if (start > end) return ""; // Return empty string if start is beyond end

    ssize_t len = end - start;
    return str.substr(start, len);
}
----------------------------
PROMT 6
Asked chat gpt to fix error in my previous expand tabs
My promt 
What chat gpt gave 
RESPONSE 6
std::string ExpandTabs(const std::string &str, int tabsize) noexcept{
    std::string result = str;
    std::string::size_type pos = 0;
    while ((pos = result.find('\t', pos)) != std::string::npos) {
        int spaces = tabsize - (pos % tabsize);
        result.replace(pos, 1, std::string(spaces, ' '));
        pos += spaces;
    }
    return result;
}
-------------------------------------------
PROMT 7
Asked chat gpt // Gave my code to chat gpt asked why am I getting segmentation error and asked it to add segmentation tags if required
RESPONSE 7
What Chat Gpt Gave 
  if (src == CPathRouter::InvalidVertexID || dest == CPathRouter::InvalidVertexID) {
        std::cerr << "Invalid vertex ID detected in path request." << std::endl;
        return CPathRouter::NoPathExists;
    }

    // Check if the source and destination are identical
    if (src == dest) {
        std::cerr << "Source equals destination: " << src << std::endl;
        path = {src}; // Directly assign the source to the path, clearing any previous contents
        return 0.0; // The distance for identical start and end points is zero
    }
----------------------------------------
PROMT 8
Gave Chat Gpt my code for findshortest path and find fastest path and asked it to fix bugs 
What I gave chat gpt
// double FindShortestPath(TNodeID src, TNodeID dest, std::vector< TNodeID > &path){
    //     std::vector< CPathRouter::TVertexID > ShortestPath; // compare node id to vector id // create a struct
    //     // put in some checks for this
    //     auto SourceVertexID = DNodeToVertexID[src];
    //     auto DestinationVertexID = DNodeToVertexID[dest];
    //     auto Distance = DShortestPathRouter.FindShortestPath(SourceVertexID,DestinationVertexID,ShortestPath);
    //     path.clear();
    //     for(auto VertexID : ShortestPath){
    //         path.push_back(std::any_cast< TNodeID >(DShortestPathRouter.GetVertexTag(VertexID)));
    //     }
    //     return Distance;
    // }
RESPONSE 8
double FindShortestPath(TNodeID src, TNodeID dest, std::vector<TNodeID> &path) {
        if (DNodeToVertexID.find(src) == DNodeToVertexID.end() || DNodeToVertexID.find(dest) == DNodeToVertexID.end()) {
            return CPathRouter::NoPathExists;
        }
        std::vector<CPathRouter::TVertexID> VertexPath;
        auto SourceVertexID = DNodeToVertexID[src];
        auto DestinationVertexID = DNodeToVertexID[dest];
        double Distance = DShortestPathRouter.FindShortestPath(SourceVertexID, DestinationVertexID, VertexPath);
        if (Distance == CPathRouter::NoPathExists) {
            return CPathRouter::NoPathExists;
        }
        path.clear();
        for (auto VertexID : VertexPath) {
            path.push_back(std::any_cast<TNodeID>(DShortestPathRouter.GetVertexTag(VertexID)));
        }
        return Distance;
    }
-----------------------------------
PROMT 9
What I gave chat gpt to fix
What I gave chat gpt 
// double FindFastest(TNodeID src, TNodeID dest, std::vector< TTripStep > &path){
    //     std::vector< CPathRouter::TVertexID > ShortestPathBike;
    //     std::vector< CPathRouter::TVertexID > ShortestPathBusWalk;
       

    //     auto SourceVertexID = DNodeToVertexID[src];
    //     auto DestinationVertexID = DNodeToVertexID[dest];

    //     auto DistanceBike = DShortestPathRouter.FindShortestPath(SourceVertexID,DestinationVertexID,ShortestPathBike);
    //     auto DistanceBusWalk = DShortestPathRouter.FindShortestPath(SourceVertexID,DestinationVertexID,ShortestPathBusWalk);

    //     auto Distance = 0.0;

    //     path.clear();

    //     if(DistanceBike < DistanceBusWalk){
    //         Distance = DistanceBike;
    //         for(auto VertexID : ShortestPathBike){
    //             auto x = std::any_cast< TNodeID >(DShortestPathRouter.GetVertexTag(VertexID));
    //             TTripStep pair = {CTransportationPlanner::ETransportationMode::Bike, x};
    //             path.push_back(pair);
    //         }
    //     }
    //     else{
    //         Distance = DistanceBusWalk;
    //         for(auto i = 0; i < ShortestPathBusWalk.size() - 1; i++){
    //             auto x_VID = ShortestPathBusWalk[i];
    //             auto y_VID = ShortestPathBusWalk[i+1];
    //             auto x = std::any_cast< TNodeID >(DShortestPathRouter.GetVertexTag(x_VID));
    //             auto y = std::any_cast< TNodeID >(DShortestPathRouter.GetVertexTag(y_VID));
    //             if(BusSystemIndex->RouteBetweenNodeIDs(x, y)){
    //                 TTripStep pair = {CTransportationPlanner::ETransportationMode::Bus, x};
    //                 path.push_back(pair);
    //             }
    //             else{
    //                 TTripStep pair = {CTransportationPlanner::ETransportationMode::Walk, x};
    //                 path.push_back(pair);
    //             }
    //         }
    //         // for bus/walk depending on which we take
    //     }


    //     return Distance;
    //     // have to do two searches search the bike path and search the walking plus bus and just take the faster of the two the lowest time of the two
    // }

RESPONSE 9
 double FindFastest(TNodeID src, TNodeID dest, std::vector< TTripStep > &path) {
    if (DNodeToVertexID.find(src) == DNodeToVertexID.end() || DNodeToVertexID.find(dest) == DNodeToVertexID.end()) {
        return CPathRouter::NoPathExists;
    }

    std::vector<CPathRouter::TVertexID> PathBike, PathWalkBus;
    auto SourceVertexID = DNodeToVertexID[src];
    auto DestinationVertexID = DNodeToVertexID[dest];
    double DistanceBike = DFastestPathRouterBike.FindShortestPath(SourceVertexID, DestinationVertexID, PathBike);
    double DistanceWalkBus = DFastestPathRouterWalkBus.FindShortestPath(SourceVertexID, DestinationVertexID, PathWalkBus);

    double Distance = CPathRouter::NoPathExists;
    path.clear();

    if (DistanceBike < DistanceWalkBus && DistanceBike != CPathRouter::NoPathExists) {
        Distance = DistanceBike;
        for (auto VertexID : PathBike) {
            TNodeID NodeID = std::any_cast<TNodeID>(DFastestPathRouterBike.GetVertexTag(VertexID));
            path.push_back({ETransportationMode::Bike, NodeID});
        }
    } else if (DistanceWalkBus != CPathRouter::NoPathExists) {
        Distance = DistanceWalkBus;
        TNodeID previousNodeID = std::any_cast<TNodeID>(DFastestPathRouterWalkBus.GetVertexTag(PathWalkBus.front()));
        for (size_t i = 0; i < PathWalkBus.size(); ++i) {
            TNodeID currentNodeID = std::any_cast<TNodeID>(DFastestPathRouterWalkBus.GetVertexTag(PathWalkBus[i]));
            ETransportationMode mode = ETransportationMode::Walk; // Default to walk
            if (BusSystemIndex->RouteBetweenNodeIDs(previousNodeID, currentNodeID)) {
                mode = ETransportationMode::Bus;
            }
            path.push_back({mode, currentNodeID});
            previousNodeID = currentNodeID;
        }
    }

    return Distance;
}
-----------------------------------------------------------------
### Prompt 1:

This is what the command line.h file looks like "#ifndef TRANSPORTATIONPLANNERCOMMANDLINE_H
#define TRANSPORTATIONPLANNERCOMMANDLINE_H

#include "DataFactory.h"
#include "TransportationPlanner.h"
#include <memory>
#include <vector>

class CTransportationPlannerCommandLine{
    private:
        struct SImplementation;
        std::unique_ptr<SImplementation> DImplementation;
    public:
        CTransportationPlannerCommandLine(std::shared_ptr<CDataSource> cmdsrc, std::shared_ptr<CDataSink> outsink, std::shared_ptr<CDataSink> errsink, std::shared_ptr<CDataFactory> results, std::shared_ptr<CTransportationPlanner> planner);
        ~CTransportationPlannerCommandLine();
        bool ProcessCommands();
};

#endif
" Could you provide me with a skelton structure for TransportationPlannerCommandLine.cpp

### Response 1:

Key Points for Implementation:
SImplementation Struct: This struct will hold all data members and methods that are part of the CTransportationPlannerCommandLine's implementation. This typically includes resources like data sources, data sinks, data factories, and the transportation planner itself.

Constructor and Destructor: The constructor will initialize the DImplementation unique pointer with instances of required resources. The destructor will take care of automatically cleaning up these resources when an instance of CTransportationPlannerCommandLine is destroyed.

ProcessCommands Method: This method will implement the logic to read commands from the command source, parse them, and then dispatch them to the appropriate handler methods that utilize the CTransportationPlanner and other resources to perform actions.

TransportationPlannerCommandLine.cpp Implementation Sketch
cpp
Copy code
#include "TransportationPlannerCommandLine.h"
#include "DataSource.h"
#include "DataSink.h"
// Include other necessary headers...

struct CTransportationPlannerCommandLine::SImplementation {
    std::shared_ptr<CDataSource> CommandSource;
    std::shared_ptr<CDataSink> OutputSink;
    std::shared_ptr<CDataSink> ErrorSink;
    std::shared_ptr<CDataFactory> Results;
    std::shared_ptr<CTransportationPlanner> Planner;

    // Constructor for SImplementation
    SImplementation(std::shared_ptr<CDataSource> cmdsrc, std::shared_ptr<CDataSink> outsink, std::shared_ptr<CDataSink> errsink, std::shared_ptr<CDataFactory> results, std::shared_ptr<CTransportationPlanner> planner)
    : CommandSource(std::move(cmdsrc)), OutputSink(std::move(outsink)), ErrorSink(std::move(errsink)), Results(std::move(results)), Planner(std::move(planner)) {}

    // Methods for command processing...
    void ProcessCommand(const std::string& command) {
        // Implement command processing logic here
        // For example, parse the command and call the corresponding method on the Planner
    }
};

CTransportationPlannerCommandLine::CTransportationPlannerCommandLine(std::shared_ptr<CDataSource> cmdsrc, std::shared_ptr<CDataSink> outsink, std::shared_ptr<CDataSink> errsink, std::shared_ptr<CDataFactory> results, std::shared_ptr<CTransportationPlanner> planner)
: DImplementation(std::make_unique<SImplementation>(std::move(cmdsrc), std::move(outsink), std::move(errsink), std::move(results), std::move(planner))) {}

CTransportationPlannerCommandLine::~CTransportationPlannerCommandLine() = default;

bool CTransportationPlannerCommandLine::ProcessCommands() {
    std::string command;
    // Assume CommandSource provides a way to read commands as strings
    while (DImplementation->CommandSource->ReadCommand(command)) {
        DImplementation->ProcessCommand(command);
        // You might want to handle errors or invalid commands
    }
    return true; // or false based on command processing success/failure
}

### Changes 1

Providing me with a starting ground for the command line file and an idea of where I must start.


### Prompt 2:

I am finding trouble with the bool bool "CTransportationPlannerCommandLine::ProcessCommands(){
    st::string command;
}" could you find the errors in the code and help fix it 


### Response 2:
bool CTransportationPlannerCommandLine::ProcessCommands() {
    std::string command;
    while (DImplementation->CommandSource->ReadCommand(command)) {
        std::istringstream commandStream(command);
        std::string action, subAction;
        commandStream >> action >> subAction;
        if (action == "find") {
            TNodeID src, dest;
            commandStream >> src >> dest;
            if (subAction == "shortest") {
                // Assume DImplementation->Planner has a method like FindShortestPath(src, dest)
                auto result = DImplementation->Planner->FindShortestPath(src, dest);
                DImplementation->OutputSink->Write("Shortest path result: " + result);
            } else if (subAction == "fastest") {
                // Similar to FindShortestPath, for fastest path
                auto result = DImplementation->Planner->FindFastestPath(src, dest);
                DImplementation->OutputSink->Write("Fastest path result: " + result);
            } else {
                DImplementation->ErrorSink->Write("Invalid sub-action command.");
            }
        } else {
            DImplementation->ErrorSink->Write("Unknown action command.");
        }
    }
    return true; // Or false if any command processing failed
}
This implementation assumes:

CDataSource::ReadCommand reads a command into the command string.
Commands are structured in a way that the first word specifies the action (e.g., "find"), and the second specifies the sub-action (e.g., "shortest" or "fastest").
TNodeID represents the node ID type, and src and dest are extracted from the command.
FindShortestPath and FindFastestPath are methods provided by CTransportationPlanner, returning results that can be directly written to CDataSink.
You will need to adjust this implementation to match the actual commands your application supports and the real interfaces of the involved components.

### Changes 2:

The implemtaiton of the commands is supossed to deal with Parsing Commands , Executing Commands and Handling the erros . I fixed the the fucntion and added these methods of implemtnation my file.

********************************

