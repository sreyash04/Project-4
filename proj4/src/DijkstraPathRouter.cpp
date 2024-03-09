#include "DijkstraTransportationPlanner.h"
#include "DijkstraPathRouter.h"
#include <algorithm>

struct CDijkstraPathRouter::SImplementation
{
    using TEdge = std::pair<double, TVertexID>;

    struct Svertex
    {
        std::any Dtag;
        std::vector<TEdge> Dedge;
    };

    std::vector<Svertex> DVertices;

    std::size_t VertexCount() const noexcept
    {
        return DVertices.size();
    }

    TVertexID AddVertex(std::any tag) noexcept
    {
        TVertexID NewVertexID = DVertices.size();
        DVertices.push_back({tag, {}});
        return NewVertexID;
    }

    std::any GetVertexTag(TVertexID id) const noexcept
    {
        if (id < DVertices.size())
        {
            // profs code has DTag
            return DVertices[id].Dtag;
        }
        return std::any();
    }

    bool AddEdge(TVertexID src, TVertexID dest, double weight, bool bidir = false) noexcept
    {
        if ((src < DVertices.size()) && (dest < DVertices.size()) && (0.0 <= weight))
        {
            // profs code has DEdges
            DVertices[src].Dedge.push_back(std::make_pair(weight, dest));
            if (bidir)
            {
                // profs code has DEdges
                DVertices[dest].Dedge.push_back(std::make_pair(weight, src));
            }
            return true;
        }
        return true;
    }

    bool Precompute(std::chrono::steady_clock::time_point deadline) noexcept
    {
        return true;
    }

    double FindShortestPath(TVertexID src, TVertexID dest, std::vector<TVertexID> &path) noexcept
    {
        std::vector<TVertexID> PendingVertices;
        std::vector<TVertexID> Previous(DVertices.size(), CPathRouter::InvalidVertexID);
        // initialize to infinity
        std::vector<double> Distances(DVertices.size(), CPathRouter::NoPathExists);
        // like declaring a function inside of a function accesing a global variable even though it is local
        auto VertexCompare = [&Distances](TVertexID left, TVertexID right)
        { return Distances[left] < Distances[right]; };

        // set to 0
        Distances[src] = 0.0;
        PendingVertices.push_back(src);
        while (!PendingVertices.empty())
        {
            // make heap in c++ web
            // can specify the actual value how to sort it so like a comapre function
            // pop heap and then get the back and pop the back get the front
            auto CurrentID = PendingVertices.front();
            std::pop_heap(PendingVertices.begin(), PendingVertices.end(), VertexCompare);
            PendingVertices.pop_back(); // get current id from top of heap like priority queque
            // DEdges prof
            //  this will go through all of the destinations
            //  we can do an early termination
            //  here or as soon as we have the current id break out of the loop early
            //  if current id is equal to destination you have found the destination
            //  you can add an if and a break right here ~~~~ to outperform the profs code

            // i am doing this
            if(CurrentID == dest){
                break;
            }

            for (auto Edge : DVertices[CurrentID].Dedge)
            {
                auto EdgeWeight = Edge.first;
                auto DestID = Edge.second;
                auto TotalDistance = Distances[CurrentID] + EdgeWeight;
                if (TotalDistance < Distances[DestID])
                {
                    if (CPathRouter::NoPathExists == Distances[DestID])
                    {
                        PendingVertices.push_back(DestID);
                    }
                    Distances[DestID] = TotalDistance;
                    Previous[DestID] = CurrentID;
                }
            }
            std::make_heap(PendingVertices.begin(), PendingVertices.end(), VertexCompare);
        }
        // to reverse
        if (CPathRouter::NoPathExists == Distances[dest])
        {
            return CPathRouter::NoPathExists;
        }
        double PathDistance = Distances[dest];
        path.clear();
        path.push_back(dest);
        do
        {
            dest = Previous[dest];
            path.push_back(dest);
        } while (dest != src);
        std::reverse(path.begin(), path.end());
        return PathDistance;
    }
    // can do more optimizations we can do chat gpt to optimize more
    // this will work till here
};

// CDijkstraPathRouter member functions
// Constructor for the Dijkstra Path Router

// std::vector<std::shared_ptr<PathRouter::>> TVertexID;
CDijkstraPathRouter::CDijkstraPathRouter()
{
    DImplementation = std::make_unique<SImplementation>();
}
// Destructor for the Dijkstra Path Router
CDijkstraPathRouter::~CDijkstraPathRouter()
{
}

// Returns the number of vertices in the path router
std::size_t CDijkstraPathRouter::VertexCount() const noexcept
{
    return DImplementation->VertexCount();
}
// Adds a vertex with the tag provided. The tag can be of any type.
CDijkstraPathRouter::TVertexID CDijkstraPathRouter::AddVertex(std::any tag) noexcept
{
    return DImplementation->AddVertex(tag);
}
// Gets the tag of the vertex specified by id if id is in the path router. // A std::any() is returned if id is not a valid vertex ID.
std::any CDijkstraPathRouter::GetVertexTag(TVertexID id) const noexcept
{
    return DImplementation->GetVertexTag(id);
}

// Adds an edge between src and dest vertices with a weight of weight.
// bidir is set to true an additional edge between dest and src is added.
// src or dest nodes do not exist, or if the weight is negative the AddEdge
// will return false, otherwise it returns true.
bool CDijkstraPathRouter::AddEdge(TVertexID src, TVertexID dest, double weight, bool bidir) noexcept
{
    return DImplementation->AddEdge(src, dest, weight, bidir);
}
// Allows the path router to do any desired precomputation up to the deadline
bool CDijkstraPathRouter::Precompute(std::chrono::steady_clock::time_point deadline) noexcept
{
    return true;
}
// Returns the path distance of the path from src to dest, and fills out path
// with vertices. If no path exists NoPathExists is returned.
double CDijkstraPathRouter::FindShortestPath(TVertexID src, TVertexID dest, std::vector<TVertexID> &path) noexcept
{
    return DImplementation->FindShortestPath(src, dest, path);
}
