#include "CBinomialHeap.h"

void BinomialHeap::BiNode::swap(BiNode * second_heap) {
	std::swap(this->key, second_heap->key);
	std::swap(this->left_child, second_heap->left_child);
}

BinomialHeap::~BinomialHeap() {
	/*if (this->head->left_child != NULL)
		delete this->head->left_child;
	if (this->head->right_brother != NULL)
		delete this->head->right_brother;*/
}

unsigned int BinomialHeap::getSize() {
	return size;
}

int BinomialHeap::getMinimum() {
	if (this->head == NULL)
		return INT_MAX;
	else {
		int min = this->head->key;
		BiNode * i = this->head;
		while (i != NULL) {
			if (i->key < min)
				min = i->key;
			i = i->right_brother;
		}
		return min;
	}
}

void BinomialHeap::melt(IMeltableHeap* heap) {
	if (this == heap)
		return;
	if (heap == NULL)
		return;
	BinomialHeap * second_heap;
	second_heap = dynamic_cast<BinomialHeap*>(heap);
	if (second_heap == NULL) {
		throw "WRONG HEAP";
	}
	if (second_heap->head == NULL)
		return;
	if (this->size == 0) {
		this->head = second_heap->head;
		this->size = second_heap->size;
		this->min = second_heap->min;
		return;
	}

	this->size += second_heap->size;

	BinomialHeap * result_heap = new BinomialHeap();	//список - результат слияния
	result_heap->head = new BiNode;
	BiNode * result_heap_iterator = result_heap->head;
	BiNode * first_heap_iterator = new BiNode(this->head);
	BiNode * second_heap_iterator = new BiNode(second_heap->head);
	while (first_heap_iterator != NULL && second_heap_iterator != NULL) {
		result_heap_iterator->right_brother = new BiNode;
		if (first_heap_iterator->degree < second_heap_iterator->degree) {	  //дерево меньшей степени из первой кучи(списка)
			result_heap_iterator->right_brother = first_heap_iterator;
			first_heap_iterator = first_heap_iterator->right_brother;
		}
		else {
			result_heap_iterator->right_brother = second_heap_iterator;          //аналогично из второй кучи(списка)
			second_heap_iterator = second_heap_iterator->right_brother;
		}
		result_heap_iterator = result_heap_iterator->right_brother;
	}
	if (first_heap_iterator == NULL)												  //доливаем оставшиеся деревья
		while (second_heap_iterator != NULL) {
			result_heap_iterator->right_brother = second_heap_iterator;
			result_heap_iterator = result_heap_iterator->right_brother;
			second_heap_iterator = second_heap_iterator->right_brother;
			if (second_heap_iterator != NULL)
				result_heap_iterator->right_brother = new BiNode;
		}
	else
		while (first_heap_iterator != NULL) {										
			result_heap_iterator->right_brother = first_heap_iterator;
			result_heap_iterator = result_heap_iterator->right_brother;
			first_heap_iterator = first_heap_iterator->right_brother;
			if (first_heap_iterator != NULL)
				result_heap_iterator->right_brother = new BiNode;
		}  

	BiNode * no_need_node = result_heap->head;										 //удаляем первую вершину, куда 
	result_heap->head = result_heap->head->right_brother;	                                 //ничего не записано
	no_need_node->left_child = no_need_node->right_brother = NULL;
	delete no_need_node;
	no_need_node = NULL;

	result_heap_iterator = result_heap->head;
	while (result_heap_iterator->right_brother != NULL) {						 // объединение деревьев одной степени
		if (result_heap_iterator->degree == result_heap_iterator->right_brother->degree) {
			//проверяем на 3 подряд идущих одной степени
			if (result_heap_iterator->right_brother->right_brother != NULL &&
				result_heap_iterator->degree == result_heap_iterator->right_brother->right_brother->degree) {
					result_heap_iterator = result_heap_iterator->right_brother;
			}
			else {
				if (result_heap_iterator->key > result_heap_iterator->right_brother->key)
					result_heap_iterator->swap(result_heap_iterator->right_brother);
				BiNode * tmp = result_heap_iterator->right_brother;
				result_heap_iterator->right_brother = tmp->right_brother;
				tmp->right_brother = result_heap_iterator->left_child;
				result_heap_iterator->left_child = tmp;
				++(result_heap_iterator->degree);
			}
		}
		else
			result_heap_iterator = result_heap_iterator->right_brother;
	}
	this->head = result_heap->head;
	//пересчет минимума
	BiNode * i = this->head;
	this->min = this->head->key;
	while (i != NULL) {
		if (i->key < min)
			this->min = i->key;
		i = i->right_brother;
	}
}

void BinomialHeap::insert(int key) {
	BinomialHeap new_heap(key);
	new_heap.min = key;
	melt(&new_heap);
}

int BinomialHeap::extractMin() {
	if (this->size == 0 || this->head == NULL)
		return INT_MAX;
	int minimum = this->head->key;
	BiNode * before = NULL;					//храним указатель на минимум и на элемент перед минимумом для удаления
	BiNode * before_min = NULL;
	BiNode current_min;
	current_min = this->head;
	BiNode * i = this->head;
	while (i != NULL) {
		if (i->key < minimum) {
			minimum = i->key;
			current_min = *i;
			before_min = before;
		}
		before = i;
		i = i->right_brother;
	}													//удаление найденного корня из списка корней деревьев кучи
	if (before_min == NULL) {							//корень в начале списка
		if (this->head->right_brother == NULL) {
			if (this->head->left_child == NULL) {
				this->head->key = 0;
				this->size = 0;							//пустая куча
				this->min = INT_MAX;
				this->head = NULL;
				return minimum;
			}
			else {
				i = current_min.left_child;						
				BinomialHeap child_heap;						   //сливаем всех детей
				while (i != NULL) {
					BinomialHeap * j = new BinomialHeap(i);
					i = i->right_brother;
					j->head->right_brother = NULL;
					child_heap.melt(j);
				}
				this->head = child_heap.head;					  //сливаем с кучей всех детей
				this->min = child_heap.min;
				--size;
				return minimum;
			}
		}
		else {
			this->head = current_min.right_brother;
		}
	}		
	else {
		before_min->right_brother = current_min.right_brother;  
	}
	//сливаем всех детей
	i = current_min.left_child;						
	BinomialHeap child_heap;
	while (i != NULL) {
		BinomialHeap* j = new BinomialHeap(i);
		i = i->right_brother;
		j->head->right_brother = NULL;
		child_heap.melt(j);
	}
	//сливаем с кучей всех детей
	this->size -= child_heap.size;//уменьшаем size, так как он добавится в melt
	this->melt(&child_heap);

	current_min.left_child = current_min.right_brother = NULL;
	i = before = before_min = NULL;
	--size;
	return minimum;
}	
