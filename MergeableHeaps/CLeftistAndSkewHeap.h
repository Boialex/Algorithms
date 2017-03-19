#pragma once

#include <stdio.h>
#include <tchar.h>
#include <algorithm>
#include <iostream>

#include "MeltableHeap.h"

class LeftistHeap : public IMeltableHeap {
private:
	class Node {
	public:
		int key = 0;
		int dist = 1;
		unsigned int size = 0;
		LeftistHeap * left = NULL;
		LeftistHeap * right = NULL;
		Node(int key) : key(key), dist(1), size(1), left(NULL), right(NULL) {};
		Node() : key(0), dist(1), size(0), left(NULL), right(NULL) {};
		~Node();
	};
	Node head;

	void swap(LeftistHeap*);
	void print();
public:
	LeftistHeap() : head() {};
	LeftistHeap(int key) : head(key) {};
	LeftistHeap(Node head) : head(head) {};

	virtual void insert(int key);
	virtual int getMinimum();
	virtual int extractMin();
	virtual void melt(IMeltableHeap*);
	unsigned int getSize();
};

class SkewHeap : public IMeltableHeap {
private:
	class Node {
	public:
		int key = 0;
		unsigned int size = 0;
		SkewHeap * left = NULL;
		SkewHeap * right = NULL;
		Node(int key) : key(key), size(1), left(NULL), right(NULL) {};
		Node() : key(0), size(0), left(NULL), right(NULL) {};
		~Node();
	};
	Node head;

	void swap(SkewHeap*);
	void print();
public:
	SkewHeap() : head() {};
	SkewHeap(int key) : head(key) {};
	SkewHeap(Node head) : head(head) {};

	virtual void insert(int key);
	virtual int getMinimum();
	virtual int extractMin();
	virtual void melt(IMeltableHeap*);
	unsigned int getSize();
};
