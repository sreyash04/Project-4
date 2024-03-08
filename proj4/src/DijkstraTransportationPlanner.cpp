#include "DijkstraTransportationPlanner.h"
#include "TransportationPlannerConfig.h"
#include "TransportationPlanner.h"
#include "DijkstraPathRouter.h"
#include "GeographicUtils.h"
#include <unordered_map>

struct CDijkstraTransportationPlanner::SImplementation{
    std::shared_ptr< CStreetMap > DStreetMap;
    std::shared_ptr< CBusSystem > DBusSystem;
    std::shared_ptr< SGeographicUtils > GeoUtils;
    std::shared_ptr< STransportationPlannerConfig > Trans_plan;
    std::unordered_map< CStreetMap::TNodeID, CPathRouter::TVertexID > DNodeToVertexID;
    CDijkstraPathRouter DShortestPathRouter; // we want locals one for the shortest path pone for biking and one for walking plus bus
    CDijkstraPathRouter DFastestPathRouterBike;
    CDijkstraPathRouter DFastestPathRouterWalkBus;



    SImplementation(std::shared_ptr<SConfiguration> config){
        DStreetMap = config->StreetMap();
        DBusSystem = config->BusSystem();

        for(size_t Index = 0; Index < DStreetMap->NodeCount(); Index++){
            auto Node = DStreetMap->NodeByIndex(Index);
            auto VertexID = DShortestPathRouter.AddVertex(Node->ID());// add a vertex and label it by its node id
            // here there is only the shortest path router we should add them to fastest to the two and do the ecaxt same thing and assume they will be using the same vertex id
            DFastestPathRouterBike.AddVertex(Node->ID());
            DFastestPathRouterWalkBus.AddVertex(Node->ID());
            DNodeToVertexID[Node->ID()] = VertexID; 
        }
        // now go to edges
        for(size_t Index = 0; Index < DStreetMap->WayCount();Index++){
            auto Way = DStreetMap->WayByIndex(Index);
            // asumme bicyle
            bool Bikeable = Way->GetAttribute("bicycle") != "no";
            bool Bidirectional = Way->GetAttribute("oneway") != "yes";
            auto PreviousNodeID = Way->GetNodeID(0);
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

                DShortestPathRouter.AddEdge(PreviousNode_VertextID, NextNode_VertexID, H_Dist);

                auto biketime = H_Dist/(Trans_plan->BikeSpeed());
                DFastestPathRouterBike.AddEdge(PreviousNode_VertextID, NextNode_VertexID, biketime);

                auto walktime = H_Dist/(Trans_plan->WalkSpeed());

                auto bustime = H_Dist/(Trans_plan->DefaultSpeedLimit()); // ***** how to get speed limit *****

                auto total_bustime = bustime + Trans_plan->BusStopTime();

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
                // you are adding the busstoptime + how long taking to drive so for every edge you add the
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

            }
        }
    } 

    std::size_t NodeCount() const noexcept{
        return DStreetMap->NodeCount();
    }

    std::shared_ptr<CStreetMap::SNode> SortedNodeByIndex(std::size_t index) const noexcept{
        // create a vector of these and sort them using standart sort , similar to the bus system
    }

    double FindShortestPath(TNodeID src, TNodeID dest, std::vector< TNodeID > &path){
        std::vector< CPathRouter::TVertexID > ShortestPath; // compare node id to vector id // create a struct
        // put in some checks for this
        auto SourceVertexID = DNodeToVertexID[src];
        auto DestinationVertexID = DNodeToVertexID[dest];
        auto Distance = DShortestPathRouter.FindShortestPath(SourceVertexID,DestinationVertexID,ShortestPath);
        path.clear();
        for(auto VertexID : ShortestPath){
            path.push_back(std::any_cast< TNodeID >(DShortestPathRouter.GetVertexTag(VertexID)));
        }
        return Distance;
    }

    double FindFastest(TNodeID src, TNodeID dest, std::vector< TTripStep > &path){
        std::vector< CPathRouter::TVertexID > ShortestPathBike;
        std::vector< CPathRouter::TVertexID > ShortestPathBusWalk;

        auto SourceVertexID = DNodeToVertexID[src];
        auto DestinationVertexID = DNodeToVertexID[dest];

        auto DistanceBike = DShortestPathRouter.FindShortestPath(SourceVertexID,DestinationVertexID,ShortestPathBike);
        auto DistanceBusWalk = DShortestPathRouter.FindShortestPath(SourceVertexID,DestinationVertexID,ShortestPathBusWalk);

        auto Distance = 0.0;

        path.clear();

        if(DistanceBike < DistanceBusWalk){
            Distance = DistanceBike;
            for(auto VertexID : ShortestPathBike){
                auto x = std::any_cast< TNodeID >(DShortestPathRouter.GetVertexTag(VertexID));
                TTripStep pair = {CTransportationPlanner::ETransportationMode::Bike, x};
                path.push_back(pair);
            }
        }
        else{
            Distance = DistanceBusWalk;
            // for bus/walk depending on which we take
        }


        return Distance;
        // have to do two searches search the bike path and search the walking plus bus and just take the faster of the two the lowest time of the two
    }

    bool GetPathDescription(const std::vector< TTripStep >&path, std::vector < std::string > &desc) const{

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









