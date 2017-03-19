// stdafx.h: включаемый файл для интерфейса

#pragma once

class IMeltableHeap {
public:
	virtual int getMinimum() = 0;			   //минимальный ключ
	virtual int extractMin() = 0;			   //вырезаем минимальный ключ
	virtual void insert(int) = 0;			   //вставляем ключ
	virtual void melt(IMeltableHeap*) = 0;     //сливаем 2 кучи
	virtual unsigned int getSize() = 0;		   //размер
	virtual ~IMeltableHeap() {};
};

enum heap_type {
	ht_BinomialHeap,
	ht_LeftistHeap,
	ht_SkewHeap
};

IMeltableHeap* createHeap(heap_type type);

