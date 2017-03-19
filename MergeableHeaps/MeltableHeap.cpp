#include "MeltableHeap.h"
#include "CBinomialHeap.h"
#include "CLeftistAndSkewHeap.h"

IMeltableHeap* createHeap(heap_type type) {
	if (type == ht_BinomialHeap)
		return new BinomialHeap();
	if (type == ht_LeftistHeap)
		return new LeftistHeap();
	if (type == ht_SkewHeap)
		return new SkewHeap();
	return NULL;
}
