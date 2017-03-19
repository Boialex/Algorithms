

template <class Type>
class MyDeque {
private:
	size_t capacity;																	  //memory size
	size_t number_of_elements;															  //real size
	size_t begin_index;																	  //index of beginnig in array
	Type * array;																		  //dinamic array

	void grow();
	void reduce();
public:
	template <bool is_const = false> 													  //const/ !const Iterator
	class Iterator : public std::iterator < std::random_access_iterator_tag, Type > {
	private:
		typedef typename std::conditional<is_const, const MyDeque*, MyDeque*>::type deque_pointer;
		size_t elem_index;																   //index of element pointed by Iterator
		deque_pointer deque;
	public:
		typedef typename std::conditional<is_const, const Type*, Type*>::type pointer;
		typedef typename std::conditional<is_const, const Type&, Type&>::type reference;

		Iterator() : elem_index(0), deque(NULL) {};
		Iterator(MyDeque * deque, size_t index) : deque(deque), elem_index(index) {};
		Iterator(const MyDeque * deque, size_t index) :deque(deque), elem_index(index) {};
		Iterator(const Iterator & it) : deque(it.deque), elem_index(it.elem_index) {};
																							//Iterator operators
		reference operator * () {
			return deque->array[(deque->begin_index + elem_index) % deque->capacity];
		}
		pointer operator -> () {
			return &(deque->array[(deque->begin_index + elem_index) % deque->capacity]);
		}
		bool operator == (const Iterator &it) const {
			return (it.elem_index == elem_index);
		}
		bool operator != (const Iterator &it) const {
			return (it.elem_index != elem_index);
		}
		bool operator < (const Iterator &it) const {
			return elem_index < it.elem_index;
		}
		bool operator > (const Iterator &it) const {
			return elem_index > it.elem_index;
		}
		bool operator <= (const Iterator &it) const {
			return !(this > it);
		}
		bool operator >= (const Iterator &it) const {
			return !(this < it);
		}
		Iterator& operator ++ () {
			elem_index = (elem_index + 1) % deque->capacity;
			return *this;
		}
		Iterator operator ++ (int) {
			return Iterator(deque, (elem_index++) % deque->capacity);
		}
		Iterator& operator -- () {
			elem_index = (elem_index - 1) % deque->capacity;
			return *this;
		}
		Iterator operator -- (int) {
			return Iterator(deque, (elem_index--) % deque->capacity);
		}
		Iterator operator + (size_t n) const {
			return Iterator(deque, (elem_index + n) % deque->capacity);
		}
		Iterator operator - (size_t n) const {
			return Iterator(deque, (elem_index - n) % deque->capacity);
		}
		int operator - (const Iterator& it)  const {
			return elem_index - it.elem_index;
		}
		Iterator& operator = (const Iterator& it) {
			elem_index = it.elem_index;
			deque = it.deque;
			return *this;
		}
		Type& operator [] (size_t index) {
			return *(*this + n);
		}
	};
	MyDeque();
	MyDeque(Type);
	~MyDeque();
	//main methods: add, delete, size, output...
	void push_front(Type);
	void push_back(Type);
	void pop_back();
	void pop_front();
	void output();
	bool is_empty() const;
	size_t size() const;
	//get elements
	Type front() const;
	Type& front();
	Type back() const;
	Type& back();
	Type& operator[](size_t);
	Type operator[](size_t) const;

