# Class Description
The CBusSystemIndexer class provides an efficient way to index and query a bus system's data, including stops and routes. It leverages sorting and hashing techniques to quickly access stops and routes by various identifiers and relationships.

## Constructor and Destructor
CBusSystemIndexer Constructor
Purpose: Constructs a CBusSystemIndexer instance, initializing it with a shared pointer to a CBusSystem object. This setup allows the indexer to build internal data structures for quick access to stops and routes.

Parameters:

std::shared_ptr<CBusSystem> bussystem: A shared pointer to a CBusSystem object from which stop and route data will be indexed.
CBusSystemIndexer Destructor
Purpose: Destroys the CBusSystemIndexer instance, performing necessary cleanup of resources.

Documented Methods
### StopCount Method
Purpose: Retrieves the total number of stops indexed in the bus system.

Parameters: None

Return Type: std::size_t

Returns the number of stops in the system.
### RouteCount Method
Purpose: Retrieves the total number of routes indexed in the bus system.

 Parameters: None

Return Type: std::size_t

Returns the number of routes in the system.
### SortedStopByIndex Method
Purpose: Retrieves a shared pointer to a stop object by its index in the system, where stops are sorted by their IDs.

Parameters:

std::size_t index: The index of the stop in the sorted list.
Return Type: std::shared_ptr<CBusSystem::SStop>

Returns a shared pointer to the stop object if the index is valid; otherwise, returns nullptr.
### SortedRouteByIndex Method
Purpose: Retrieves a shared pointer to a route object by its index in the system, where routes are sorted by their names.

Parameters:

std::size_t index: The index of the route in the sorted list.
Return Type: std::shared_ptr<CBusSystem::SRoute>

Returns a shared pointer to the route object if the index is valid; otherwise, returns nullptr.
### StopByNodeID Method
Purpose: Retrieves a shared pointer to a stop object by its node ID.

Parameters:

TNodeID id: The node ID of the stop.
Return Type: std::shared_ptr<CBusSystem::SStop>

Returns a shared pointer to the stop object if the node ID exists; otherwise, returns nullptr.
### RoutesByNodeIDs Method
Purpose: Retrieves the set of routes that directly connect two nodes identified by their node IDs.

Parameters:

TNodeID src: The source node ID.
TNodeID dest: The destination node ID.
std::unordered_set<std::shared_ptr<SRoute>>& routes: A reference to an unordered set that will be populated with the routes connecting the source and destination nodes.
Return Type: bool

Returns true if one or more routes are found; otherwise, returns false.
RouteBetweenNodeIDs Method
Purpose: Checks if there is at least one route connecting two nodes identified by their node IDs.

### Parameters:

TNodeID src: The source node ID.
TNodeID dest: The destination node ID.
Return Type: bool

Returns true if at least one route exists between the source and destination nodes; otherwise, returns false.

## Example Usage
```
 #include "BusSystemIndexer.h"
#include "BusSystem.h"
#include <iostream>
#include <memory>

int main() {
    // Assume 'busSystem' is an instance of CBusSystem initialized with appropriate data sources.

    // Create a CBusSystemIndexer instance with a shared pointer to the bus system.
    std::shared_ptr<CBusSystem> busSystem = std::make_shared<CBusSystem>();
    CBusSystemIndexer busSystemIndexer(busSystem);

    // Retrieve and print the total number of stops and routes in the system.
    std::cout << "Total number of stops: " << busSystemIndexer.StopCount() << std::endl;
    std::cout << "Total number of routes: " << busSystemIndexer.RouteCount() << std::endl;

    // Example: Retrieve and print information about a stop by its sorted index.
    std::size_t stopIndex = 0;
    auto stop = busSystemIndexer.SortedStopByIndex(stopIndex);
    if (stop) {
        std::cout << "Stop ID: " << stop->ID << ", Node ID: " << stop->NodeID << std::endl;
    } else {
        std::cout << "Invalid stop index." << std::endl;
    }

    // Example: Retrieve and print information about a route by its sorted index.
    std::size_t routeIndex = 0;
    auto route = busSystemIndexer.SortedRouteByIndex(routeIndex);
    if (route) {
        std::cout << "Route Name: " << route->Name << ", Number of Stops: " << route->StopCount() << std::endl;
    } else {
        std::cout << "Route not found." << std::endl;
    }

    return 0;
}
```