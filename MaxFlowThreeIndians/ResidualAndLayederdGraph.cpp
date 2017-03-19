#include "stdafx.h"

const unsigned int Infinity = UINT32_MAX;

ResidualGraph::ResidualGraph() {
    numberOfVertexes = 0;
}

ResidualGraph::~ResidualGraph() {

}

ResidualGraph::ResidualGraph(Graph g) {                             //инициализация остаточной сети
    numberOfVertexes = g.getVertexNumber();
    incomingArcs = g.getIncomingArcs();
    outgoingArcs = g.getOutgoingArcs();
    allEdges = g.getEdges();
    flow.resize(numberOfVertexes);
    distance.resize(numberOfVertexes);
    for (size_t i = 0; i < numberOfVertexes; ++i) {
        flow[i].resize(numberOfVertexes);
        distance[i] = Infinity;
        for (size_t j = 0; j < numberOfVertexes; ++j) {
            flow[i][j] = 0;
        }
    }
}

void ResidualGraph::fillTheDistance() {                           //подсчет расстояний от истока до вершин в остаточной сети бфс                       
    std::queue<ui32> vertexesToVisit;
    std::vector<bool> visited(numberOfVertexes, false);
    for (size_t i = 0; i < numberOfVertexes; ++i) {
        distance[i] = Infinity;
    }
    vertexesToVisit.push(0);
    distance[0] = 0;
    while (!vertexesToVisit.empty()) {
        ui32 currentVertex = vertexesToVisit.front();
        vertexesToVisit.pop();
        if (!visited[currentVertex]) {
            visited[currentVertex] = true;
        } else {
            continue;
        }
        for (size_t i = 0; i < outgoingArcs[currentVertex].size(); ++i) {
            ui32 positionOfCurrentEdge = outgoingArcs[currentVertex][i];
            Edge currentEdge = allEdges[positionOfCurrentEdge];
            if (currentEdge.capacityForward - flow[currentVertex][currentEdge.end] > 0) {
                distance[currentEdge.end] = std::min(distance[currentEdge.end], distance[currentVertex] + 1);
                if (!visited[currentEdge.end]) {
                    vertexesToVisit.push(currentEdge.end); 
                }
            }
        }
        for (size_t i = 0; i < incomingArcs[currentVertex].size(); ++i) {
            ui32 positionOfCurrentEdge = incomingArcs[currentVertex][i];
            Edge currentEdge = allEdges[positionOfCurrentEdge].reverseEdge();
            if (currentEdge.capacityForward - flow[currentVertex][currentEdge.end] > 0) {
                distance[currentEdge.end] = std::min(distance[currentEdge.end], distance[currentVertex] + 1);
                if (!visited[currentEdge.end]) {
                    vertexesToVisit.push(currentEdge.end);
                }
            }
        }
    }
}

bool ResidualGraph::hasPath() const {                                   //есть ли путь от истока до стока в остаточной сети
    if (distance[numberOfVertexes - 1] != Infinity)
        return true;
    return false;
}

long long ResidualGraph::maxFlow() const {                              
    long long sumFlow = 0;
    for (size_t i = 0; i < incomingArcs[numberOfVertexes - 1].size(); ++i) {
        ui32 numberOfCurrentVertex = incomingArcs[ numberOfVertexes - 1 ][i];
        sumFlow += flow[ allEdges[ numberOfCurrentVertex].begin ] [ numberOfVertexes - 1 ];
    }
    return sumFlow;
}

bool ResidualGraph::isGoodEdge(Edge e) const {                      //лежит ли ребро на кратчайшем пути
    if (distance[e.begin] + 1 == distance[e.end] && e.capacityForward != 0) {
        return true;
    }
    return false;
}

std::vector<Graph::Edge> ResidualGraph::getEdges() const {
    std::vector<Edge> ostEdges;
    for (size_t i = 0; i < allEdges.size(); ++i) {
        Edge currentEdge = allEdges[i];
        long long a = 0;
        currentEdge.capacityForward = std::max(a, currentEdge.capacityForward - flow[currentEdge.begin][currentEdge.end]);
        currentEdge.capacityBack = std::max(a, currentEdge.capacityBack - flow[currentEdge.end][currentEdge.begin]);
        ostEdges.push_back(currentEdge);
    }
    return ostEdges;
}

void ResidualGraph::updateFlow(ui32 begin, ui32 end, long long newFlow) {
    /*for (size_t i = 0; i < numberOfVertexes; ++i) {
        for (size_t j = 0; j < numberOfVertexes; ++j) {
            flow[i][j] += newFlow[i][j];
        }
    }*/
    flow[begin][end] += newFlow;
    flow[end][begin] -= newFlow;
}

std::vector<long long> ResidualGraph::getFlowEdges(std::vector<Edge> edgesWanted) {
    std::vector<long long> answer;
    for (size_t i = 0; i < edgesWanted.size(); ++i) {
        long long realFlow = std::min(edgesWanted[i].capacityForward, flow[edgesWanted[i].begin][edgesWanted[i].end]);
        if (realFlow < 0) {
            realFlow = 0;
        }
        answer.push_back(realFlow);
        flow[edgesWanted[i].begin][edgesWanted[i].end] -= realFlow;
    }
    return answer;
}

