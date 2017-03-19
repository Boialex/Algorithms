#include <iostream>
#include <vector>
#include "Graph.h"
#include "User.h"

int main() {																				//¹1
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
	}  */
																							//¹2
	/*size_t start, n, end;
	Graph<Buses> G;
	std::cin >> n >> start >> end;
	G.Input(Buses::ConvertInput(n));
	std::vector<Buses::Path> answer = G.Dekstr(start - 1);
	if (answer[end - 1].GetDistance() == Max_Path)
		std::cout << -1;
	else
		std::cout << answer[end - 1].GetDistance();	  */
																								  //¹3
	/*Graph<CupTransportation> G;
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
	std::cout << l << std::endl;*/
	return 0;
}
