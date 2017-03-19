#pragma once

#include <vector>
#include <queue>
#include <stack>
#include <limits>

#define ui32 unsigned int

class Graph {
public:
    struct Edge {
        ui32 begin = 0;
        ui32 end = 0;
        long long capacityForward = 0;
        long long capacityBack = 0;

        Edge(ui32 newBegin, ui32 newEnd, long long newCapacityForward, long long newCapacityBack);
        Edge reverseEdge() const;
        Edge edgeDirection(ui32 vertex) const;
    };

    Graph();
    Graph(ui32 numberOfVertexes);
    Graph(ui32 newNumberOfVertexes, std::vector<Edge> newEdges);
    ~Graph();

    ui32 getVertexNumber() const;
    std::vector<std::vector <ui32> > getIncomingArcs() const;
    std::vector<std::vector <ui32> > getOutgoingArcs() const;
    std::vector<Edge> getEdges() const;

protected:
    ui32 numberOfVertexes = 0;
    std::vector<Edge> allEdges;                                                 //множество всех ребер(прямое и обратное как одно)
    std::vector<std::vector <ui32> > incomingArcs;                              //для каждой вершины номера в множестве входящих ребер
    std::vector<std::vector <ui32> > outgoingArcs;                              //выходящих ребер
};