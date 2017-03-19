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
	std::vector<IMeltableHeap*> bi_heap;			  //������ ������������ ���	
	std::vector<IMeltableHeap*> left_heap;			  //������ �������� ���
	std::vector<IMeltableHeap*> skew_heap;			  //������ ����� ���
public:
	void addHeap(int key);						  //�������� ����
	void insert(int, int);						  //�������� ���� � ���� � �������� � �������
	void extractMin(int);						  //������ ������� �� ���� � �������� � �������
	void melt(int, int);						  //����� ��� ���� � ��������� � �������
	void mainTest();							  //������� ����������� �������

};	 
