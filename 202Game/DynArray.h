#include <array>
#include <cassert>


// dynamic, templated array with variable size 
template<typename T, size_t _size>
class DynArray {
public:
	DynArray() = default; 
	void add(const T& obj); 
	void remove(size_t index);
	inline size_t size() { return count; }
	inline size_t maxSize() { return _size; }
	void clear();

	T* begin();
	T* end();

	T& operator[](size_t index);

private:
	std::array<T, _size> data;
	size_t count = 0;
};

template<typename T, size_t _size>
inline void DynArray<T, _size>::add(const T& obj) {
	data[count] = obj;
	count = (count+1) % _size;
}

template<typename T, size_t _size>
inline void DynArray<T, _size>::remove(size_t index) {
	assert(count > 0);
	assert(index >= 0 && index < count);
	data[index] = data[--count];
}

template<typename T, size_t _size>
inline void DynArray<T, _size>::clear() {
	count = 0;
}

template<typename T, size_t _size>
inline T* DynArray<T, _size>::begin() {
	return &data[0];
}

template<typename T, size_t _size>
inline T* DynArray<T, _size>::end() {
	return &data[count];
}

template<typename T, size_t _size>
inline T& DynArray<T, _size>::operator[](size_t index) {
	return data[index];
}
