#include "stdafx.h"
#include "MyVector.h"

void MyVector::Insert(int key, size_t position) {
	array.push_back(key);
	for (int i = position; i < array.size(); ++i)
		std::swap(array[i], array[array.size() - 1]);
}

void MyVector::Output() {
	for (std::vector<int>::iterator i = array.begin(); i < array.end(); ++i)
		std::cout << *i << ' ';
	std::cout << std::endl;
}

void MyVector::Assign(int key, size_t position) {
	if (position < 0 || position > array.size() + 1)
		return;
	array[position] = key;
}

int MyVector::Sum(size_t l, size_t r) {
	if (l > r)
		return 0;
	int sum = 0;
	if (l < 0)
		l = 0;
	if (r > array.size())
		r = array.size();
	for (int i = l; i < r; ++i)
		sum += array[i];
	return sum;
}

bool MyVector::NextPermutation(size_t l, size_t r) {
	if (l > r)
		return 0;
	return std::next_permutation(array.begin() + l, array.begin() + r);
}

size_t MyVector::Size() {
	return array.size();
}

void MyVector::VectorOutput(std::vector<int> & array) {
	for (std::vector<int>::iterator i = (this->array).begin(); i < (this->array).end(); ++i)
		array.push_back(*i);
}