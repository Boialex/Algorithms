#include <iostream>
#include <vector>
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
		Path() : distance(Max_Path), prev(Max_Path) {};
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
			edge.push_back(Edge(begin, end, weight));
			edge.push_back(Edge(end, begin, weight));
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