#pragma once

#include "IArray.h"

class MyVector : public IArray {
private:
	std::vector<int> array;
public:
	MyVector() : array(0) {};
	MyVector(int n) : array(n) {};
	virtual void Output();
	virtual void Insert(int, size_t);
	virtual void Assign(int, size_t);
	virtual int Sum(size_t, size_t);
	virtual bool NextPermutation(size_t, size_t);
	virtual size_t Size();
	virtual void VectorOutput(std::vector<int> &);
};