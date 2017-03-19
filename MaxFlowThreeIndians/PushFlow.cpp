#pragma once

#include "stdafx.h"

#include <deque>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>

using namespace std;

struct vertex {
    vertex();
    long long overflow;
    int height;
};
vertex::vertex (){
    overflow = 0;
    height = 0;
    
}

struct edge {
    edge();
    long long capacity;
    long long flow;
};

edge::edge () {
    capacity = 0;
    flow = 0;
}

class PushFlow {
    
    private :
    
    int vertices;
    int startFlow;
    int finishFlow;
    int maxHeight;
    deque <int> node;
    vector <vertex> vert;
    vector <vector <int> > neigh;
    vector < vector < edge > > edges;
    
    public :
    
    long long Min (long long a, long long b) {
        if (a > b) {
            return b;
        }
        return a;
    }
    PushFlow (int num, int start, int finish) {
        vertices = num;
        startFlow = start;
        finishFlow = finish;
        vert.resize(num);
        vert[0].height = num;
        edges.resize(num);
        maxHeight = num;
        neigh.resize(num);
        for (int i = 0; i < num; ++i) {
            edges[i].resize(num);
        }
    }
    void AddEdge (int u, int v, long long capacity) {
        edges[u][v].capacity += capacity;
        neigh[u].push_back(v);
        neigh[v].push_back(u);
    }
    void InizPreFlow () { // Push from startFlow
        for (int v = 0; v < vertices; v++) {
            if (startFlow == v) continue;
            edges[startFlow][v].flow = edges[startFlow][v].capacity;
            edges[v][startFlow].flow = -edges[startFlow][v].capacity;
            vert[v].overflow = edges[startFlow][v].capacity;
            vert[startFlow].overflow -= edges[startFlow][v].capacity;
            if (edges[startFlow][v].capacity != 0 && v != finishFlow) {
                node.push_back(v);
            }
        }
    }
    void Push (int u, int v) {   // height(u) = height(v) + 1
        long long pushFlow = Min (vert[u].overflow, edges[u][v].capacity - edges[u][v].flow );
        if (vert[v].overflow == 0 && pushFlow > 0 && v != finishFlow && v!= startFlow) {
            node.push_back(v);
        }
        edges[u][v].flow += pushFlow;
        edges[v][u].flow = -edges[u][v].flow;
        vert[u].overflow -= pushFlow;
        vert[v].overflow += pushFlow;
    }
    void Relabel (int u) { // overflow(u) > 0 && NoPush
        vert[u].height = maxHeight;
        for (int v = 0; v < neigh[u].size(); ++v) {
            if (edges[u][neigh[u][v]].capacity - edges[u][neigh[u][v]].flow > 0) {
                vert[u].height = min (vert[u].height, vert[neigh[u][v]].height);
            }
        }
        ++vert[u].height;
        maxHeight = max(maxHeight, vert[u].height);
    }
    void Discharge (int u) {
        int v = 0;
        while (vert[u].overflow > 0) {
            if (vert[u].height == vert[neigh[u][v]].height + 1 && edges[u][neigh[u][v]].capacity - edges[u][neigh[u][v]].flow > 0) {
                Push(u, neigh[u][v]);
            }
            v++;
            if (v == neigh[u].size()) {
                Relabel(u);
                v = 0;
            }
        }
    }
    long long MaxFlow () {
        while (node.size() > 0){
            int u = node.front();
            node.pop_front();
            Discharge(u);
        }
        return vert[finishFlow].overflow;
    }
    long long getFlow (int u, int v, int cap) {
        long long flow = Min(cap, edges[u][v].flow);
        if (flow < 0) flow = 0;
        edges[u][v].flow -= flow;
        return flow;
    }
};

std::pair<long long, std::vector<long long> > SolvePushFlow () {
    std::vector<long long> answer;
    int num, edges;
    std::fstream cin;
    cin.open("C:\\Games\\input.txt");
    cin >> num >> edges;
    vector <int> start, finish, cap;
    PushFlow flow(num, 0, num - 1);
    for (int i = 0; i < edges; ++i) {
        int u, v, capacity;
        cin >> u >> v >> capacity;
        --u;
        --v;
        flow.AddEdge (u, v, capacity);
        start.push_back(u);
        finish.push_back(v);
        cap.push_back(capacity);
    }
    cin.close();
    //fstream cout;
    //cout.open("C:\\Games\\output.txt");
    flow.InizPreFlow();
    long long ans = flow.MaxFlow();
    //cout << flow.MaxFlow() << "\n";
    for (int i = 0; i < edges; ++i) {
        answer.push_back(flow.getFlow(start[i], finish[i], cap[i]));
    }
    return std::make_pair(ans, answer);
   // cout.close();
}