//-------------------------------------------------------------------------------------------------------------------------

LayeredNet::LayeredNet() {
    numberOfVertexes = 0;
}

LayeredNet::LayeredNet(ResidualGraph g) {                                       //построение слоистой сети по остаточной
    numberOfVertexes = g.getVertexNumber();
    incomingArcs.resize(numberOfVertexes);
    outgoingArcs.resize(numberOfVertexes);
    potentialIn.resize(numberOfVertexes);
    potentialOut.resize(numberOfVertexes);
    std::vector<Edge> newEdges = g.getEdges();

    size_t numberOfEdge = 0;
    for (size_t i = 0; i < newEdges.size(); ++i) {
        Edge currentEdge = newEdges[i];
        if (g.isGoodEdge(currentEdge.reverseEdge())) {
            currentEdge = currentEdge.reverseEdge();
        }
        if (g.isGoodEdge(currentEdge)) {
            allEdges.push_back(currentEdge);
            potentialIn[currentEdge.end] += currentEdge.capacityForward;
            potentialOut[currentEdge.begin] += currentEdge.capacityForward;
            incomingArcs[currentEdge.end].push_back(numberOfEdge);
            outgoingArcs[currentEdge.begin].push_back(numberOfEdge);
            ++numberOfEdge;
        }
    }
    potentialIn[0] = INT64_MAX;
    potentialOut[numberOfVertexes - 1] = INT64_MAX;
    edgesLeft = allEdges.size();
    flow.resize(numberOfVertexes);
    deletedNode.resize(numberOfVertexes);
    for (size_t i = 0; i < numberOfVertexes; ++i)
        deletedNode[i] = false;
    /*for (size_t i = 0; i < numberOfVertexes; ++i) {
        flow[i].resize(numberOfVertexes);
        for (size_t j = 0; j < numberOfVertexes; ++j) {
            flow[i][j] = 0;
        }
    }*/
}
/*
void LayeredNet::recauntVertexPotential(ui32 number) {
    long long potentialIn = 0, potentialOut = 0;
    for (size_t j = 0; j < incomingArcs[number].size(); ++j) {
        Edge currentEdge = allEdges[incomingArcs[number][j]];
        potentialIn += currentEdge.capacityForward;
    }
    for (size_t j = 0; j < outgoingArcs[number].size(); ++j) {
        Edge currentEdge = allEdges[outgoingArcs[number][j]];
        potentialOut += currentEdge.capacityForward;
    }
    LayeredNet::potentialIn[number] = potentialIn;
    LayeredNet::potentialOut[number] = potentialOut;
}

void LayeredNet::recountAllPotentials() {
    for (size_t i = 0; i < numberOfVertexes; ++i) {
        recauntVertexPotential(i);
    }
    potentialIn[0] = INT64_MAX;
    potentialOut[numberOfVertexes - 1] = INT64_MAX;
}
*/
void LayeredNet::deleteZeroPotentialVertexes() {
    std::queue<ui32> zeroPotentialNodes;
    for (size_t i = 0; i < numberOfVertexes; ++i) {
        if (std::min(potentialIn[i], potentialOut[i]) == 0 && !deletedNode[i]) {
            zeroPotentialNodes.push(i);
        }
    }
    while (!zeroPotentialNodes.empty()) {
        ui32 currentNode = zeroPotentialNodes.front();
        zeroPotentialNodes.pop();
        if (!deletedNode[currentNode]) {
            deletedNode[currentNode] = true;
        } else {
            continue;
        }
        if (currentNode != 0) {
            for (size_t i = 0; i < incomingArcs[currentNode].size(); ++i) {
                ui32 beginOfEdge = allEdges[incomingArcs[currentNode][i]].begin;
                if (deletedNode[beginOfEdge]) {
                    continue;
                }
                potentialOut[beginOfEdge] -= allEdges[incomingArcs[currentNode][i]].capacityForward;
                allEdges[incomingArcs[currentNode][i]].capacityForward = 0;
                --edgesLeft;
                if (std::min(potentialIn[beginOfEdge], potentialOut[beginOfEdge]) == 0 && !deletedNode[beginOfEdge]) {
                    zeroPotentialNodes.push(beginOfEdge);
                }
            }
        }
        if (currentNode != numberOfVertexes - 1) {
            for (size_t i = 0; i < outgoingArcs[currentNode].size(); ++i) {
                ui32 endOfEdge = allEdges[outgoingArcs[currentNode][i]].end;
                if (deletedNode[endOfEdge]) {
                    continue;
                }
                potentialIn[endOfEdge] -= allEdges[outgoingArcs[currentNode][i]].capacityForward;
                allEdges[outgoingArcs[currentNode][i]].capacityForward = 0;
                --edgesLeft;
                if (std::min(potentialIn[endOfEdge], potentialOut[endOfEdge]) == 0 && !deletedNode[endOfEdge]) {
                    zeroPotentialNodes.push(endOfEdge);
                }
            }
        }
    }

}

