#include <gtest/gtest.h>
#include "DijkstraPathRouter.h"

TEST(DijkstraPathRouter,RouteTest){
    CDijkstraPathRouter PathRouter;
    std::vector<CPathRouter::TVertexID> Vertices;
    for(std::size_t Index = 0; Index < 6;Index++){
        Vertices.push_back(PathRouter.AddVertex(Index));
        EXPECT_EQ(Index,std::any_cast<std::size_t>(PathRouter.GetVertexTag(Vertices.back())));
        }
EXPECT_EQ(6,PathRouter.VertexCount());
PathRouter.AddEdge(Vertices [0], Vertices [4],3);
PathRouter.AddEdge(Vertices [4], Vertices [5],90);
PathRouter.AddEdge(Vertices [5], Vertices [3],5);
PathRouter.AddEdge(Vertices [3], Vertices [2],8);
PathRouter.AddEdge(Vertices [2], Vertices [0],1);
PathRouter.AddEdge(Vertices [2], Vertices [1],3);
PathRouter.AddEdge(Vertices [1], Vertices [3],9); 
std::vector<CPathRouter::TVertexID> Route;
std::vector<CPathRouter::TVertexID> ExpectedRoute = {Vertices[2],Vertices[1],Vertices[3]};
EXPECT_EQ(12.0, PathRouter.FindShortestPath(Vertices[2] ,Vertices[3] ,Route));
EXPECT_EQ(Route, ExpectedRoute);
}

TEST(DijkstraPathRouter, OnePathTest) {
    CDijkstraPathRouter PathRouter;
    auto v1 = PathRouter.AddVertex("A");
    auto v2 = PathRouter.AddVertex("B");
    PathRouter.AddEdge(v1, v2, 1.0);
    
    std::vector<CPathRouter::TVertexID> route;
    EXPECT_EQ(1.0, PathRouter.FindShortestPath(v1, v2, route));
    std::vector<CPathRouter::TVertexID> expectedRoute = {v1, v2};
    EXPECT_EQ(route, expectedRoute);
}

TEST(DijkstraPathRouter, NoTest) {
    CDijkstraPathRouter PathRouter;
    auto v1 = PathRouter.AddVertex("A");
    auto v2 = PathRouter.AddVertex("B");
    // No edge added between v1 and v2
    
    std::vector<CPathRouter::TVertexID> route;
    EXPECT_EQ(CPathRouter::NoPathExists, PathRouter.FindShortestPath(v1, v2, route));
}

TEST(DijkstraPathRouter, PathSameTest) {
    CDijkstraPathRouter PathRouter;
    auto v1 = PathRouter.AddVertex("A");
    
    std::vector<CPathRouter::TVertexID> route;
    EXPECT_EQ(0.0, PathRouter.FindShortestPath(v1, v1, route));
    std::vector<CPathRouter::TVertexID> expectedRoute = {v1};
    EXPECT_EQ(route, expectedRoute);
}
