#include <iostream>
#include <vector>
#include <set>
#include <utility>

template <class T>
class Graph {
	typedef typename T::Path Path;
	typedef typename T::Edge Edge;
	typedef typename T::Info Info;
private:
	std::vector< std::vector<Edge> > V;								   //для каждой вершины список(вектор) ребер из нее
public:
	size_t Size() {
		return V.size();
	}
	Graph() {
		V.resize(0);
	}
	Graph(size_t n) {
		V.resize(n);
	}
	void Input(const std::pair<size_t, std::vector<Edge> > & in) {			   //получение 	V  по (кол-во вершин, список ребер)
		V.resize(in.first);
		for (size_t i = 0; i < in.second.size(); ++i) {
			V[in.second[i].GetBegin()].push_back(in.second[i]);
		}
	}
	typedef std::vector<Path> ans;
	ans Dekstr(size_t vertex) {										 //алгоритм Дейкстры, возвращает вектор, в i-ом элементе структура
		ans d(V.size());											 //	Path содержащая длину пути от vertex до i, предыдущую верш пути
		d[vertex] = Path(0, 0);
		std::set< std::pair<Path, size_t> > FindMin;				 //set для нахождения минимума по путям
		FindMin.insert(std::make_pair(d[vertex], vertex));
		while (!FindMin.empty()) {
			size_t current_vertex = FindMin.begin()->second;
			FindMin.erase(FindMin.begin());
			for (size_t i = 0; i < V[current_vertex].size(); ++i) {
				Edge e = V[current_vertex][i];
				if (d[current_vertex] + e < d[e.GetEnd()]) {
					FindMin.erase(std::make_pair(d[e.GetEnd()], e.GetEnd()));
					d[e.GetEnd()] = d[current_vertex] + e;
					FindMin.insert(std::make_pair(d[e.GetEnd()], e.GetEnd()));
				}
			}
		}
		return d;
	}
};