// Deque.cpp: ���������� ����� ����� ��� ����������� ����������.
//

#include "stdafx.h"
#include "gtest/gtest.h"

int _tmain(int argc, _TCHAR* argv[])
{
	::testing::InitGoogleTest(&argc, argv);
	while (true)
		std::cerr << RUN_ALL_TESTS();
	return 0;
}