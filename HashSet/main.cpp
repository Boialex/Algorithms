#include "stdafx.h"
#include <iostream>
#include <fstream>

#define ui32 unsigned int

std::random_device random_engine;

using namespace HashConst;
std::uniform_int_distribution<unsigned long long> RAND = std::uniform_int_distribution<unsigned long long>
         (static_cast<unsigned long long>(0), PRIME - 1);

ui32 Busket::h(ui32 key) const{
	unsigned long long a1 = a / MAX_UI32;
	unsigned long long b1 = a % MAX_UI32;
	unsigned long long h_k = (MAX_UI32 * a1 * key) % PRIME + (b1 * key) % PRIME;
	ui32 value = static_cast<ui32>((h_k + b) % PRIME % keys_in_cell.size());
	return value;														
}
void Busket::init(const std::vector<ui32> & keys) {
	keys_in_cell.clear();
	key_is_initialized.clear();
	key_is_pushed.clear();
	key_is_initialized.resize(keys.size() * keys.size());
	keys_in_cell.resize(keys.size() * keys.size());
	key_is_pushed.resize(keys.size() * keys.size());
	bool have_smth_to_hash = true;

	while (have_smth_to_hash) {
		a = RAND(random_engine);
		b = RAND(random_engine);
		for (std::vector<ui32>::const_iterator j = keys.begin(); j < keys.end(); ++j) {
			ui32 value = h(*j);
			//std::cout << a << ' ' << b << ' ' << *j << ' ' << value << std::endl;
			if (key_is_initialized[value]) {
				if (keys_in_cell[value] == (*j))
					throw "Found Equal Keys";
				key_is_initialized.assign(key_is_initialized.size(), false);
				keys_in_cell.assign(keys_in_cell.size(), 0);
				break;													
			}
			else {
				keys_in_cell[value] = (*j);
				key_is_initialized[value] = true;
				if (j == keys.end() - 1)
					have_smth_to_hash = false;
			}
		}
	}
}
bool Busket::isPossibleKey(ui32 key) const {
	return key_is_initialized[h(key)];
}
bool Busket::has(ui32 key) const {
	return key_is_pushed[h(key)];
}
void Busket::insert(ui32 key) {
	if (key_is_initialized[h(key)]) {
		if (key != keys_in_cell[h(key)])
			throw "COLLISION WITH IMPOSSIBLE KEY";
		else {
			if (!key_is_pushed[h(key)]) {
				key_is_pushed[h(key)] = true;
				++size_of_table;
			}
		}
	}
	else
		throw "IMPOSSIBLE KEY";
}
void Busket::erase(ui32 key) {
	if (key_is_initialized[h(key)]) {
		if (key != keys_in_cell[h(key)])
			throw "COLLISION WITH IMPOSSIBLE KEY";
		else {
			if (key_is_pushed[h(key)]) {
				key_is_pushed[h(key)] = false;
				--size_of_table;
			}
		}
	}
	else
		throw "KEY IS NOT GIVEN";
}
ui32 Busket::size() const{
	return size_of_table;
}

