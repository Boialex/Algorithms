// MaxFlowThreeIndians.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include <fstream>
#include <random>

/*
void generateTest(ui32 n) {
    std::ofstream cout;
    cout.open("C:\\Games\\input.txt");
    cout.clear();
    ui32 edg = n * n * 3 / 4;
    cout << n << ' ' << edg << std::endl;
    for (size_t i = 0; i < edg; ++i) {
        ui32 begin = rand() % n + 1;
        ui32 end = rand() % n + 1;
        long long capacity = rand() % 5;
       cout << begin << ' ' << end << ' ' << capacity << std::endl;
    }
    cout.close();
}

bool check(ResidualGraph myGraph, std::vector<Graph::Edge> edges, std::pair<long long, std::vector<long long> > hisAnswer) {
    bool oK = true;
    long long myAnswer = myGraph.maxFlow();
    std::vector<long long> answer = myGraph.getFlowEdges(edges);
    std::fstream cout;
    cout.open("C:\\Games\\output.txt");
    //long long hisAnswer, hisFlow;
    //cin >> hisAnswer;
    if (myAnswer != hisAnswer.first) {
        cout.clear();
        cout << "error my = " << myAnswer << " his = " << hisAnswer.first << std::endl;
        oK = false;
    }
    if (!oK) {
        for (size_t i = 0; i < edges.size(); ++i) {
            //if (hisAnswer.second[i] != answer[i] && !oK) {
            cout << "edge error his =  " << hisAnswer.second[i] << " my = " << answer[i] << " edgeNumber = " << i + 1 << std::endl;
            // oK = false;
         //}
        }
    }
    cout.close();
    return oK;
}
*/

void MKM() {
    std::ios_base::sync_with_stdio(false);
    /*
    for (size_t i = 2; i < 500; ++i) {
        for (size_t j = 0; j < 10000; ++j) {
            std::vector<Graph::Edge> edges;
            ui32 v = 0, e = 0;
            generateTest(i);
            std::fstream cin;
            cin.open("C:\\Games\\input.txt");
            cin >> v >> e;
            for (size_t i = 0; i < e; ++i) {
                ui32 begin, end;
                long long capacity;
                cin >> begin >> end >> capacity;
                //std::cout << begin << ' ' << end << ' ' << capacity << std::endl;
                Graph::Edge edge(begin - 1, end - 1, capacity, 0);
                edges.push_back(edge);
            }
            cin.close();
            Graph g(v, edges);
            //std::cout << MKM(g) << std::endl;
            ResidualGraph myGraph(g);
            myGraph.fillTheDistance();
            while (myGraph.hasPath()) {
                LayeredNet newNet(myGraph);
                newNet.findBlockingFlow();
                myGraph.updateFlow(newNet.getFlow());
                myGraph.fillTheDistance();
            }
            std::cout << myGraph.maxFlow() << std::endl;
            std::vector<long long> answer = myGraph.getFlowEdges(edges);
            for (size_t i = 0; i < edges.size(); ++i) {
            std::cout << answer[i] << std::endl;
            }
            if (!check(myGraph, edges, SolvePushFlow())) {
                std::cout << "error" << std::endl;
                ui32 v;
                std::cin >> v;
            }
            //std::cout << j << " is OK " << i << std::endl;
        }
        std::cout << i << " is OK" << std::endl;
    }
    ui32 v;
    std::cin >> v;*/
    std::vector<Graph::Edge> edges;
    ui32 v = 0, e = 0;
    std::fstream cin;
    cin.open("C:\\Games\\input.txt");
    cin >> v >> e;
    for (size_t i = 0; i < e; ++i) {
        ui32 begin, end;
        long long capacity;
        cin >> begin >> end >> capacity;
        Graph::Edge edge(begin - 1, end - 1, capacity, 0);
        edges.push_back(edge);
    }
    cin.close();
    Graph g(v, edges);
    ResidualGraph myGraph(g);
    myGraph.fillTheDistance();
    while (myGraph.hasPath()) {
        LayeredNet newNet(myGraph);
        newNet.findBlockingFlow(myGraph);
        //myGraph.updateFlow(newNet.getFlow());
        myGraph.fillTheDistance();
    }
    std::cout << myGraph.maxFlow() << std::endl;
    std::vector<long long> answer = myGraph.getFlowEdges(edges);
    for (size_t i = 0; i < edges.size(); ++i) {
        std::cout << answer[i] << std::endl;
    }
    std::cin >> v;
}

int main()
{
    MKM();
    return 0;
}

