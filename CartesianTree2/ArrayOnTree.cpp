#include "stdafx.h"

#include "ArrayOnTree.h"

size_t ArrayOnTree::Size() {
	return CartesianTree::Sizeof(tree);
}

ArrayOnTree::ArrayOnTree(int number_of_elements) {
	for (int i = 0; i < number_of_elements; i++) {
		int element = rand() % 50;
		//std::cin >> element;
		Insert(element, CartesianTree::Sizeof(tree));
	}
}

ArrayOnTree::ArrayOnTree() {
	tree = NULL;
}

void ArrayOnTree::Assign(int key, size_t position) {
	if (position < 0 || position > CartesianTree::Sizeof(tree))
		return;
	Pair cut_left = CartesianTree::Split(tree, position);
	Pair cut_right = CartesianTree::Split(cut_left.second, 1);
	delete cut_right.first;
	cut_right.first = new CartesianTree(key);
	cut_left.second = CartesianTree::Merge(cut_right.first, cut_right.second);
	tree = CartesianTree::Merge(cut_left.first, cut_left.second);
}

int ArrayOnTree::Sum(size_t l, size_t r) {
	if (l > r)
		return 0;
	if (l < 0)
		l = 0;
	if (r > CartesianTree::Sizeof(tree))
		r = CartesianTree::Sizeof(tree);
	Pair cut_left = CartesianTree::Split(tree, l);
	Pair cut_right = CartesianTree::Split(cut_left.second, r - l);
	int sum = CartesianTree::GetInfo(cut_right.first).sum;
	tree = CartesianTree::Merge(cut_left.first, CartesianTree::Merge(cut_right.first, cut_right.second));
	return sum;
}

bool ArrayOnTree::NextPermutation(size_t l, size_t r) {
	if (l > r)
		return 0;
	Pair left_part = CartesianTree::Split(tree, l);
	Pair right_part = CartesianTree::Split(left_part.second, r - l);
	bool all_permutaions = CartesianTree::NextPermutationFull(right_part.first);
	tree = CartesianTree::Merge(left_part.first, CartesianTree::Merge(right_part.first, right_part.second));
	return all_permutaions;
}

void ArrayOnTree::Insert(int key, size_t position) {
	if (position < 0 || position > CartesianTree::Sizeof(this->tree) + 1)
		return;
	Pair tree_part = CartesianTree::Split(tree, position);
	CartesianTree * new_tree = new CartesianTree(key);
	tree_part.first = CartesianTree::Merge(tree_part.first, new_tree);
	tree = CartesianTree::Merge(tree_part.first, tree_part.second);
}

void ArrayOnTree::VectorOutput(std::vector<int> & array) {
	CartesianTree::ArrayInput(tree, &array);
}

void ArrayOnTree::Output() {
	if (tree != NULL)
		tree->Print();
}

CartesianTree::Node::Node() {
	key = cnt = sum = priority = 0;
	min = INT_MAX;
	max = INT_MIN;
	need_reverse = false;
	decreasing = increasing = true;
}

CartesianTree::Node::Node(int key) {
	this->key = key;
	cnt = 1;
	sum = min = max = key;
	priority = rand() % Priority::PRIME;
	need_reverse = false;
	decreasing = increasing = true;
}

void CartesianTree::Print0() {
	Push();
	if (left != NULL)
		left->Print0();
	std::cout << node.key << ' ';
	if (right != NULL)
		right->Print0();
}

void CartesianTree::Print() {
	Print0();
	std::cout << std::endl;
}

CartesianTree::Node CartesianTree::GetInfo(CartesianTree * tree) {
	if (tree == NULL)
		return CartesianTree::Node();
	else {
		tree->Push();
		return tree->node;
	}
}

int Min(int a, int b, int c) {
	return std::min(std::min(a, b), c);
}

int Max(int a, int b, int c) {
	return std::max(std::max(a, b), c);
}

void CartesianTree::Recount() {
	if (left != NULL)
		left->Push();
	if (right != NULL)
		right->Push();
	Node left_node = GetInfo(left);
	Node right_node = GetInfo(right);
	node.cnt = Sizeof(left) + Sizeof(right) + 1;
	node.sum = left_node.sum + right_node.sum + node.key;
	node.min = Min(left_node.min, right_node.min, node.key);
	node.max = Max(left_node.max, right_node.max, node.key);
	node.decreasing = left_node.decreasing
		&& right_node.decreasing
		&& (left_node.min >= right_node.max)
		&& (node.key <= left_node.min)
		&& (node.key >= right_node.max);
	node.increasing = left_node.increasing
		&& right_node.increasing
		&& (left_node.max <= right_node.min)
		&& (node.key >= left_node.max)
		&& (node.key <= right_node.min);
}

size_t CartesianTree::Sizeof(CartesianTree * tree) {
	if (tree == NULL)
		return 0;
	else
		return CartesianTree::GetInfo(tree).cnt;
}

