#pragma once
template<typename T>
class dArr{
private:
	unsigned cap;
	unsigned numItems;
	T* *array;

public:
	dArr(unsigned cap = 5);
	dArr(const dArr& obj);
	~dArr();
	T& operator[](int index);

	// Functions
	void add(const T element);
	void remove(int index);
	unsigned size()const;
	void clear();
	void initialize(unsigned from);
	void expand();
};

template<typename T>
dArr<T>::dArr(unsigned cap){
	this->cap = cap;
	this->numItems = 0;
	this->array = new T*[this->cap];
	this->initialize(0);
}

template<typename T>
dArr<T>::dArr(const dArr& obj){
	this->cap = obj.cap;
	this->numItems = obj.numItems;
	this->array = new T*[this->cap];
	for (size_t i = 0; i < this->numItems; i++){
		this->array[i] = new T(*obj.array[i]);
	}
	this->initialize(this->numItems);
}

template<typename T>
dArr<T>::~dArr(){
	for (size_t i = 0; i < this->numItems; i++){
		delete this->array[i];
	}
	delete[] this->array;
}

template<typename T>
T& dArr<T>::operator[](int index){
	return *this->array[index];
}



//// INITIALIZE //// 
template<typename T>
void dArr<T>::initialize(unsigned from){

	for (size_t i = from; i < this->cap; i++){
		this->array[i] = nullptr;
	}
}

//// EXPAND //// 
template<typename T>
void dArr<T>::expand(){
	this->cap *= 2;

	T* *tempArr = new T*[this->cap];

	for (size_t i = 0; i < this->numItems; i++){
		tempArr[i] = this->array[i];
	}
	delete [] this->array;
	this->array = tempArr;
	initialize(this->numItems);
}

//// ADD //// 
template<typename T>
void dArr<T>::add(const T element){
	if (this->numItems >= this->cap)
		this->expand();
	this->array[this->numItems++] = new T(element);
}

//// REMOVE //// 
template<typename T>
void dArr<T>::remove(int index){
	if (index < 0 || index >= this->numItems)
		throw("OUT OF BOUNDS EXEPTION!");

	delete this->array[index];
	this->array[index] = this->array[this->numItems - 1];
	this->array[--numItems] = nullptr;
}

//// SIZE //// 
template<typename T>
unsigned dArr<T>::size()const{
	return this->numItems;
}

//// CLEAR //// 
template<typename T>
void dArr<T>::clear(){
	for (size_t i = 0; i < this->numItems; i++){
		delete this->array[i];
	}
	initialize(0);
	this->numItems = 0;
}