ui32 PerfectHashSet::h(ui32 key) const {
	ui32 a1 = static_cast<ui32>(a / MAX_UI32), b1 = static_cast<ui32>(a % MAX_UI32);
	unsigned long long h_k = (MAX_UI32 * static_cast<unsigned long long>(a1 * key)) % PRIME +
										 static_cast<unsigned long long>(b1 * key) % PRIME;
	ui32 value = static_cast<ui32>((h_k + b) % PRIME % table.size());
	return value;
}
void PerfectHashSet::print(Busket t) {
	std::fstream cout;
	cout.open("Output.txt", std::ios_base::app);
	//cout << t.keys_in_cell.size() << ", (" << t.a << ',' << t.b << ") : ";
	for (ui32 i = 0; i < t.keys_in_cell.size(); ++i) {
		cout << t.keys_in_cell[i] << " | ";
	}
	cout << std::endl;
	cout.close();
}
bool PerfectHashSet::check(std::vector<ui32>::iterator begin, std::vector<ui32>::iterator end) {
	for (std::vector<ui32>::iterator i = begin; i < end - 1; ++i) {
		if (*i == *(i + 1))
			return false;
	}
	return true;
}
bool PerfectHashSet::tableIsLinear(std::vector<Busket>::iterator begin, std::vector<Busket>::iterator end, const ui32 n) {
	unsigned long long sum = 0;
	unsigned long long max_sum = MEMORY_CONST * n;
	for (std::vector<Busket>::iterator i = begin; i < end; ++i) {
		sum += i->keys_in_cell.size() * i->keys_in_cell.size();
		if (sum > max_sum)
			return false;
	}
	return true;
}
bool PerfectHashSet::checkNeighbors(std::vector<ui32>::iterator begin, std::vector<ui32>::iterator end) {
	if (end - begin > 1)
		for (std::vector<ui32>::iterator i = begin; i < end - 1; ++i) {
			if (*i == *(i + 1))
				return false;
		}
	if (end - begin == 3)
		if (*begin == *(end - 1))
			return false;
	return true;
}
void PerfectHashSet::init(const std::vector<ui32> & keys) {
	table.clear();
	if (keys.size() == 0)
		throw "NO KEYS GIVEN";
	table.resize(keys.size());
	ui32 current_attempt_number = 0;
	while (current_attempt_number < NUMBER_OF_ATTEMPTS) {
		a = RAND(random_engine);
		b = RAND(random_engine);
		for (std::vector<Busket>::iterator i = table.begin(); i < table.end(); ++i)
			i->keys_in_cell.clear();
		for (std::vector<ui32>::const_iterator i = keys.begin(); i < keys.end(); ++i) {
			ui32 value = h(*i);
			table[value].keys_in_cell.push_back(*i);
		}
		if (!tableIsLinear(table.begin(), table.end(), table.size()))
			for (std::vector<Busket>::iterator i = table.begin(); i < table.end(); ++i) {
				if (!checkNeighbors(i->keys_in_cell.begin(), i->keys_in_cell.end())) {
					throw "Found Equal Keys in main table";
				}
			}
		else
			current_attempt_number = NUMBER_OF_ATTEMPTS;
		++current_attempt_number;
	}
	for (std::vector<Busket>::iterator i = table.begin(); i < table.end(); ++i) {
		if (i->keys_in_cell.size() == 0)
			continue;
		std::vector<ui32> buffer_for_keys;
		buffer_for_keys.resize(i->keys_in_cell.size());
		copy(i->keys_in_cell.begin(), i->keys_in_cell.end(), buffer_for_keys.begin());
		i->init(buffer_for_keys);
	}
}
bool PerfectHashSet::isPossibleKey(ui32 key) const {
	if (table[h(key)].keys_in_cell.size() != 0)
		return table[h(key)].isPossibleKey(key);
	else
		throw "KEY WASN'T GIVEN";
}
bool PerfectHashSet::has(ui32 key) const {
	if (table[h(key)].keys_in_cell.size() != 0)
		return table[h(key)].has(key);
	else
		throw "KEY WASN'T GIVEN";
}
void PerfectHashSet::insert(ui32 key) {
	ui32 value = h(key);
	if (table[value].keys_in_cell.size() != 0) {
		size_of_table -= table[value].size();
		table[h(key)].insert(key);
		size_of_table += table[value].size();
	}
	else 
		throw "KEY WASN'T GIVEN";
}
void PerfectHashSet::erase(ui32 key) {
	ui32 value = h(key);
	if (table[value].keys_in_cell.size() != 0) {
		size_of_table -= table[value].size();
		table[h(key)].erase(key);
		size_of_table += table[value].size();
	}
	else
		throw "KEY WASN'T GIVEN";
}
ui32 PerfectHashSet::size() const {
	return size_of_table;
}



