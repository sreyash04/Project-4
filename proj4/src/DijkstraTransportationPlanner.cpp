#include "DijkstraTransportationPlanner.h"
#include "TransportationPlannerConfig.h"
#include "TransportationPlanner.h"
#include "DijkstraPathRouter.h"
#include "GeographicUtils.h"
#include "BusSystemIndexer.h"
#include "CSVBusSystem.h"
#include "BusSystem.h"
#include <unordered_map>
#include <algorithm>
#include <iostream>
#include <cmath>  // For std::sort

struct CDijkstraTransportationPlanner::SImplementation{
    std::shared_ptr< CStreetMap > DStreetMap;
    std::shared_ptr< CBusSystem > DBusSystem;
    std::unordered_map< CStreetMap::TNodeID, CPathRouter::TVertexID > DNodeToVertexID;
    // Path routers for different routing strategies (shortest path, fastest by bike, fastest by walking and bus)
    CDijkstraPathRouter DShortestPathRouter; // we want locals one for the shortest path pone for biking and one for walking plus bus
    CDijkstraPathRouter DFastestPathRouterBike;
    CDijkstraPathRouter DFastestPathRouterWalkBus;
    std::vector<std::shared_ptr<CStreetMap::SNode>> Sorted;
    std::shared_ptr< SGeographicUtils > GeoUtils = std::make_shared<SGeographicUtils>();
    //std::shared_ptr< CBusSystemIndexer > BusSystemIndex  = std::make_shared<CBusSystemIndexer>();
    //std::shared_ptr<CBusSystemIndexer> BusSystemIndex = std::make_shared<CBusSystemIndexer>(DBusSystem);
    // std::shared_ptr< STransportationPlannerConfig > Trans_plan = std::make_shared<STransportationPlannerConfig>();
    std::shared_ptr<CBusSystemIndexer> BusSystemIndex;
    double speed = 25.0;
    std::string convertDMS(double degrees, char type) const{
        std::string str = "";
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
        str + direction;

        return str;  
}



