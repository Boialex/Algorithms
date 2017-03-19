

#include "stdafx.h"

#include "IArray.h"
#include "ArrayOnTree.h"
#include "MyVector.h"

IArray * createArray(array_type type) {
	if (type == ht_Vector)
		return new MyVector();
	if (type == ht_ArrayOnTree)
		return new ArrayOnTree();
	return NULL;
}