void Test::input(std::vector<ui32>& keys, ui32 number_of_test) {
	if (keys.size() == 0)
		return;
	std::set<ui32> keys_pushed;
	ui32 k0, k1, sq_root_of_n;
	switch (number_of_test) {
	case 1:
	case 2:
		for (std::vector<ui32>::iterator i = keys.begin(); i < keys.end(); ++i)
			*i = RAND(random_engine) % MAX_UI32;
		std::sort(keys.begin(), keys.end());
		for (std::vector<ui32>::iterator i = keys.begin(); i < keys.end() - 1; ++i) {
			while (*i == *(i + 1))
				keys.erase(i + 1);
		}
		break;
	case 3:
		k0 = RAND(random_engine) % MAX_UI32;
		for (std::vector<ui32>::iterator i = keys.begin(); i < keys.end(); ++i) {
			*i = k0;
		}
		break;
	case 4:
		k0 = RAND(random_engine) % MAX_UI32;
		do {
			k1 = RAND(random_engine) % MAX_UI32;
		} while (k1 == k0);
		for (std::vector<ui32>::iterator i = keys.begin(); i < keys.end() - 1; i += 2) {
			*i = k0;
			*(i + 1) = k1;
		}
		break;
	case 5:
		if (keys.size() % 2 == 1)
			keys.resize(keys.size() + 1);
		for (std::vector<ui32>::iterator i = keys.begin(); i < keys.end() - 1; i += 2) {
			ui32 k;
			do {
				k = RAND(random_engine) % MAX_UI32;
			} while (keys_pushed.count(k));
			keys_pushed.insert(k);
			*i = k;
			*(i + 1) = k;
		}
		break;
	case 6:
		for (std::vector<ui32>::iterator i = keys.begin(); i < keys.end(); ++i)
			*i = RAND(random_engine) % MAX_UI32;
		std::sort(keys.begin(), keys.end());
		for (std::vector<ui32>::iterator i = keys.begin(); i < keys.end() - 1; ++i) {
			while (*i == *(i + 1))
				keys.erase(i + 1);
		}
		if (keys.size() > 1)
			*(keys.end() - 1) = *keys.begin();
		break;
	case 7:
		for (std::vector<ui32>::iterator i = keys.begin(); i < keys.end(); ++i)
			*i = RAND(random_engine) % MAX_UI32;
		std::sort(keys.begin(), keys.end());
		for (std::vector<ui32>::iterator i = keys.begin(); i < keys.end() - 1; ++i) {
			while (*i == *(i + 1))
				keys.erase(i + 1);
		}
		if (keys.size() > 1) {
			*(keys.end() - 2) = *keys.begin();
			*(keys.end() - 1) = *(keys.begin() + 1);
		}
		break;
	case 8:
		sq_root_of_n = static_cast<ui32>(sqrt(keys.size() - 1));
		keys.resize(sq_root_of_n * sq_root_of_n + 1);
		for (std::vector<ui32>::iterator i = keys.begin(); i < keys.begin() + sq_root_of_n; ++i) {
			ui32 k;
			do {
				k = RAND(random_engine) % MAX_UI32;
			} while (keys_pushed.count(k));
			keys_pushed.insert(k);
			for (std::vector<ui32>::iterator j = i; j < keys.end(); j += sq_root_of_n)
				*j = k;
		}
		break;
	case 9:
		for (std::vector<ui32>::iterator i = keys.begin(); i < keys.end(); ++i) {
			do {
				k0 = RAND(random_engine) % MAX_UI32;
			} while ((k0 < BIG_KEY) && keys_pushed.count(k0));
			keys_pushed.insert(k0);
			*i = k0;
		}
		break;
	default:
		break;
	}
}

