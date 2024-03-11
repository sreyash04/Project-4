## CTransportationPlanner Documentation
CTransportationPlanner is an abstract class designed to orchestrate transportation planning, utilizing various modes of transport within a geographical area defined by a street map and a bus system. It provides interfaces for calculating the shortest and fastest paths between points, considering walking, biking, and bus riding as possible modes of transportation.

Public Types and Enums
TNodeID: Alias for CStreetMap::TNodeID, representing a unique identifier for nodes (points or intersections) within the street map.

ETransportationMode: Enum class representing the different modes of transportation available in the planner. Includes Walk, Bike, and Bus.

TTripStep: Represents a single step in a trip, pairing a mode of transportation with a node ID to indicate moving through that node by the specified transportation mode.

SConfiguration Struct
SConfiguration is a nested struct within CTransportationPlanner that defines a configuration interface for the transportation planner. It provides access to essential components and parameters needed for planning, such as the street map, bus system, and various speeds.

## Virtual Destructor
~SConfiguration(): Ensures proper cleanup of derived configuration instances.
Pure Virtual Functions
StreetMap() const noexcept: Returns a shared pointer to a CStreetMap instance representing the geographical area's map.

BusSystem() const noexcept: Returns a shared pointer to a CBusSystem instance for accessing bus routes and schedules.

WalkSpeed() const noexcept: Returns the average walking speed (in meters per second) used for planning.

BikeSpeed() const noexcept: Returns the average biking speed (in meters per second) used for planning.

DefaultSpeedLimit() const noexcept: Returns the default speed limit (in meters per second) applied where specific speed limits are not available.

BusStopTime() const noexcept: Returns the average time (in seconds) spent at each bus stop during boarding and alighting.

PrecomputeTime() const noexcept: Returns the time allowed (in seconds) for precomputation or optimization tasks before route calculations.

## Destructor
~CTransportationPlanner(): Virtual destructor to allow derived classes to clean up resources properly.
Pure Virtual Functions
NodeCount() const noexcept: Returns the total number of nodes (points or intersections) within the street map.

SortedNodeByIndex(std::size_t index) const noexcept: Returns a shared pointer to a CStreetMap::SNode object representing the node at the specified index, ordered in some consistent manner.

FindShortestPath(TNodeID src, TNodeID dest, std::vector<TNodeID> &path): Calculates the shortest path between the source and destination nodes. Populates path with the sequence of node IDs representing this path and returns the total distance.

FindFastestPath(TNodeID src, TNodeID dest, std::vector<TTripStep> &path): Calculates the fastest path between the source and destination nodes, considering different transportation modes. Populates path with a sequence of TTripStep elements representing this path and returns the total time taken.

GetPathDescription(const std::vector<TTripStep> &path, std::vectorstd::string &desc) const: Converts a given path (sequence of TTripStep elements) into a descriptive string format, populating desc with a readable description of each step in the path.

## Usage
CTransportationPlanner is designed to be extended by concrete classes that implement the transportation planning logic specific to different geographical areas, routing algorithms, and transportation networks. Instantiation and use of this class require a derived class that implements all pure virtual functions, providing specific functionalities for transportation planning tasks.