void CartesianTree::Push() {
	if (node.need_reverse == false)
		return;
	else {
		std::swap(left, right);
		node.need_reverse = false;
		std::swap(node.decreasing, node.increasing);
		if (left != NULL) {
			left->node.need_reverse = !(left->node.need_reverse);
		}
		if (right != NULL) {
			right->node.need_reverse = !(right->node.need_reverse);
		}
	}
}

CartesianTree * CartesianTree::Merge(CartesianTree * first, CartesianTree * second) {
	if (first == NULL || second == NULL)
		if (first != NULL)
			return first;
		else
			return second;
	if (first->node.priority > second->node.priority) {
		first->Push();
		first->right = Merge(first->right, second);
		first->Recount();
		return first;
	}
	else {
		second->Push();
		second->left = Merge(first, second->left);
		second->Recount();
		return second;
	}
}

Pair CartesianTree::Split(CartesianTree * tree, int number_of_elements) {
	if (tree == NULL)
		return Pair(NULL, NULL);
	if (number_of_elements < 1)
		return Pair(NULL, tree);
	if (number_of_elements >= Sizeof(tree))
		return Pair(tree, NULL);
	tree->Push();
	if (number_of_elements == Sizeof(tree->left)) {
		CartesianTree * buffer = tree->left;
		tree->left = NULL;
		tree->Recount();
		return Pair(buffer, tree);
	}
	if (number_of_elements == Sizeof(tree->left) + 1) {
		CartesianTree * buffer = tree->right;
		tree->right = NULL;
		tree->Recount();
		return Pair(tree, buffer);
	}
	if (number_of_elements < Sizeof(tree->left)) {
		Pair new_tree = Split(tree->left, number_of_elements);
		tree->left = new_tree.second;
		tree->Recount();
		return Pair(new_tree.first, tree);
	}
	if (number_of_elements > Sizeof(tree->left) + 1) {
		Pair new_tree = Split(tree->right, number_of_elements - Sizeof(tree->left) - 1);
		tree->right = new_tree.first;
		tree->Recount();
		return Pair(tree, new_tree.second);
	}
}

bool CartesianTree::NextPermutationFull(CartesianTree * & tree) {
	if (tree->node.decreasing) {
		tree->node.need_reverse = true;
		return false;
	}
	Pair tree_parts = CartesianTree::SplitDescendingSuffix(tree, INT_MIN);
	tree_parts.second->node.need_reverse = true;
	Pair last_element = Split(tree_parts.first, Sizeof(tree_parts.first) - 1);
	last_element.second->node.key = CartesianTree::BinInsert(tree_parts.second, last_element.second->node.key);
	last_element.second->Recount();
	tree = CartesianTree::Merge(CartesianTree::Merge(last_element.first, last_element.second), tree_parts.second);
	return true;
}

Pair CartesianTree::SplitDescendingSuffix(CartesianTree * tree, int max) {
	if (tree == NULL)
		return Pair(NULL, NULL);
	tree->Push();
	if (tree->node.decreasing)
		if (tree->node.min >= max)
			return Pair(NULL, tree);
		else
			return Pair(tree, NULL);
	CartesianTree::Node right_node = GetInfo(tree->right);
	if (right_node.decreasing == false) {
		Pair new_parts = SplitDescendingSuffix(tree->right, max);
		tree->right = new_parts.first;
		tree->Recount();
		return Pair(tree, new_parts.second);
	}
	else {
		if (right_node.min >= max) {
			if ((tree->node.key >= right_node.max) && (tree->node.key >= max)) {
				Pair new_parts = SplitDescendingSuffix(tree->left, tree->node.key);
				tree->left = new_parts.second;
				tree->Recount();
				return Pair(new_parts.first, tree);
			}
			else {
				CartesianTree * right_son = tree->right;
				tree->right = NULL;
				tree->Recount();
				return Pair(tree, right_son);
			}
		}
		else
			return Pair(tree, NULL);
	}
}

int CartesianTree::BinInsert(CartesianTree * tree, int key) {
	tree->Push();
	int change;
	if (key < tree->node.key) {
		if (GetInfo(tree->left).max <= key) {
			change = tree->node.key;
			tree->node.key = key;
		}
		else {
			change = BinInsert(tree->left, key);
		}
	}
	else {
		if (tree->right != NULL)
			change = BinInsert(tree->right, key);
		else {
			change = tree->node.key;
			tree->node.key = key;
		}
	}
	tree->Recount();
	return change;
}

void CartesianTree::ArrayInput(CartesianTree * tree, std::vector<int> * array) {
	if (tree == NULL)
		return;
	tree->Push();
	ArrayInput(tree->left, array);
	array->push_back(tree->node.key);
	ArrayInput(tree->right, array);
}

ArrayOnTree::~ArrayOnTree() {
	delete tree;
}

CartesianTree::~CartesianTree() {
	delete left;
	delete right;
}