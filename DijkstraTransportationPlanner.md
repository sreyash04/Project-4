## Class Description

The CDijkstraTransportationPlanner class provides comprehensive functionalities for planning transportation routes. It integrates various modes of transport, including walking, biking, and bus travel, by utilizing Dijkstra's algorithm for pathfinding. This planner accounts for different parameters such as distance, travel time, and transportation modes, offering the shortest and fastest routes between points in a street map.

## Constructor and Destructor

CDijkstraTransportationPlanner Constructor
Purpose: Constructs a CDijkstraTransportationPlanner instance, initializing it with configuration settings and data sources for street maps and bus systems.

Parameters:

std::shared_ptr<SConfiguration> config: A shared pointer to a configuration object that provides access to street maps, bus systems, and other essential resources.
CDijkstraTransportationPlanner Destructor
Purpose: Destroys the CDijkstraTransportationPlanner instance, performing necessary cleanup of resources.

Documented Methods

## NodeCount Method

Purpose: Retrieves the total number of nodes (e.g., intersections, points of interest) in the street map.

Parameters: None

Return Type: std::size_t

Returns the number of nodes.

## SortedNodeByIndex Method

Purpose: Retrieves a node from the street map by its sorted index.

Parameters:

std::size_t index: The index of the node in the sorted list.
Return Type: std::shared_ptr<CStreetMap::SNode>

Returns a shared pointer to the node if the index is valid; otherwise, returns nullptr.

## FindShortestPath Method
Purpose: Finds the shortest path between two nodes based on distance.

Parameters:

TNodeID src: The ID of the source node.
TNodeID dest: The ID of the destination node.
std::vector<TNodeID>& path: A reference to a vector that will be populated with the IDs of the nodes along the shortest path, in order.
Return Type: double

Returns the distance of the shortest path from src to dest. If no path exists, a specific constant (e.g., NoPathExists) is returned.

## FindFastestPath Method
Purpose: Determines the fastest route between two nodes considering various transportation modes (e.g., walking, biking, bus).

Parameters:

TNodeID src: The ID of the source node.
TNodeID dest: The ID of the destination node.
std::vector<TTripStep>& path: A reference to a vector that will be populated with the trip steps, including transportation modes and node IDs, along the fastest route.
Return Type: double

Returns the total travel time of the fastest route from src to dest. If no route is available, a specific constant (e.g., NoPathExists) is returned.

## GetPathDescription Method
Purpose: Generates a human-readable description of a given path, detailing the sequence of travel steps and the corresponding geographic coordinates in a formatted string.

Parameters:

const std::vector<TTripStep>& path: A vector containing the trip steps, including transportation modes and node IDs, of a route.
std::vector<std::string>& desc: A reference to a vector that will be populated with strings describing each step of the path.
Return Type: bool

Returns true if the description is successfully generated; otherwise, returns false.

## Example Usage
This example is a general illustration of how to use the CDijkstraTransportationPlanner to find and describe a route. The actual usage will depend on the specific implementation details of the planner and the configuration.


#include "DijkstraTransportationPlanner.h"
#include <vector>
#include <iostream>

int main() {
    // Initialize the transportation planner with configuration
    std::shared_ptr<SConfiguration> config = std::make_shared<SConfiguration>();
    CDijkstraTransportationPlanner planner(config);

    TNodeID sourceID = 1; // Example source node ID
    TNodeID destinationID = 10; // Example destination node ID
    std::vector<TNodeID> shortestPath;
    std::vector<TTripStep> fastestPath;
    std::vector<std::string> pathDescription;

    // Find the shortest path based on distance
    double distance = planner.FindShortestPath(sourceID, destinationID, shortestPath);
    std::cout << "Shortest distance: " << distance << std::endl;

    // Find the fastest path considering multiple modes of transportation
    double travelTime = planner.FindFastestPath(sourceID, destinationID, fastestPath);
    std::cout << "Fastest travel time: " << travelTime << std::endl;

    // Get a human-readable description of the fastest path
    if (planner.GetPathDescription(fastestPath, pathDescription)) {
        for (const auto& step : pathDescription) {
            std::cout << step << std::endl;
        }
    }

    return 0;
}