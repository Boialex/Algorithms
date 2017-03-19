#pragma once

#include <stdio.h>
#include <tchar.h>
#include <list>
#include <vector>
#include <algorithm>

#include "MeltableHeap.h"

class BinomialHeap : public IMeltableHeap {
private:
	struct BiNode {
		int key;
		unsigned int degree;
		BiNode * left_child;
		BiNode * right_brother;
		BiNode(int key) : key(key), degree(0), left_child(NULL), right_brother(NULL) {};
		BiNode() : key(0), degree(0), left_child(NULL), right_brother(NULL) {};
		BiNode(BiNode * node) : key(node->key), degree(node->degree), left_child(node->left_child), right_brother(node->right_brother) {};
		void swap(BiNode *);
	};
	BiNode * head;
	unsigned int size = 0;
	int min = INT_MAX;

	void swap(BinomialHeap*);
	void print();
public:
	BinomialHeap() : head(NULL), size(0), min(INT_MAX) {};
	BinomialHeap(int key) : head(new BiNode(key)), size(1), min(key) {};
	BinomialHeap(BiNode * head) : head(head), size(1), min(head->key) {};
	virtual ~BinomialHeap();

	virtual void insert(int key);
	virtual int getMinimum();
	virtual int extractMin();
	virtual void melt(IMeltableHeap*);
	unsigned int getSize();
};