    SImplementation(std::shared_ptr<CTransportationPlanner::SConfiguration> config){
        // std::shared_ptr< STransportationPlannerConfig > Trans_plan = std::make_shared<STransportationPlannerConfig>();
        auto transConfig = std::dynamic_pointer_cast<STransportationPlannerConfig>(config);
        if (!transConfig) {
            throw std::runtime_error("Configuration cast failed.");
        }

        DStreetMap = transConfig->StreetMap(); // prof
        DBusSystem = transConfig->BusSystem(); //prof
        BusSystemIndex = std::make_shared<CBusSystemIndexer>(DBusSystem);


        for(size_t Index = 0; Index < DStreetMap->NodeCount(); Index++){
            auto Node = DStreetMap->NodeByIndex(Index);
            //Sorted.push_back(Node);
            // std::sort(Sorted.begin(), Sorted.end(), [](const std::shared_ptr<CStreetMap::SNode>& a, const std::shared_ptr<CStreetMap::SNode>& b) {
            // return a->ID() < b->ID(); // Assuming each SNode has an ID method for comparison
            // });//changed here ------------------
            auto VertexID = DShortestPathRouter.AddVertex(Node->ID());// add a vertex and label it by its node id
            // here there is only the shortest path router we should add them to fastest to the two and do the ecaxt same thing and assume they will be using the same vertex id
            DFastestPathRouterBike.AddVertex(Node->ID());
            DFastestPathRouterWalkBus.AddVertex(Node->ID());
            DNodeToVertexID[Node->ID()] = VertexID;
            Sorted.push_back(Node); //


        }
        std::sort(Sorted.begin(), Sorted.end(), [](const std::shared_ptr<CStreetMap::SNode>& a, const std::shared_ptr<CStreetMap::SNode>& b) {
            return a->ID() < b->ID(); // Assuming each SNode has an ID method for comparison
            });//changed here ------------------
            
        // now go to edges to calculate
        for(size_t Index = 0; Index < DStreetMap->WayCount();Index++){
            auto Way = DStreetMap->WayByIndex(Index);
            // asumme bicyle
            bool Bikeable = Way->GetAttribute("bicycle") != "no";
            bool Bidirectional = Way->GetAttribute("oneway") != "yes";
            auto PreviousNodeID = Way->GetNodeID(0);

            if(Way->HasAttribute("maxspeed")){
                speed = stoi(Way->GetAttribute("maxspeed"));
            }

            for(size_t NodeIndex = 1; NodeIndex < Way->NodeCount();NodeIndex++){
                // we will get the 
                auto NextNodeID = Way->GetNodeID(NodeIndex);

                auto PreviousNode = DStreetMap->NodeByID(PreviousNodeID);
                auto NextNode = DStreetMap->NodeByID(NextNodeID);

                auto PreviousNode_VertextID = DNodeToVertexID[PreviousNodeID];
                auto NextNode_VertexID = DNodeToVertexID[NextNodeID];

                auto PreviousNode_Loc = PreviousNode->Location();
                auto NextNode_Loc = NextNode->Location();

                auto H_Dist = GeoUtils->HaversineDistanceInMiles(PreviousNode_Loc, NextNode_Loc);
                
                DShortestPathRouter.AddEdge(PreviousNode_VertextID, NextNode_VertexID, H_Dist, Bidirectional);
                // Populating DShortestPathRouter Edges based on Bidirectional or not
                // if(Bidirectional){
                //     DShortestPathRouter.AddEdge(PreviousNode_VertextID, NextNode_VertexID, H_Dist);
                //     DShortestPathRouter.AddEdge(NextNode_VertexID, PreviousNode_VertextID, H_Dist);
                // }
                // else{
                //     DShortestPathRouter.AddEdge(PreviousNode_VertextID, NextNode_VertexID, H_Dist);
                // }

                // Populating DFastestPatRouterBike Edges based on Bidirectional or not & Bikeable
                // if(Bikeable & !Bidirectional){
                //     auto biketime = H_Dist/(transConfig->BikeSpeed());
                //     DFastestPathRouterBike.AddEdge(PreviousNode_VertextID, NextNode_VertexID, biketime);
                // }
                // else if(Bikeable & Bidirectional){
                //     auto biketime = H_Dist/(transConfig->BikeSpeed());
                //     DFastestPathRouterBike.AddEdge(PreviousNode_VertextID, NextNode_VertexID, biketime);
                //     DFastestPathRouterBike.AddEdge(NextNode_VertexID, PreviousNode_VertextID, biketime);
                // }

                auto biketime = H_Dist/(transConfig->BikeSpeed());
                if(Bikeable){
                    DFastestPathRouterBike.AddEdge(PreviousNode_VertextID, NextNode_VertexID, biketime, Bidirectional);
                }

                // if(BusSystemIndex->RouteBetweenNodeIDs(PreviousNodeID, NextNodeID)){
                //     auto walktime = H_Dist/(transConfig->WalkSpeed());

                //     auto bustime = H_Dist/(transConfig->DefaultSpeedLimit()); // ***** how to get speed limit *****

                //     auto total_bustime = bustime + transConfig->BusStopTime();

                //     if(total_bustime < walktime){
                //         DFastestPathRouterWalkBus.AddEdge(PreviousNode_VertextID, NextNode_VertexID, total_bustime);
                //     }
                //     else{
                //         DFastestPathRouterWalkBus.AddEdge(PreviousNode_VertextID, NextNode_VertexID, walktime);
                //     }
                // }



                //trying from here 
                // DFastestPathRouterWalkBus.AddEdge(PreviousNode_VertextID, NextNode_VertexID, total_bustime);

                //what weight to put in for the edges in DFastestPathRouterBusWalk


                // now we have the node ids and we can translate the node ids to vertex ids and then we can add an edge based on the weight
                // to get the weight we have to get the location of the two nodes
                // so if we have the location of the two nodes we can use Haversindistance formula 15:26
                // in geographic utils.cpp there is a haversindistancemiles if you have two nodes you get the location
                // and if you pass them as location one and location two this will give you the distance in miles
                // for the shortest path you are just using the distance and for the fastest path you are just dividing by the speed that you are moving along
                // the bike speed or the walk in transportationplanner.h will get you time, distance/speed = time
                // so then you can have the weights be all in time for the fastest path then you are just relying on Dijkstras to solve it for you
                // if speed limit is not specified use the default speed limit and that is only for the bus
                // bike speed is specified and walk speed is specified, busstoptime is the time you are spending at eacg stop
                // //you are adding the busstoptime + how long taking to drive so for every edge you add the
                // bus stop time when you add the busses in 
                // for each edge its the whole time like half of it is when i got on and half when i got off the bus
                // if you are traversing multiple steps you are sitting on the bus on the whole time
                //
                // just add the bus stop time
                // for the walking and the bus basically you create the walking fastest path and then
                // for the bus edges basically what you do is find the shortest path for the bus and to drive
                // between each of the stops and calculate how long that will take so just add an edege 
                // directly between the two nodes 
                // just use the shortest path for the bus, the busses will follow the shortest path
                //this is where you will build up the shortest path graph and use it to find the shortest path
                PreviousNodeID = NextNodeID;
            }
        }
// calculate using time
        for(size_t Index = 0; Index < DBusSystem->RouteCount(); Index++){
            auto Route = DBusSystem->RouteByIndex(Index);
            auto PreviousStopID = Route->GetStopID(0);
            auto PreviousStop = DBusSystem->StopByID(PreviousStopID);
            auto PreviousNodeID = PreviousStop->NodeID();
            for(size_t NodeIndex = 1; NodeIndex < Route->StopCount(); NodeIndex++){
                auto NextStopID = Route->GetStopID(NodeIndex);
                auto NextStop = DBusSystem->StopByID(NextStopID);
                auto NextNodeID = NextStop->NodeID();
                auto PreviousNode_VertextID = DNodeToVertexID[PreviousNodeID];
                auto NextNode_VertexID = DNodeToVertexID[NextNodeID];
                auto H_Dist = GeoUtils->HaversineDistanceInMiles(DStreetMap->NodeByID(PreviousNodeID)->Location(), DStreetMap->NodeByID(NextNodeID)->Location());
                if(BusSystemIndex->RouteBetweenNodeIDs(PreviousNodeID, NextNodeID)){
                    auto walktime = H_Dist/(transConfig->WalkSpeed());

                    auto bustime = H_Dist/speed; // ***** how to get speed limit *****

                    auto total_bustime = bustime + ((transConfig->BusStopTime())/3600);

                    if(total_bustime < walktime){
                        DFastestPathRouterWalkBus.AddEdge(PreviousNode_VertextID, NextNode_VertexID, total_bustime);
                    }
                    else{
                        DFastestPathRouterWalkBus.AddEdge(PreviousNode_VertextID, NextNode_VertexID, walktime);
                    }
                PreviousNodeID = NextNodeID;
            }
        }
    }
    } 
// return the node count
    std::size_t NodeCount() const noexcept{
        return DStreetMap->NodeCount();
    }