	typedef typename MyDeque::Iterator<false> iterator;
	typedef typename MyDeque::Iterator<true> const_iterator;
	//work with Iterator
	iterator begin() {
		return Iterator<false>(this, 0);
	}
	const_iterator begin() const {
		return Iterator<true>(this, 0);
	}
	const_iterator cbegin() const {
		return Iterator<true>(this, 0);
	}
	iterator end() {
		return Iterator<false>(this, number_of_elements);
	}
	const_iterator end() const {
		return Iterator<true>(this, number_of_elements);
	}
	const_iterator cend() const {
		return Iterator<true>(this, number_of_elements);
	}
	//work with reverse Iterator
	std::reverse_iterator<iterator> rbegin() {
		return std::reverse_iterator<iterator>(end());
	}
	std::reverse_iterator<const_iterator> rbegin() const {
		return std::reverse_iterator<const_iterator>(cend());
	}
	std::reverse_iterator<const_iterator> crbegin() const {
		return std::reverse_iterator<const_iterator>(cend());
	}
	std::reverse_iterator<iterator> rend() {
		return std::reverse_iterator<iterator>(begin());
	}
	std::reverse_iterator<const_iterator> rend() const {
		return std::reverse_Iterator<const_iterator>(cbegin());
	}
	std::reverse_iterator<const_iterator> crend() const {
		return std::reverse_iterator<const_iterator>(cbegin());
	}
};

#define My_Type template <class Type>

My_Type MyDeque<Type>::~MyDeque() {
	delete[] array;
}

My_Type MyDeque<Type>::MyDeque() {
	capacity = 1;
	number_of_elements = begin_index = 0;
	array = new Type[1];
}

My_Type MyDeque<Type>::MyDeque(Type element) {
	capacity = 2;
	number_of_elements = 1;
	array = new Type[2];
	array[0] = element;
	begin_index = 0;
}

My_Type void MyDeque<Type>::grow() {
	Type * tmp = array;
	array = new Type[capacity * 2];
	for (size_t i = 0; i < number_of_elements; ++i) {
		array[i] = tmp[(begin_index + i) % capacity];										   //copy to new sized array
	}
	delete[] tmp;
	capacity *= 2;
	begin_index = 0;
}

My_Type void MyDeque<Type>::reduce() {
	Type * tmp = array;
	array = new Type[capacity / 2];
	for (size_t i = 0; i < number_of_elements; ++i) {
		array[i] = tmp[(i + begin_index) % capacity];
	}
	delete[] tmp;
	capacity /= 2;
	begin_index = 0;
}

My_Type void MyDeque<Type>::push_front(Type element) {
	if (number_of_elements + 1 == capacity) {												   //realloc
		grow();
	}
	if (begin_index == 0)																   //add element
		begin_index = capacity - 1;
	else
		begin_index--;
	array[begin_index] = element;
	number_of_elements++;
}

My_Type void MyDeque<Type>::push_back(Type element) {
	if (number_of_elements + 1 == capacity) {												   //realloc
		grow();
	}
	array[(begin_index + number_of_elements) % capacity] = element;						   //add element
	number_of_elements++;						  
}

My_Type void MyDeque<Type>::pop_back() {
	if (is_empty()) return;
	number_of_elements--;																   //delete
	if (number_of_elements == 0)
		return;
	if (number_of_elements <= capacity / 4) {											   //realloc when size <= capacity/4
		reduce();
	}
}

My_Type void MyDeque<Type>::pop_front() {
	if (is_empty()) return;
	begin_index = (begin_index + 1) % capacity;														   //delete
	number_of_elements--;
	if (number_of_elements == 0)
		return;
	if (number_of_elements <= capacity / 4) {											   //realloc
		reduce();
	}
}

My_Type size_t MyDeque<Type>::size() const {
	return number_of_elements;
}

My_Type Type& MyDeque<Type>::operator [] (size_t index) {
	return array[(begin_index + index) % capacity];
}

My_Type Type MyDeque<Type>::operator [] (size_t index) const {
	return array[(begin_index + index) % capacity];
}

My_Type void MyDeque<Type>::output() {
	for (size_t i = 0; i < number_of_elements; ++i)
		std::cout << (*this)[i] << ' ';
	std::cout << std::endl;
}

My_Type bool MyDeque<Type>::is_empty() const {
	return (number_of_elements == 0);
}

My_Type Type MyDeque<Type>::front() const {
	return array[begin_index];
}

My_Type Type MyDeque<Type>::back() const {
	return array[(begin_index + number_of_elements - 1) % capacity];
}

My_Type Type& MyDeque<Type>::front() {
	return array[begin_index];
}

My_Type Type& MyDeque<Type>::back() {
	return array[(begin_index + number_of_elements - 1) % capacity];
}

