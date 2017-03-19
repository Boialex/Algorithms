#include <iostream>
#include <vector>
#include <set>
#include <utility>

const unsigned int Max_Path = 2009000999;								   //вершина недостижима

class User {																//стандартная задача
public:
	class Info {															//инфо о весе ребра
	public:
		int weight;
		Info(int w) : weight(w) {};
		void Input() {
			std::cin >> weight;
		}
	};
	//ребро
	class Edge {
	private:
		size_t begin;														//из вершины
		size_t end;															//в вершину
		Info weight;														//вес
	public:
		Edge(size_t begin, size_t end, Info weight) : begin(begin), end(end), weight(weight) {};
		Info GetInfo() {
			return weight;
		}
		size_t GetBegin() {
			return begin;
		}
		size_t GetEnd() {
			return end;
		}
	};

	class Path {															//путь и пересчет пути
	private:
		int distance;														//путь(время, расстояние...)
		size_t prev;														//предыдущая вершина на кратчайшем пути
	public:
		Path() : distance(2009000999), prev(2009000999) {};
		Path(int w, int p) {
			distance = w;
			prev = p;
		}
		Path operator + (Edge edge) {										 //пересчет пути
			return Path(distance + edge.GetInfo().weight, edge.GetBegin());
		}
		bool operator < (Path p) const {									 //сравнение путей
			if (distance < p.distance)
				return true;
			return false;
		}
		int GetDistance() {
			return distance;
		}
		size_t GetPred() {
			return prev;
		}
	};
	static std::pair<size_t, std::vector<Edge> > ConvertInput() {			 //перевод ввода к виду (кол-во вершин, список ребер)
		size_t vertexs, edges;
		std::vector<Edge> edge;
		std::cin >> vertexs >> edges;
		for (size_t i = 0; i < edges; ++i) {
			size_t begin, end;
			int weight;
			std::cin >> begin >> end >> weight;
			edge.push_back(Edge(begin, end, Info(weight)));
			edge.push_back(Edge(end, begin, Info(weight)));
		}
		return std::make_pair(vertexs, edge);
	}
};

class Buses {																   //задача про автобусы, назначения классов те же
public:
	class Info {
	public:
		size_t start;
		size_t end;
		Info() : start(0), end(0) {};
		Info(size_t start, size_t end) : start(start), end(end) {};
		void Input() {
			std::cin >> start;
			std::cin >> end;
		}
	};

	class Edge {
	private:
		size_t begin;
		size_t end;
		Info weight;
	public:
		Edge(size_t begin, size_t end, Info weight) : begin(begin), end(end), weight(weight) {};
		Info GetInfo() {
			return weight;
		}
		size_t GetBegin() {
			return begin;
		}
		size_t GetEnd() {
			return end;
		}
	};

	class Path {
	private:
		int distance;
		size_t prev;
	public:
		Path() : distance(Max_Path), prev(Max_Path) {};
		Path(int w, size_t p) {
			distance = w;
			prev = p;
		}
		Path operator + (Edge edge) {
			if (distance <= edge.GetInfo().start)
				return Path(edge.GetInfo().end, edge.GetBegin());
			else
				return Path();
		}
		bool operator < (Path p) const {
			if (distance < p.distance)
				return true;
			return false;
		}
		int GetDistance() {
			return distance;
		}
		size_t GetPred() {
			return prev;
		}
	};
	static std::pair<size_t, std::vector<Edge> > ConvertInput(size_t vertexs) {
		size_t edges;
		std::vector<Edge> edge;
		std::cin >> edges;
		for (size_t i = 0; i < edges; ++i) {
			size_t begin, end, time_begin, time_end;
			std::cin >> begin >> time_begin >> end >> time_end;
			Info weight(time_begin, time_end);
			edge.push_back(Edge(begin - 1, end - 1, weight));
		}
		return std::make_pair(vertexs, edge);
	}
};

size_t number_of_cups;

class CupTransportation {													   //задача про доставку кружек. Требуется доставить 
public:																		   //максимальное кол-во кружек, успев за 24 часа
	class Info {															   //если на каждой дороге есть максимально перевозимый вес 
	public:
		size_t time;
		size_t weight_max;
		Info() : time(0), weight_max(0) {};
		Info(size_t start, size_t end) : time(start), weight_max(end) {};
		void Input() {
			std::cin >> time;
			std::cin >> weight_max;
		}
	};

	class Edge {
	private:
		size_t begin;
		size_t end;
		Info weight;
	public:
		Edge(size_t begin, size_t end, Info weight) : begin(begin), end(end), weight(weight) {};
		Info GetInfo() {
			return weight;
		}
		size_t GetBegin() {
			return begin;
		}
		size_t GetEnd() {
			return end;
		}
	};

	class Path {
	private:
		int distance;
		size_t prev;
	public:
		Path() : distance(Max_Path), prev(Max_Path) {};
		Path(int w, size_t p) {
			distance = w;
			prev = p;
		}
		Path operator + (Edge edge) {
			if (number_of_cups * 100 + 3000000 <= edge.GetInfo().weight_max)
				return Path(distance + edge.GetInfo().time, edge.GetBegin());
			else
				return Path();
		}
		bool operator < (Path p) const {
			if (distance < p.distance)
				return true;
			return false;
		}
		int GetDistance() {
			return distance;
		}
		size_t GetPred() {
			return prev;
		}
	};
	static std::pair<size_t, std::vector<Edge> > ConvertInput() {
		size_t vertexs, edges;
		std::vector<Edge> edge;
		std::cin >> vertexs >> edges;
		for (size_t i = 0; i < edges; ++i) {
			size_t begin, end;
			std::cin >> begin >> end;
			Info weight;
			weight.Input();
			edge.push_back(Edge(begin - 1, end - 1, weight));
			edge.push_back(Edge(end - 1, begin - 1, weight));
		}
		return std::make_pair(vertexs, edge);
	}
};

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
	void Input(std::pair<size_t, std::vector<Edge> > in) {			   //получение 	V  по (кол-во вершин, список ребер)
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

int main() {
	//№1"
	/*size_t start, n, end, NUM;
	std::cin >> NUM;
	for (size_t i = 0; i < NUM; ++i) {
	Graph<User> G;
	G.Input(User::ConvertInput());
	std::cin >> start;
	std::vector<User::Path> answer = G.Dekstr(start);
	for (size_t j = 0; j < answer.size(); ++j)
		std::cout << answer[j].GetDistance() << ' ';
	std::cout << std::endl;
	}	*/ 
	//№2
	/*size_t start, n, end;
	Graph<Buses> G;
	std::cin >> n >> start >> end;
	G.Input(Buses::ConvertInput(n));
	std::vector<Buses::Path> answer = G.Dekstr(start - 1);
	if (answer[end - 1].GetDistance() == Max_Path)
		std::cout << -1;
	else
		std::cout << answer[end - 1].GetDistance();	   */
	//№3
	Graph<CupTransportation> G;
	G.Input(CupTransportation::ConvertInput());
	size_t l = 0, r = 10000001;
	number_of_cups = 10000001;
	while (r > l + 1) {
	size_t m = (l + r) / 2;
	number_of_cups = m;
	std::vector<CupTransportation::Path> answer = G.Dekstr(0);
	if (answer[G.Size() - 1].GetDistance() <= 1440)
	l = m;
	else
	r = m;
	}
	std::cout << l << std::endl;
	return 0;
}