    std::shared_ptr<CStreetMap::SNode> SortedNodeByIndex(std::size_t index) const noexcept {
        // create a vector of these and sort them using standart sort , similar to the bus system
        //std::sort(Sorted.begin(), Sorted.end());

        if(index < Sorted.size()){
            return Sorted[index];
        }else
        //if(index >= Sorted.size())
        {
            return nullptr;
        }
    }
// to find the shortest path
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
            //path.push_back(std::any_cast<TNodeID>(DShortestPathRouter.GetVertexTag(VertexID)));
            path.push_back(VertexID+1);
        }
        return Distance;
    }
// to find the fastest path
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


    bool GetPathDescription(const std::vector< TTripStep >&path, std::vector < std::string > &desc) const{
        // TTripStep start = path[0];
        // TNodeID start_nodeID = start.second;
        // auto start_node = DStreetMap->NodeByID(start_nodeID);
        // auto start_node_loc = start_node->Location();
        // double start_lat = start_node_loc.first;
        // double start_lon = start_node_loc.second;
        // std::string start_lat_str = convertDMS(start_lat, 'L');
        // std::string start_lon_str = convertDMS(start_lon, 'O');
        

        // std::string start_str = "Start at " + start_lat_str + ", " + start_lon_str;
        // desc.push_back(start_str);

        // for(auto i = 1; i < path.size(); i++){
        //     TTripStep stop = path[i];
        //     TNodeID nodeID = stop.second;
        //     auto node = DStreetMap->NodeByID(nodeID);
        //     auto node_loc = node->Location();
        //     double lat = node_loc.first;
        //     double lon = node_loc.second;
        //     std::string lat_str = convertDMS(lat, 'L');
        //     std::string lon_str = convertDMS(lon, 'O');
            return true;

       // }
    }

    

};
// second 4:05

CDijkstraTransportationPlanner::CDijkstraTransportationPlanner(std::shared_ptr<SConfiguration> config){
    DImplementation = std::make_unique<SImplementation>(config);
}

CDijkstraTransportationPlanner::~CDijkstraTransportationPlanner(){

}

std::size_t CDijkstraTransportationPlanner::NodeCount() const noexcept{
    return DImplementation->NodeCount();
}

std::shared_ptr<CStreetMap::SNode> CDijkstraTransportationPlanner::SortedNodeByIndex(std::size_t index) const noexcept{
    return DImplementation->SortedNodeByIndex(index);
}

double CDijkstraTransportationPlanner::FindShortestPath(TNodeID src, TNodeID dest, std::vector< TNodeID > &path){
    return DImplementation->FindShortestPath(src,dest,path);
}

double CDijkstraTransportationPlanner::FindFastestPath(TNodeID src, TNodeID dest, std::vector< TTripStep > &path){
    // profs vid 4:05 it has FindFastestPath
    return DImplementation->FindFastest(src,dest,path);
}

bool  CDijkstraTransportationPlanner::GetPathDescription(const std::vector< TTripStep > &path, std::vector< std::string > &desc) const{
    return DImplementation->GetPathDescription(path,desc);
}
