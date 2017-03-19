#pragma once

class IArray {
public:
	virtual ~IArray() {};
	virtual void Insert(int, size_t) = 0;
	virtual void Assign(int, size_t) = 0;
	virtual int Sum(size_t, size_t) = 0;
	virtual void Output() = 0;
	virtual bool NextPermutation(size_t, size_t) = 0;
	virtual size_t Size() = 0;
	virtual void VectorOutput(std::vector<int> &) = 0;
};

enum array_type {
	ht_Vector,
	ht_ArrayOnTree
};

IArray * createArray(array_type type);