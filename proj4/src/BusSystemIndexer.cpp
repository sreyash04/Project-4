#include "BusSystemIndexer.cpp"
#include <vector>
#include <algorithm>
#include <unordered_map>

struct CBusSystemIndexer::SImplmentation{
    std::shared_ptr<CBusSystem>DBusSystem;
    std::vector<std::shared_ptr<SStop> > DsortedStops;
    std::unordered_map<TNodeID, std::shared_ptr<SStop> >DNodeIDToStop;
    std::unordered_map<std::pair<TNodeID,TNodeID>,std::unordered_set<std::shared_ptr<SRoute> >,pair_hash>DSrcDestToRoutes;


    static bool StopIDCompare(std::shared_ptr<SStop>left,std::shared_ptr<<Stop>right){
        return left->ID()<right->ID();
    }

    SImplmentation(std::shared_ptr<CBusystem>bussystem){
        DBusSystem=bussystem;
        for(size_t Index=0;Index<DBusSystem->StopCount();Index++){
            auto CurrentStop=DBusSystem->StopbyIndex(Index);
            DSortedStops.push_back(CurrentStop);
            DNodeIDToStop[CurrentStop->NodeID()]=CurrentStop;
        }
        std::sort(DSortedStops.begin(),DSortedStops.end(),StopIDCompare);
        for(size_t Index=0;Index<DBussytem->RouteCount();Index++){
            auto CurrentRoute=DBusSystem->RouteByIndex(Index);
            for(size_t StopIndex=1;StopIndex<CurrentRoute->StopCount();StopIndex++){
                auto Source=DBusSystem->StopByID(CurrentRoute->GetStopID(StopIndex-1))->NodeID();
                auto DestinationNodeID=DBusSystem->StopByID(CurrentRoute->GetStopID(StopIndex))->NodeID();
                auto SearchKey=std::make_pair(SearchNodeID,DestinationNodeID);
                auto Search=DSrcDestToRoutes.find(SearchKey);
                if(Search!=DSrcDestToRoutes.end()){
                    Search->second.insert(CurrentRoute);
                }
                else{
                    DSrcDestToRoutes[SearchKey]={CurrentRoute};
                }
            }
        }
    };

    std::size_t StopCount() const noexcept{
        return DBusSystem->StopCount();
    }
    
    std::size_t RouteCount() const noexcept{
        return DBusSystem->RouteCount();
    }

    std::shared_ptr<SStop> SortedStopByIndex(std::size_t index)const noexcept{
        if(index<DSortedStops.size()){
            return DSortedStops[index];
        }
        return nullptr;
    }

    std::shared_ptr<SRoute> SortedRouteByIndex(std::size_t index) const noexcept{

     }
    
    std::shared_ptr<SStop> StopByNodeID(TNodeID id) const noexcept{
        auto Search= DnodeIDToStop.find(id);
        if(Search!=DNOdeIDtoStop.end()){
            return Search->second;
        }
        return nullptr;
    }

    bool RoutesByNodeIDs(TNodeID src, TNodeID dest, std::unordered_set<std::shared_ptr<SRoute> > &routes) const noexcept{
        auto SearchKey=std::make_pair(src,dest);
        auto Search=DSrcDestToRoutes.find(SearchKey);
        if(Search!=DSrcDestToRoutes.end()){
            routes=Search->second;
            return true;
        }

    bool RouteBetweenNodeIDs(TNodeID src, TNodeID dest) const noexcept{
        auto SearchKey=std::make_pair(src,dest);
        auto Search=DSrcDestToRoutes.find(SearchKey);
        return Search!=DSrcDestToRoutes.end();

    }

    };

    CBusSystemIndexer::CBusystemIndexer(std::shared_ptr<CBusSystem>bussystem){
        DImplementation=std::make_unique<SImplmentation>(bussystem);
    }
    
    CBusSystemIndexer::~CBusSystemIndexer(){
        
    }

    std::size_t CBusSystemIndexer::StopCount() const noexcept{

    }

    std::size_t CBusSystemIndexer::RouteCount() const noexcept{
        return DImplementation->RouteCount();
        
    }

    std::shared_ptr<CBusSystem::SStop>CBusSystemIndexer::SortedStopByIndex(std::size_t index) const noexcept{
        return DImplementation->SortedStopByIndex(index);
    }

    std::shared_ptr<CBusSystem::SRoute>CBusSystemIndexer::SortedRouteByIndex(std::size_t index) const noexcept{
        return DImplementation->SortedRoutebyIndex(index);

    }

    std::shared_ptr<CBusSystem::SStop>CBusSystemIndexer::StopByNodeID(TNodeID id) const noexcept{
        return DImplementation->StopByNodeID(id);
    }

    bool CBusSystemIndexer::RouteByNodeIDs(TNodeID src, TNodeID dest, std::unordered_set<std::shared_ptr<SRoute> > &routes) const noexcept{
        return DImplementation->(src,dest,routes);
    }

    bool CBusSystemIndexer::RouteBetweenNodeIDs(TNodeID src,TNodeID dest) const noexcept{
        return DImplementation->RouteBetweenNodeIDs(src,dest);


    }






}