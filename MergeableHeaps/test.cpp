#include "test.h"

std::random_device random_engine;
std::uniform_int_distribution<int> RAND = std::uniform_int_distribution<int>(static_cast<int>(0), TestConst::MAX_KEY - 1);


void Test::addHeap(int key) {
	std::fstream cout;
	cout.open("Output.txt", std::ios_base::app);
	cout << "addHeap : " << key << std::endl;

	bi_heap.push_back(createHeap(ht_BinomialHeap));
	bi_heap[bi_heap.size() - 1]->insert(key);

	left_heap.push_back(createHeap(ht_LeftistHeap));
	left_heap[left_heap.size() - 1]->insert(key);

	skew_heap.push_back(createHeap(ht_SkewHeap));
	skew_heap[skew_heap.size() - 1]->insert(key);

	cout.close();
}

void Test::insert(int index, int key) {						//сравниваем времена работы вставки ключа в кучу
	long long time_bi = 0, time_left = 0, time_skew = 0;
	std::fstream cout;
	cout.open("Output.txt", std::ios_base::app);
	cout << "insert : index = " << index << ", key = " << key << std::endl;
	cout.close();
	time_left = clock();
	left_heap[index]->insert(key);
	time_left = clock() - time_left;
	leftist_time.insert_time += time_left;

	time_bi = clock();										
	bi_heap[index]->insert(key);
	time_bi = clock() - time_bi;
	binary_time.insert_time += time_bi;	 
	
	time_skew = clock();
	skew_heap[index]->insert(key);
	time_skew = clock() - time_skew;
	skew_time.insert_time += time_skew;
	
	if (skew_heap[index]->getMinimum() != left_heap[index]->getMinimum() || skew_heap[index]->getMinimum() != bi_heap[index]->getMinimum())
		throw "WRONG insertion min";
	if (skew_heap[index]->getSize() != left_heap[index]->getSize() || skew_heap[index]->getSize() != bi_heap[index]->getSize())
		throw "WRONG insertion size";
}				
															 
void Test::extractMin(int index) {							 //изъ€ть минимум и сравнить врем€ работы и результат
	long long time_bi = 0, time_left = 0, time_skew = 0;
	int min_bi = 0, min_left = 0, min_skew = 0;
	std::fstream cout;
	cout.open("Output.txt", std::ios_base::app);
	cout << "extract : index = " << index << std::endl;
	cout.close();
	time_bi = clock();
	min_bi = bi_heap[index]->extractMin();
	time_bi = clock() - time_bi;
	binary_time.extractMin_time += time_bi;

	time_left = clock();
	min_left = left_heap[index]->extractMin();
	time_left = clock() - time_left;
	leftist_time.extractMin_time += time_left;
	
	time_skew = clock();
	min_skew = skew_heap[index]->extractMin();
	time_skew = clock() - time_skew;
	skew_time.extractMin_time += time_skew;
	
	if (min_bi != min_left || min_bi != min_skew || min_left != min_skew)
		throw "Wrong mins";
	if (skew_heap[index]->getSize() != left_heap[index]->getSize() || skew_heap[index]->getSize() != bi_heap[index]->getSize()) {
		std::cout << "Wrong sizes = " << bi_heap[index]->getSize() << ' ' << left_heap[index]->getSize() << ' ' << skew_heap[index]->getSize();
		std::cout << std::endl;
		throw "WRONG extract size";
	}
}