void Test::initTest(ui32 number_of_test) {
		std::vector<ui32> keys;
		ui32 n;
		PerfectHashSet hash_set;
		switch (number_of_test) {
		case 1:
			try {
				n = RAND(random_engine) % SMALL_N; 
				keys.resize(n);
				input(keys, number_of_test);
				do {
					hash_set.init(keys);
				} while (std::next_permutation(keys.begin(), keys.end()));
			}
			catch (const char* s) {
				std::fstream cout;
				cout.open("Output.txt", std::ios_base::app);
				cout << s << ' ' << number_of_test << std::endl;
				cout.close();
			}
			break;
		case 2:
		case 3:
		case 9:
			for (int i = 0; i < 5; ++i) {
				try {
					//n = RAND(random_engine) % MAX_N;
					n = 1000000;
					keys.resize(n);
					input(keys, number_of_test);
					std::cout << "init ";
					hash_set.init(keys);
				} 
				catch (const char* s) {
					std::fstream cout;
					cout.open("Output.txt", std::ios_base::app);
					cout<< s << ' ' << number_of_test << std::endl;
					cout.close();
				}
			}
			break;
		case 4:
		case 5:
		case 6:
		case 7:
			for (int i = 0; i < 5; ++i) {
				n = RAND(random_engine) % MAX_N;
				keys.resize(n);
				input(keys, number_of_test);
				try	 {
					hash_set.init(keys);
				}
				catch (const char* s) {
					std::fstream cout;
					cout.open("Output.txt", std::ios_base::app);
					cout << s << " in usual order, test number =  " << number_of_test << std::endl;
					cout.close();
				}
				for (ui32 i = 0; i < SMALL_N; ++i) {
					std::random_shuffle(keys.begin(), keys.end());
					try	 {
						hash_set.init(keys);
					}
					catch (const char* s) {
						std::fstream cout;
						cout.open("Output.txt", std::ios_base::app);
						cout << s << " in random_shuffle order, test number =  " << number_of_test << std::endl;
						cout.close();
					}
				}
				if (keys.size() <= SMALL_N) {
					std::sort(keys.begin(), keys.end());
					do {
						try	 {
							hash_set.init(keys);
						}
						catch (const char* s) {
							std::fstream cout;
							cout.open("Output.txt", std::ios_base::app);
							cout << s << " all permutations, test number =  " << number_of_test << std::endl;
							cout.close();
						}
					} while (std::next_permutation(keys.begin(), keys.end()));
				}
			}
			break;
		case 8:
			for (ui32 k = 0; k < 5; ++k) {
				do {
					n = RAND(random_engine) % MAX_N;
				} while (n < 1);
				keys.resize(n);
				input(keys, number_of_test);
				try	 {
					hash_set.init(keys);
				}
				catch (const char* s) {
					std::fstream cout;
					cout.open("Output.txt", std::ios_base::app);
					cout << s << " in usual order, test number =  " << number_of_test << std::endl;
					cout.close();
				}
				for (ui32 i = 0; i < SMALL_N; ++i) {
					for (std::vector<ui32>::iterator j = keys.begin(); j <= keys.end() - static_cast<ui32>(sqrt(keys.size()));
						j += static_cast<ui32>(sqrt(keys.size()))) {
						std::random_shuffle(j, j + static_cast<ui32>(sqrt(keys.size())));
						try	 {
							hash_set.init(keys);
						}
						catch (const char* s) {
							std::fstream cout;
							cout.open("Output.txt", std::ios_base::app);
							cout << s << " in random_shuffle of parts order, test number =  " << number_of_test << std::endl;
							cout.close();
						}
					}
				}
				for (int i = 0; i < SMALL_N; ++i) {
					std::random_shuffle(keys.begin(), keys.end());
					try	 {
						hash_set.init(keys);
					}
					catch (const char* s) {
						std::fstream cout;
						cout.open("Output.txt", std::ios_base::app);
						cout << s << " in random_shuffle order, test number =  " << number_of_test << std::endl;
						cout.close();
					}
				}
				if (keys.size() <= SMALL_N) {
					std::sort(keys.begin(), keys.end());
					do {
						try	 {
							hash_set.init(keys);
						}
						catch (const char* s) {
							std::fstream cout;
							cout.open("Output.txt", std::ios_base::app);
							cout << s << " all permutations, test number =  " << number_of_test << std::endl;
							cout.close();
						}
					} while (std::next_permutation(keys.begin(), keys.end()));
				}
			}
			break;
		default:
			break;
		}
}

