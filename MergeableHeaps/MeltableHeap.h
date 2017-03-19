// stdafx.h: ���������� ���� ��� ����������

#pragma once

class IMeltableHeap {
public:
	virtual int getMinimum() = 0;			   //����������� ����
	virtual int extractMin() = 0;			   //�������� ����������� ����
	virtual void insert(int) = 0;			   //��������� ����
	virtual void melt(IMeltableHeap*) = 0;     //������� 2 ����
	virtual unsigned int getSize() = 0;		   //������
	virtual ~IMeltableHeap() {};
};

enum heap_type {
	ht_BinomialHeap,
	ht_LeftistHeap,
	ht_SkewHeap
};

IMeltableHeap* createHeap(heap_type type);