void Test::melt(int index1, int index2) {
	if (index1 == index2)
		return;
	//врем€ сли€ние двух куч
	long long time_bi, time_left, time_skew;
	std::fstream cout;
	cout.open("Output.txt", std::ios_base::app);
	cout << "melt : index1 = " << index1 << ", index2 = " << index2 << std::endl;
	cout.close();
	time_bi = clock();
	bi_heap[index1]->melt(bi_heap[index2]);
	time_bi = clock() - time_bi;		   
	bi_heap[index2] = *(bi_heap.end() - 1);
	bi_heap.pop_back();
	binary_time.melt_time += time_bi;

	time_left = clock();
	left_heap[index1]->melt(left_heap[index2]);
	time_left = clock() - time_left;
	left_heap[index2] = *(left_heap.end() - 1);
	left_heap.pop_back();
	leftist_time.melt_time += time_left;

	time_skew = clock();
	skew_heap[index1]->melt(skew_heap[index2]);
	time_skew = clock() - time_skew;
	skew_heap[index2] = *(skew_heap.end() - 1);
	skew_heap.pop_back();
	skew_time.melt_time += time_skew;

	if (skew_heap[index1]->getSize() != left_heap[index1]->getSize() || skew_heap[index1]->getSize() != bi_heap[index1]->getSize())
		throw "WRONG melt size";
	if (skew_heap[index1]->getMinimum() != left_heap[index1]->getMinimum() ||
		skew_heap[index1]->getMinimum() != bi_heap[index1]->getMinimum()) {
		std::cout << "wrong melt mins: " << bi_heap[index1]->getMinimum() << ' ' << left_heap[index1]->getMinimum() << ' '
			<< skew_heap[index1]->getMinimum() << std::endl;
		throw "WRONG melt min";
	}
}

void Test::mainTest() {
	try {
		bi_heap.resize(0);
		left_heap.resize(0);
		skew_heap.resize(0);
		for (int i = 0; i < TestConst::NUMBER_OF_TEST; ++i) {
			unsigned int index1, index2; 
			int key;
			unsigned short kind_of_test = RAND(random_engine) % 4;	 
			if (bi_heap.size() == 0)		 //если нет ни одной кучи, то первым тестом добавим ее
				kind_of_test = 1;
			enum whichTest{ test_insert, test_addHeap, test_melt, test_extractMin };
			switch (kind_of_test) {
			case 0:	//insert
				index1 = rand() % bi_heap.size();
				key = RAND(random_engine);
				insert(index1, key);
				break;
			case 1:	//addHeap
				key = RAND(random_engine);
				addHeap(key);
				break;
			case 2:	 //melt
				index1 = RAND(random_engine) % bi_heap.size();
				index2 = RAND(random_engine) % bi_heap.size();
				if (index1 > index2)
					std::swap(index1, index2);
				melt(index1, index2);
				break;
			case 3:	 //extractMin
				index1 = RAND(random_engine) % bi_heap.size();
				extractMin(index1);
				break;
			default:
				break;
			}
		}
		std::cout << "Numer of tests = " << TestConst::NUMBER_OF_TEST << std::endl;
		//биномиальна€ куча - суммарное врем€ по операци€м
		std::cout << "Binomial Heap : insert = " << static_cast<double>(binary_time.insert_time) / CLOCKS_PER_SEC;
		std::cout << ", extract = " << static_cast<double>(binary_time.extractMin_time) / CLOCKS_PER_SEC;
		std::cout << ", melt =  " << static_cast<double>(binary_time.melt_time) / CLOCKS_PER_SEC << std::endl;
		//левацка€ куча - суммарное врем€ по операци€м
		std::cout << "Leftist Heap : insert = " << static_cast<double>(leftist_time.insert_time) / CLOCKS_PER_SEC;
		std::cout << ", extract = " << static_cast<double>(leftist_time.extractMin_time) / CLOCKS_PER_SEC;
		std::cout << ", melt =  " << static_cast<double>(leftist_time.melt_time) / CLOCKS_PER_SEC << std::endl;
		//коса€ куча - суммарное врем€ по операци€м
		std::cout << "Skew Heap : insert = " << static_cast<double>(skew_time.insert_time) / CLOCKS_PER_SEC;
		std::cout << ", extract = " << static_cast<double>(skew_time.extractMin_time) / CLOCKS_PER_SEC;
		std::cout << ", melt =  " << static_cast<double>(skew_time.melt_time) / CLOCKS_PER_SEC << std::endl;
	}
	catch (const char * s) {
		std::cout << s << std::endl;
	}
	bi_heap.clear();
	left_heap.clear();
	skew_heap.clear();
}





