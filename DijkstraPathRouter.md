## Class Description
The CDijkstraPathRouter class implements the Dijkstra algorithm for finding the shortest path between vertices in a graph. It is designed to handle transportation planning scenarios, where vertices represent points of interest (e.g., bus stops, stations) and edges represent connections between these points with associated weights (e.g., travel time, distance).

## Constructor and Destructor
CDijkstraPathRouter Constructor
Purpose: Constructs a CDijkstraPathRouter instance, initializing internal structures for vertex and edge management.

Parameters: None

CDijkstraPathRouter Destructor
Purpose: Destroys the CDijkstraPathRouter instance, performing necessary cleanup of resources.

### Documented Methods

## VertexCount Method
Purpose: Retrieves the total number of vertices in the path router.

Parameters: None

Return Type: std::size_t

Returns the number of vertices.

## AddVertex Method
Purpose: Adds a new vertex to the path router with an associated tag.

Parameters:

std::any tag: An arbitrary tag associated with the vertex. This tag can be used to store additional information about the vertex, such as its name or location.
Return Type: TVertexID

Returns the ID of the newly added vertex.

## GetVertexTag Method
Purpose: Retrieves the tag associated with a given vertex ID.

Parameters:

TVertexID id: The ID of the vertex whose tag is to be retrieved.
Return Type: std::any

Returns the tag associated with the vertex. If the vertex ID is invalid, returns an empty std::any object.

## AddEdge Method
Purpose: Adds an edge between two vertices in the path router.

Parameters:

TVertexID src: The ID of the source vertex.
TVertexID dest: The ID of the destination vertex.
double weight: The weight of the edge (e.g., travel time, distance).
bool bidir (optional): If set to true, an additional edge from dest to src is also added, making the connection bidirectional.
Return Type: bool

Returns true if the edge is successfully added; otherwise, returns false.

## Precompute Method
Purpose: Allows the path router to perform any desired precomputation up to a specified deadline. This can be used to optimize pathfinding operations.

Parameters:

std::chrono::steady_clock::time_point deadline: The deadline by which precomputation must be completed.
Return Type: bool

Returns true if precomputation is successful; otherwise, returns false.

## FindShortestPath Method
Purpose: Finds the shortest path from a source vertex to a destination vertex and returns the path distance.

Parameters:

TVertexID src: The ID of the source vertex.
TVertexID dest: The ID of the destination vertex.
std::vector<TVertexID>& path: A reference to a vector that will be filled with the IDs of the vertices along the shortest path, in order.
Return Type: double

Returns the path distance of the shortest path from src to dest. If no path exists, returns CPathRouter::NoPathExists.

## Example Usage

cpp
Copy code
#include "DijkstraTransportationPlanner.h"
#include <iostream>
#include <vector>

int main() {
    CDijkstraPathRouter router;

    // Add vertices and edges to the router
    auto vertexA = router.AddVertex("A");
    auto vertexB = router.AddVertex("B");
    auto vertexC = router.AddVertex("C");

    router.AddEdge(vertexA, vertexB, 5.0);
    router.AddEdge(vertexB, vertexC, 3.0);
    router.AddEdge(vertexC, vertexA, 2.0, true); // Bidirectional edge

    // Find the shortest path from A to C
    std::vector<CDijkstraPathRouter::TVertexID> path;
    double distance = router.FindShortestPath(vertexA, vertexC, path);

    if (distance != CDijkstraPathRouter::NoPathExists) {
        std::cout << "Shortest path distance: " << distance << std::endl;
        std::cout << "Path: ";
        for (auto vertexID : path) {
            std::any tag = router.GetVertexTag(vertexID);
            if (tag.has_value()) {
                std::cout << std::any_cast<std::string>(tag) << " ";
            }
        }
        std::cout << std::endl;
    } else {
        std::cout << "No path exists." << std::endl;
    }

    return 0;
}