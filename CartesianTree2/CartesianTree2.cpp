// CartesianTree2.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include "IArray.h"

bool Check(IArray * first, IArray * second) {
	std::vector<int> first_array;
	std::vector<int> second_array;
	first->VectorOutput(first_array);
	second->VectorOutput(second_array);
	for (int i = 0; i < first_array.size(); ++i)
		if (first_array[i] != second_array[i])
			return false;
	return true;
}

void test(int max_element, int number_of_tests, IArray * tree, IArray * my_vector) {
	while (number_of_tests--) {
		if (tree->Size() == 0) {
			int element = rand() % max_element;
			tree->Insert(element, 0);
			my_vector->Insert(element, 0);
			continue;
		}
		short unsigned int a = rand() % 4;
		int element;
		size_t position, l, r;
		int tree_sum, vector_sum;
		bool tree_permut, vector_permut;
		switch(a) {
			case 0:
				element = rand() % max_element;
				position = rand() % (tree->Size() + 1);
				tree->Insert(element, position);
				my_vector->Insert(element, position);
				break;
			case 1:
				element = rand() % max_element;
				position = rand() % tree->Size();
				tree->Assign(element, position);
				my_vector->Assign(element, position);
				break;
			case 2:
				l = rand() % tree->Size();
				r = rand() % (tree->Size() + 1);
				while (r <= l)
					r = rand() % (tree->Size() + 1);
				tree_sum = tree->Sum(l, r);
				vector_sum = my_vector->Sum(l, r);
				if (tree_sum != vector_sum)
					throw "error";
				break;
			case 3:
				l = rand() % tree->Size();
				r = rand() % (tree->Size() + 1);
				while (r <= l)
					r = rand() % (tree->Size() + 1);
				tree_permut = tree->NextPermutation(l, r);
				vector_permut = my_vector->NextPermutation(l, r);
				if (tree_permut != vector_permut)
					throw "error";
				break;
		}
	}
	if (!Check(tree, my_vector))
		throw "error";
}

int _tmain(int argc, _TCHAR* argv[])
{
	int number_of_tests, max_element; 
	std::cin >> max_element >> number_of_tests;
	while (true) {
		IArray * tree = createArray(ht_ArrayOnTree);
		IArray * vect = createArray(ht_Vector);
		test(max_element, number_of_tests, tree, vect);
		/*tree->Output();
		vect->Output();*/
		delete tree;
		delete vect;
	}
	return 0;
}