void Test::isPossibleKeyTest() {
	ui32 n;
	n = RAND(random_engine) % MAX_N;
	std::vector<ui32> keys;
	std::set<ui32> keys_set;
	keys.resize(n);
	input(keys, 2);
	PerfectHashSet hash_set;
	try {
		hash_set.init(keys);
		for (std::vector<ui32>::iterator i = keys.begin(); i < keys.end(); ++i) {
			keys_set.insert(*i);
			if (!hash_set.isPossibleKey(*i))
				throw "Error with given keys";
		}
		for (ui32 i = 0; i < keys.size(); ++i) {
			ui32 k;
			do {
				k = RAND(random_engine) % MAX_UI32;
			} while (keys_set.count(k));
			if (hash_set.isPossibleKey(k))
				throw "Error with stranger key";
		}
	}
	catch (const char* s) {
		std::fstream cout;
		cout.open("Output.txt", std::ios_base::app);
		cout << s << " isPossibleKey test" << std::endl;
		cout.close();
	}
}
void Test::otherTest(ui32 number_of_test) {
	PerfectHashSet hash_set;
	Set my_set;
	std::vector<ui32> keys;
	ui32 n;
	n = RAND(random_engine) % MAX_N;
	keys.resize(n);
	input(keys, 2);
	ui32 number_of_operations = RAND(random_engine) % MAX_NUMBER_OF_OPERATIONS;
	try {
		hash_set.init(keys);
		switch (number_of_test) {
		case 1:
			for (std::vector<ui32>::iterator i = keys.begin(); i < keys.end(); ++i) {
				if (hash_set.has(*i))
					std::cout << "ERROR WITH HAS BEFORE /n";
				ui32 size0 = hash_set.size();
				hash_set.insert(*i);
				if (!hash_set.has(*i))
					std::cout << "ERROR WITH HAS BEFORE /n";
				ui32 size1 = hash_set.size();
				hash_set.erase(*i);
				if (hash_set.has(*i))
					std::cout << "ERROR WITH HAS BEFORE /n";
				ui32 size2 = hash_set.size();
				if ((size0 != size1 - 1) || (size2 != size0))
					std::cout << "ERROR WITH SIZE /n";
			}
			break;
		case 2:
			for (std::vector<ui32>::iterator i = keys.begin(); i < keys.end(); ++i) {
				ui32 size0 = hash_set.size();
				hash_set.insert(*i);
				if (hash_set.size() != size0 + 1)
					std::cout << "ERROR IN SECOND TEST INSERT" << std::endl;
				hash_set.insert(*i);
				if (hash_set.size() != size0 + 1)
					std::cout << "ERROR IN SECOND TEST INSERT" << std::endl;
				hash_set.erase(*i);
				if (hash_set.size() != size0)
					std::cout << "ERROR IN SECOND TEST INSERT" << std::endl;
				hash_set.erase(*i);
				if (hash_set.size() != size0)
					std::cout << "ERROR IN SECOND TEST INSERT" << std::endl;
			}
			break;
		case 3:
			for (ui32 i = 0; i < number_of_operations; ++i) {
				ui32 k = RAND(random_engine) % keys.size();
				if (k % 2) {
					hash_set.insert(keys[k]);
					my_set.insert(keys[k]);
				}
				else {
					hash_set.erase(keys[k]);
					my_set.erase(keys[k]);
				}
				for (std::vector<ui32>::iterator i = keys.begin(); i < keys.end(); ++i)
					if (hash_set.has(*i) != my_set.has(*i))
						std::cout << "ERROR IN THIRD TEST WITH SET" << std::endl;
			}
			break;
		case 4:
			ui32 strange_key = RAND(random_engine) % MAX_UI32;
			hash_set.insert(strange_key);
			break;
		}
	}
	catch (const char* s) {
		std::fstream cout;
		cout.open("Output.txt", std::ios_base::app);
		cout << s << " other tests " << std::endl;
		cout.close();
	}
}

void Set::init(const std::vector<ui32>& keys) {
	inner_set.clear();
}
bool Set::has(ui32 key) const {
	return inner_set.count(key) != 0;
}
bool Set::isPossibleKey(ui32 key) const {
	return true;
}
void Set::insert(ui32 key) {
	inner_set.insert(key);
}
void Set::erase(ui32 key) {
	inner_set.erase(key);
}
ui32 Set::size() const {
	return inner_set.size();
}