ui32 LayeredNet::referenceNode() const {                                    //вершина с минимальным потенциалом
    long long minPotential = INT64_MAX;
    ui32 minNode = 0;
    for (size_t i = 0; i < numberOfVertexes; ++i) {
        long long currentPotential = std::min(potentialIn[i], potentialOut[i]);
        if (currentPotential == 0)
            continue;
        if (currentPotential < minPotential) {
            minPotential = currentPotential;
            minNode = i;
        }
    }
    return minNode;
}

void LayeredNet::findBlockingFlow(ResidualGraph & g) {
    //recountAllPotentials();
    deleteZeroPotentialVertexes();
    while (edgesLeft > 0) {
        ui32 referNode = referenceNode();
        long long pushedFlow = std::min(potentialIn[referNode], potentialOut[referNode]);
        push(referNode, pushedFlow, g);
        draw(referNode, pushedFlow, g);
        //recountAllPotentials();
        deleteZeroPotentialVertexes();
    }
    //setFlowObliq();                                                           
}

void LayeredNet::push(ui32 node, long long pushedFlow, ResidualGraph & g) {                                                         //толкает поток к стоку
    std::queue<ui32> nodeToCount;
    std::vector<long long> extraFlow(numberOfVertexes, 0);
    extraFlow[node] = pushedFlow;
    nodeToCount.push(node);
    while (!nodeToCount.empty()) {
        ui32 currentNode = nodeToCount.front();
        nodeToCount.pop();
        if (currentNode == numberOfVertexes - 1) {
            continue;
        }
        size_t i = 0;
        while (extraFlow[currentNode] != 0) {
            ui32 numberOfEdge = outgoingArcs[currentNode][i];
            if (allEdges[numberOfEdge].capacityForward != 0) {
                long long currentPushedFlow;
                if (extraFlow[currentNode] >= allEdges[numberOfEdge].capacityForward) {     //насыщающее проталкивание
                    currentPushedFlow = allEdges[numberOfEdge].capacityForward;
                } else {
                    currentPushedFlow = extraFlow[currentNode];                                    //ненасыщающее
                }

                //flow[currentNode][allEdges[numberOfEdge].end] += currentPushedFlow;
                g.updateFlow(currentNode, allEdges[numberOfEdge].end, currentPushedFlow);
                extraFlow[currentNode] -= currentPushedFlow;
                extraFlow[allEdges[numberOfEdge].end] += currentPushedFlow;
                allEdges[numberOfEdge].capacityForward -= currentPushedFlow;
                potentialOut[currentNode] -= currentPushedFlow;
                potentialIn[allEdges[numberOfEdge].end] -= currentPushedFlow;

                nodeToCount.push(allEdges[numberOfEdge].end);
            }
            ++i;
        }
    }
}

void LayeredNet::draw(ui32 node, long long pushedFlow, ResidualGraph & g) {                                                        //подкачивает поток от истока
    std::queue<ui32> nodeToCount;
    std::vector<long long> extraFlow(numberOfVertexes, 0);
    extraFlow[node] = pushedFlow;
    nodeToCount.push(node);
    while (!nodeToCount.empty()) {
        ui32 currentNode = nodeToCount.front();
        nodeToCount.pop();
        if (currentNode == 0) {
            continue;
        }
        size_t i = 0;
        while (extraFlow[currentNode] != 0) {
            ui32 numberOfEdge = incomingArcs[currentNode][i];
            if (allEdges[numberOfEdge].capacityForward != 0) {
                long long currentDrawFlow;
                if (extraFlow[currentNode] >= allEdges[numberOfEdge].capacityForward) {
                    currentDrawFlow = allEdges[numberOfEdge].capacityForward;
                }
                else {
                    currentDrawFlow = extraFlow[currentNode];
                }

                //flow[allEdges[numberOfEdge].begin][currentNode] += currentDrawFlow;
                g.updateFlow(allEdges[numberOfEdge].begin, currentNode, currentDrawFlow);
                extraFlow[currentNode] -= currentDrawFlow;
                extraFlow[allEdges[numberOfEdge].begin] += currentDrawFlow;
                allEdges[numberOfEdge].capacityForward -= currentDrawFlow;
                potentialIn[currentNode] -= currentDrawFlow;
                potentialOut[allEdges[numberOfEdge].begin] -= currentDrawFlow;

                nodeToCount.push(allEdges[numberOfEdge].begin);
            }
            ++i;
        }
    }
}

std::vector<std::vector <long long> > LayeredNet::getFlow() const {
    return flow;
}

void LayeredNet::setFlowObliq() {
    for (size_t i = 0; i < numberOfVertexes; ++i) {
        for (size_t j = 0; j < numberOfVertexes; ++j) {
            if (flow[i][j] == 0)
                flow[i][j] = -flow[j][i];
        }
    }
}

LayeredNet::~LayeredNet() {

}
