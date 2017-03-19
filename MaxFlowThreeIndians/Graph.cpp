#include "stdafx.h"

Graph::Edge::Edge(ui32 newBegin, ui32 newEnd, long long newCapacityForward, long long newCapacityBack) {
    begin = newBegin;
    end = newEnd;
    capacityForward = newCapacityForward;
    capacityBack = newCapacityBack;
}

Graph::Edge Graph::Edge::reverseEdge() const {
    return Edge(this->end, this->begin, this->capacityBack, this->capacityForward);
}

Graph::Edge Graph::Edge::edgeDirection(ui32 vertex) const {
    if (vertex == this->begin) {
        return Edge(this->begin, this->end, this->capacityForward, this->capacityBack);
    } else {
        return this->reverseEdge();
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------

Graph::Graph() {
    numberOfVertexes = 0;
}

Graph::Graph(ui32 newNumberOfVertexes) {
    numberOfVertexes = newNumberOfVertexes;
    incomingArcs.resize(numberOfVertexes);
    outgoingArcs.resize(numberOfVertexes);
}

Graph::Graph(ui32 newNumberOfVertexes, std::vector<Edge> newEdges) {            //инициализация графа ребрами
    numberOfVertexes = newNumberOfVertexes;
    incomingArcs.resize(numberOfVertexes);
    outgoingArcs.resize(numberOfVertexes);
    std::vector<std::vector < int > > addedEdgesNumbers;
    addedEdgesNumbers.resize(numberOfVertexes);                                 //ребро с такими концами уже было добавлено
    for (size_t i = 0; i < numberOfVertexes; ++i) {
        addedEdgesNumbers[i].resize(numberOfVertexes);
        for (size_t j = 0; j < numberOfVertexes; ++j)
            addedEdgesNumbers[i][j] = -1;
    }

    size_t numberInEdgeVector = 0;
    for (size_t i = 0; i < newEdges.size(); ++i) {
        Edge myEdge = newEdges[i];
        int position = addedEdgesNumbers[myEdge.begin][myEdge.end];
        
        if (position != -1) {
            if (myEdge.begin < myEdge.end) {
                allEdges[position].capacityForward += myEdge.capacityForward;
            } else {
                allEdges[position].capacityBack += myEdge.capacityForward;
            }
        }
        else {
            if (myEdge.begin > myEdge.end) {
                myEdge = myEdge.reverseEdge();
            }
            allEdges.push_back(myEdge);
            incomingArcs[myEdge.end].push_back(numberInEdgeVector);
            outgoingArcs[myEdge.begin].push_back(numberInEdgeVector);
            addedEdgesNumbers[myEdge.begin][myEdge.end] = addedEdgesNumbers[myEdge.end][myEdge.begin] = numberInEdgeVector;
            ++numberInEdgeVector;
        }
    }
}

Graph::~Graph() {

}

ui32 Graph::getVertexNumber() const {
    return numberOfVertexes;
}

std::vector<std::vector <ui32> > Graph::getIncomingArcs() const {
    return incomingArcs;
}

std::vector<std::vector <ui32> > Graph::getOutgoingArcs() const {
    return outgoingArcs;
}

std::vector<Graph::Edge> Graph::getEdges() const {
    return allEdges;
}
