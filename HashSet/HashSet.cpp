#include "stdafx.h"

#define ui32 unsigned int

int _tmain(int argc, _TCHAR* argv[])
{
	std::vector<ui32> keys;
	Test test;
	for (ui32 i = 1; i < 10; ++i) {		  
		for (ui32 j = 0; j < 20; ++j)	  //тестируем init по 20 раз на каждый тест
			test.initTest(i);
		std::cout << "init with test number " << i << " is alright" << std::endl;
	}
	test.isPossibleKeyTest();
	std::cout << "isPossibleKey is alright" << std::endl;
	for (ui32 i = 1; i < 5; ++i) {
		test.otherTest(i);
		std::cout << "other Test number " << i << " is alright" << std::endl;
	}
	return 0;
}