#pragma once
#include <ctime>
#include <random>
#include <fstream>
#include <iostream>
#include "MeltableHeap.h"

namespace TestConst {
	const unsigned int MAX_KEY = 1000;
	const unsigned int NUMBER_OF_TEST = 10000;
}

class Test {
private:
	struct Time {
		long long melt_time = 0;
		long long insert_time = 0;
		long long extractMin_time = 0;
	};
	Time binary_time, leftist_time, skew_time;
	std::vector<IMeltableHeap*> bi_heap;			  //вектор биномиальный куч	
	std::vector<IMeltableHeap*> left_heap;			  //вектор левацких куч
	std::vector<IMeltableHeap*> skew_heap;			  //вектор косых куч
public:
	void addHeap(int key);						  //добавить кучу
	void insert(int, int);						  //вставить ключ в кучу с индексом в векторе
	void extractMin(int);						  //изъ€ть минимум из кучи с индексом в векторе
	void melt(int, int);						  //слить две кучи с индексами в векторе
	void mainTest();							  //главна€ тестирующа€ функци€

};	 
