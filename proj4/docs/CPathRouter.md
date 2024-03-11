## Class Description
The CPathRouter class serves as an abstract base class for implementing various path routing algorithms. It defines a common interface for adding vertices and edges to a graph, tagging vertices, and finding the shortest path between vertices. This class is designed to be extended by concrete implementations that specify particular pathfinding algorithms, such as Dijkstra's algorithm.

## Constants
InvalidVertexID: A constant representing an invalid vertex ID. This is used to signify when a requested vertex does not exist within the graph.
NoPathExists: A constant representing the absence of a path between two vertices. It is returned by pathfinding methods when no path can be found.
Destructor
virtual ~CPathRouter()
A virtual destructor to ensure proper cleanup of derived classes.

Pure Virtual Methods

## VertexCount
Purpose: Returns the number of vertices in the graph.

Parameters: None

Return Type: std::size_t

The total number of vertices.

## AddVertex
Purpose: Adds a vertex to the graph and associates it with a user-defined tag.

Parameters:

std::any tag: An arbitrary object to associate with the vertex. This can be used to store additional information about the vertex.
Return Type: TVertexID

The ID of the newly added vertex.

## GetVertexTag
Purpose: Retrieves the tag associated with a given vertex.

Parameters:

TVertexID id: The ID of the vertex whose tag is to be retrieved.
Return Type: std::any

The tag associated with the vertex. If the vertex ID is invalid, returns an empty std::any object.

## AddEdge
Purpose: Adds an edge between two vertices in the graph.

Parameters:

TVertexID src: The ID of the source vertex.
TVertexID dest: The ID of the destination vertex.
double weight: The weight of the edge.
bool bidir (optional): If set to true, an additional edge from dest to src is added, making the connection bidirectional.
Return Type: bool

Returns true if the edge is successfully added; otherwise, returns false.

## Precompute

Purpose: Allows the implementation to perform any necessary precomputation before pathfinding queries.

Parameters:

std::chrono::steady_clock::time_point deadline: The deadline by which precomputation must be completed.
Return Type: bool

Returns true if precomputation is successful; otherwise, returns false.

## FindShortestPath

Purpose: Finds the shortest path between two vertices in the graph.

Parameters:

TVertexID src: The ID of the source vertex.
TVertexID dest: The ID of the destination vertex.
std::vector<TVertexID>& path: A reference to a vector that will be populated with the IDs of the vertices along the shortest path, in order.
Return Type: double

The total weight of the shortest path. If no path exists, NoPathExists is returned.

## Example Usage

class CDijkstraRouter : public CPathRouter {
    // Implementation of CPathRouter using Dijkstra's algorithm
    ...
};

int main() {
    std::shared_ptr<CPathRouter> router = std::make_shared<CDijkstraRouter>();

    // Add vertices and edges
    auto v1 = router->AddVertex("Start");
    auto v2 = router->AddVertex("End");
    router->AddEdge(v1, v2, 1.0);

    // Find the shortest path
    std::vector<CPathRouter::TVertexID> path;
    double distance = router->FindShortestPath(v1, v2, path);

    if (distance != CPathRouter::NoPathExists) {
        // Process the path
    }
}
