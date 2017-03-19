#pragma once

#include "Graph.h"

class ResidualGraph : public Graph {
public:
    ResidualGraph();
    ResidualGraph(Graph g);
    ~ResidualGraph();

    void fillTheDistance();
    bool hasPath() const;
    bool isGoodEdge(Edge e) const;
    long long maxFlow() const;
    void updateFlow(ui32 begin, ui32 end, long long newFlow);
    std::vector<Edge> getEdges() const;
    std::vector<long long> getFlowEdges(std::vector<Edge> edgesWanted);

protected:
    std::vector<std::vector < long long > > flow;

private:
    std::vector<ui32> distance;
};
   
//---------------------------------------------------------------------------------------------------------------------------------------------------------------

class LayeredNet : public ResidualGraph {
public:
    LayeredNet();
    LayeredNet(ResidualGraph resGraph);
    ~LayeredNet();

    //void recauntVertexPotential(ui32 number);
    //void recountAllPotentials();
    void deleteZeroPotentialVertexes();
    ui32 referenceNode() const;
    void findBlockingFlow(ResidualGraph & g);
    void push(ui32 node, long long pushedFlow, ResidualGraph & g);
    void draw(ui32 node, long long pushedFlow, ResidualGraph & g);
    void setFlowObliq();
    std::vector<std::vector <long long> > getFlow() const;

private:
    int edgesLeft = 0;
    std::vector<long long> potentialIn;
    std::vector<long long> potentialOut;
    std::vector<bool> deletedNode;
};
