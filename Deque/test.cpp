#include "stdafx.h"
#include "gtest/gtest.h"
#include "Deque.h"

class DequeTest : public testing::Test {
protected:
	virtual void SetUp() {}

	virtual void TearDown() {}

	std::deque<int> stddeq;
	MyDeque<int> mydeq;
	unsigned int number_of_elements_for_tests = 1000;
	int max_element = 100;
	bool check() {
		int count = 0;
		for (size_t i = 0; i < mydeq.size(); ++i) {
			EXPECT_EQ(stddeq[i], mydeq[i]) << "i elements not equal " << i;
			if (stddeq[i] != mydeq[i])
				count++;
		}
		if (count) return false;
		return true;
	}
};

TEST_F(DequeTest, IsEmptyInitially) {
	EXPECT_EQ(0, mydeq.size());
	EXPECT_TRUE(mydeq.is_empty());
}

TEST_F(DequeTest, PushAndOperAndWorks) {
	srand(clock());
	for (size_t i = 0; i < number_of_elements_for_tests / 2; ++i) {
		int element = rand() % max_element;
		mydeq.push_back(element);
		stddeq.push_back(element);
	}
	ASSERT_TRUE(mydeq.size() == stddeq.size());
	ASSERT_TRUE(check());
	for (size_t i = 0; i < number_of_elements_for_tests / 2; ++i) {
		int element = rand() % max_element;
		mydeq.push_front(element);
		stddeq.push_front(element);
	}
	ASSERT_TRUE(mydeq.size() == stddeq.size());
	ASSERT_TRUE(check());
}

TEST_F(DequeTest, FrontWorks) {
	srand(clock());
	for (size_t i = 0; i < number_of_elements_for_tests / 2; ++i) {
		int element = rand() % max_element;
		mydeq.push_back(element);
		stddeq.push_back(element);
		EXPECT_EQ(stddeq.front(), mydeq.front());
	}
	for (size_t i = 0; i < number_of_elements_for_tests / 2; ++i) {
		int element = rand() % max_element;
		mydeq.push_front(element);
		stddeq.push_front(element);
		EXPECT_EQ(stddeq.front(), mydeq.front());
	}
}

TEST_F(DequeTest, BackWorks) {
	srand(clock());
	for (size_t i = 0; i < number_of_elements_for_tests / 2; ++i) {
		int element = rand() % max_element;
		mydeq.push_back(element);
		stddeq.push_back(element);
		EXPECT_EQ(stddeq.back(), mydeq.back());
	}
	for (size_t i = 0; i < number_of_elements_for_tests / 2; ++i) {
		int element = rand() % max_element;
		mydeq.push_front(element);
		stddeq.push_front(element);
		EXPECT_EQ(stddeq.back(), mydeq.back());
	}
}

TEST_F(DequeTest, PopWorks) {
	srand(clock());
	for (size_t i = 0; i < number_of_elements_for_tests; ++i) {
		int element = rand() % max_element;
		mydeq.push_back(element);
		stddeq.push_back(element);
	}
	for (size_t i = 0; i < number_of_elements_for_tests - 1; ++i) {
		EXPECT_EQ(stddeq.front(), mydeq.front());
		EXPECT_EQ(stddeq.back(), mydeq.back());
		mydeq.pop_front();
		stddeq.pop_front();
	}
	for (size_t i = 0; i < number_of_elements_for_tests; ++i) {
		int element = rand() % max_element;
		mydeq.push_back(element);
		stddeq.push_back(element);
	}
	for (size_t i = 0; i < number_of_elements_for_tests - 1; ++i) {
		EXPECT_EQ(stddeq.front(), mydeq.front());
		EXPECT_EQ(stddeq.back(), mydeq.back());
		mydeq.pop_back();
		stddeq.pop_back();
	}
}

TEST_F(DequeTest, IteratorWithSortWorks) {
	std::vector<int> stdvector;
	srand(clock());
	for (size_t i = 0; i < number_of_elements_for_tests; ++i) {
		int element = rand() % max_element;
		mydeq.push_back(element);
		stdvector.push_back(element);
	}
	std::sort(mydeq.begin(), mydeq.end());
	std::sort(stdvector.begin(), stdvector.end());
	EXPECT_TRUE(std::equal(mydeq.begin(), mydeq.end(), stdvector.begin()));
}

TEST_F(DequeTest, ReverseIteratorsWorks) {
	int value = 0;
	for (size_t i = 0; i < number_of_elements_for_tests; ++i) {
		mydeq.push_back(value++);
		mydeq.push_back(value++);
		mydeq.pop_front();
	}
	for (std::reverse_iterator< MyDeque<int>::const_iterator > it = mydeq.crbegin(); it != mydeq.crend(); ++it) {
		EXPECT_EQ(--value, *it);
	}
}

TEST_F(DequeTest, NoConstIteratorCheck) {
	for (size_t i = 0; i < number_of_elements_for_tests; ++i) 
		mydeq.push_back(i);
	for (MyDeque<int>::iterator it = mydeq.begin(); it != mydeq.end(); ++it)
		*it = -1;
	for (MyDeque<int>::const_iterator it = mydeq.cbegin(); it != mydeq.cend(); ++it)
		EXPECT_EQ(-1, *it);
	std::vector<int> my;
	std::vector<int>::const_iterator it = my.begin();
}

TEST_F(DequeTest, CheckAmortisedTime) {
	int element;
	//std::vector<double> amortized_time;
	//double avarage = 0.17;
	//long long tests = 0;
	for (long long number_of_tests = 1000; number_of_tests < 1000000; number_of_tests += 1000) {
		MyDeque<int> deq;
		srand(clock());
		//long long i = number_of_tests;
		//tests++;
		long long time = clock();
		element = rand() % max_element;
		deq.push_back(element);
		for (long long i = 0; i < number_of_tests / 2 - 1; ++i) {
			short int a = rand() % 2;
			if (a) {
				element = rand() % max_element;
				deq.push_back(element);
			}
			else {
				element = rand() % max_element;
				deq.push_front(element);
			}
		}
		for (long long i = 0; i < number_of_tests / 2; ++i) {
			short int a = rand() % 2;
			if (a)
				deq.pop_back();
			else
				deq.pop_front();
		}
		/*while (i--) {
		if (deq.is_empty()) {
		element = rand() % max_element;
		deq.push_back(element);
		continue;
		}
		short unsigned int a = rand() % 4;
		int element;
		switch (a) {
		case 0:
		element = rand() % max_element;
		deq.push_back(element);
		break;
		case 1:
		element = rand() % max_element;
		deq.push_front(element);
		break;
		case 2:
		deq.pop_back();
		break;
		case 3:
		deq.pop_back();
		break;
		}
		}*/
		time = clock() - time;
		//std::cout << time << '\n';
		//amortized_time.push_back(time * 1000.0 / number_of_tests);
		//avarage += time * 1000.0 / number_of_tests;
		EXPECT_LE(time * 1000.0 / number_of_tests, 1) << "number or operations = " << number_of_tests;
	}
	//std::cout << avarage / tests << '\n';
	//for (std::vector<double>::iterator it = amortized_time.begin(); it != amortized_time.end(); ++it)
	//	EXPECT_DOUBLE_EQ(avarage, *it, 0.1);
}