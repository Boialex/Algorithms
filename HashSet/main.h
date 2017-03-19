#pragma once
#include "stdafx.h"
#include <set>

namespace HashConst {
	const long long MAX_UI32 = 4294967296;
	const unsigned int MEMORY_CONST = 3;
	const unsigned long long PRIME = MAX_UI32 + 15;
	const unsigned int NUMBER_OF_ATTEMPTS = 100;
	const unsigned int MAX_N = 100;
	const unsigned int SMALL_N = 5;
}

#define ui32 unsigned int

class ISet {
	virtual void init(const std::vector<ui32>& keys) = 0;
	virtual bool has(ui32 key) const = 0;
	virtual bool isPossibleKey(ui32 key) const = 0;
	virtual void insert(ui32 key) = 0;
	virtual void erase(ui32 key) = 0;
	virtual ui32 size() const = 0;
};

class Busket : public ISet {
private:
	unsigned long long a, b;
	std::vector<bool> key_is_pushed;
	std::vector<bool> key_is_initialized;
	ui32 size_of_table = 0;
	ui32 h(ui32 key) const;
public:
	std::vector<ui32> keys_in_cell;
	void init(const std::vector<ui32> & keys);
	bool isPossibleKey(ui32 key) const;
	bool has(ui32 key) const;
	void insert(ui32 key);
	void erase(ui32 key);
	ui32 size() const;
};

class PerfectHashSet : public ISet {
private:
	unsigned long long a, b;
	ui32 size_of_table = 0;
	std::vector<Busket> table;

	ui32 h(ui32 key) const;
	void print(Busket t);
	bool check(std::vector<ui32>::iterator begin, std::vector<ui32>::iterator end);
	bool tableIsLinear(std::vector<Busket>::iterator begin, std::vector<Busket>::iterator end, const ui32 n);
	bool checkEachWithEach(std::vector<ui32>::iterator begin, std::vector<ui32>::iterator end);
	bool checkNeighbors(std::vector<ui32>::iterator begin, std::vector<ui32>::iterator end);
public:
	PerfectHashSet() : a(0), b(0) {};
	void init(const std::vector<ui32>& keys);
	bool isPossibleKey(ui32 key) const;
	bool has(ui32 key) const;
	void insert(ui32 key);
	void erase(ui32 key);
	ui32 size() const;
};

class Set : public ISet {
private:
	std::set<ui32> inner_set;
public:
	void init(const std::vector<ui32>& keys);
	bool has(ui32 key) const;
	bool isPossibleKey(ui32 key) const;
	void insert(ui32 key);
	void erase(ui32 key);
	ui32 size() const;
};

class Test {
private:
	const ui32 BIG_KEY = HashConst::MAX_UI32 - HashConst::MAX_UI32 / 10;
	const ui32 MAX_NUMBER_OF_OPERATIONS = 20;
public:
	void input(std::vector<ui32>& keys, ui32 number_of_test);
	void initTest(ui32 number_of_test);
	void isPossibleKeyTest();
	void otherTest(ui32 number_of_test);
};