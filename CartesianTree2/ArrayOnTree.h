#pragma once

#include "IArray.h"
#include "stdafx.h"

namespace Priority {
	const unsigned long long PRIME = static_cast<unsigned long long>(UINT_MAX)+15;
}

struct Pair;
class MyVector;

class CartesianTree{
private:
	struct Node {
		int key;
		size_t priority;
		size_t cnt;
		long long sum;
		int min;
		int max;
		bool decreasing;
		bool increasing;
		bool need_reverse;
		Node();
		Node(int key);
	} node;
	CartesianTree * left;
	CartesianTree * right;

	void Recount();
	void Print0();
	void Push();
	static int BinInsert(CartesianTree *, int);
	static Pair SplitDescendingSuffix(CartesianTree *, int);
public:
	CartesianTree(int key) : node(key), left(NULL), right(NULL) {};
	CartesianTree() : CartesianTree(0) {};
	~CartesianTree();
	void Print();
	static Node GetInfo(CartesianTree *);
	static size_t Sizeof(CartesianTree *);
	static void ArrayInput(CartesianTree *, std::vector<int> *);
	static CartesianTree * Merge(CartesianTree *, CartesianTree *);
	static Pair Split(CartesianTree *, int);
	static bool NextPermutationFull(CartesianTree * &);
};

struct Pair {
	Pair(CartesianTree * one, CartesianTree * two) : first(one), second(two) {};
	CartesianTree * first;
	CartesianTree * second;
};

class ArrayOnTree : public IArray {
private:
	CartesianTree * tree;
public:
	ArrayOnTree();
	ArrayOnTree(int);
	virtual ~ArrayOnTree();
	virtual void Insert(int, size_t);
	virtual void Assign(int, size_t);
	virtual int Sum(size_t, size_t);
	virtual bool NextPermutation(size_t, size_t);
	virtual size_t Size();
	virtual void VectorOutput(std::vector<int> &);
	virtual void Output();
};