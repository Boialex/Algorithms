#include "CLeftistAndSkewHeap.h"

LeftistHeap::Node::~Node() {
	delete left;
	delete right;
}

void LeftistHeap::swap(LeftistHeap * second_heap) {
	LeftistHeap tmp;
	tmp.head = second_heap->head;
	second_heap->head = this->head;
	this->head = tmp.head;
}

unsigned int LeftistHeap::getSize() {
	return head.size;
}

void LeftistHeap::melt(IMeltableHeap* heap) { 
	if (heap == NULL)
		return;
	LeftistHeap * second_heap = NULL;
	second_heap = dynamic_cast<LeftistHeap*>(heap);
	if (second_heap == NULL)
		throw "WRONG HEAP";

	if (this == second_heap)
		throw "SAME Heap";
	if (second_heap->head.size == 0)
		return;
	if (this->head.size == 0) {
		this->head = second_heap->head;
		return;
	}
	//сливаем вторую кучу с левым поддеревом
	if (second_heap->head.key < this->head.key)
		this->swap(second_heap);
	if (this->head.right == NULL) {
		if (this->head.right == second_heap)
			throw "SAME HEAP";
		this->head.right = new LeftistHeap(second_heap->head);
	}
	else {
		if (this->head.right == second_heap)
			throw "SAME HEAP";
		this->head.right->melt(second_heap);
	}
	//поворот
	if (this->head.left == NULL) {
		this->head.left = new LeftistHeap(this->head.right->head);
		this->head.right = NULL;
	}
	else
		if (this->head.right->head.dist > this->head.left->head.dist)
			this->head.left->swap(this->head.right);
	//пересчитываем количество вершин
	this->head.size = 1;
	if (this->head.left != NULL)
		this->head.size += this->head.left->head.size;
	if (this->head.right != NULL)
		this->head.size += this->head.right->head.size; 
	//пересчитываем ранг
	if (this->head.left == NULL || this->head.right == NULL)
		this->head.dist = 1;
	else
		this->head.dist = std::min(this->head.right->head.dist, this->head.left->head.dist) + 1;
}

void LeftistHeap::insert(int key) {
	LeftistHeap new_heap(key);
	melt(&new_heap);
}

int LeftistHeap::getMinimum() {
	if (this->head.size == 0)
		return INT_MAX;
	else
		return this->head.key;
}

int LeftistHeap::extractMin() {
	if (this->head.size == 0)
		return INT_MAX;
	int min = this->head.key;
	if (this->head.left == NULL) {
		if (this->head.right == NULL) {
			this->head.key = 0;
			this->head.size = 0;
			this->head.dist = 0;
			return min;
		}
		else {
			this->head = this->head.right->head;
			return min;
		}
	}
	else {
		this->head.left->melt(this->head.right);
		this->head = this->head.left->head;
	}
	return min;
}

void SkewHeap::swap(SkewHeap* second_heap) {
	SkewHeap tmp;
	tmp.head = second_heap->head;
	second_heap->head = this->head;
	this->head = tmp.head;
}

SkewHeap::Node::~Node() {
	/*if (this != NULL) {
		delete left;
		delete right;
	}*/
}

unsigned int SkewHeap::getSize() {
	return this->head.size;
}

int SkewHeap::getMinimum() {
	if (this->head.size == 0)
		return INT_MAX;
	else
		return this->head.key;
}

int SkewHeap::extractMin() {
	if (this->head.size == 0)
		return INT_MAX;
	int min = this->head.key;
	if (this->head.left == NULL) {
		if (this->head.right == NULL) {
			this->head.key = 0;
			this->head.size = 0;
			return min;
		}
		else {
			this->head = this->head.right->head;
			return min;
		}
	}
	else {
		this->head.left->melt(this->head.right);
		this->head = this->head.left->head;
	}
	return min;
}

void SkewHeap::insert(int key) {
	SkewHeap new_heap(key);
	melt(&new_heap);
}

void SkewHeap::melt(IMeltableHeap * heap) {
	if (this == heap)
		return;
	if (heap == NULL)
		return;
	SkewHeap * second_heap = NULL;
	second_heap = dynamic_cast<SkewHeap*>(heap);
	if (second_heap == NULL)
		throw "WRONG HEAP";
	if (second_heap->head.size == 0)
		return;
	if (this->head.size == 0) {
		this->head = second_heap->head;
		return;
	}
	//сливаем
	if (second_heap->head.key < this->head.key)
		this->swap(second_heap);
	if (this->head.right == NULL) {
		this->head.right = new SkewHeap(second_heap->head);
	}
	else
		this->head.right->melt(second_heap);
	//поворот
	if (this->head.left == NULL) {
		this->head.left = new SkewHeap(this->head.right->head);
		this->head.right = NULL;
	}
	else
		this->head.left->swap(this->head.right);
	//пересчитываем количество вершин
	this->head.size = 1;
	if (this->head.left != NULL)
		this->head.size += this->head.left->head.size;
	if (this->head.right != NULL)
		this->head.size += this->head.right->head.size;
}
