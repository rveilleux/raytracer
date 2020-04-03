#pragma once
#include <iostream>
#pragma warning (disable: 26446)

template<typename T>
class Dynarray {
public:
	explicit Dynarray(const int size)
		: _size(size)
		, _storage(std::make_unique<T[]>(size))
	{
		//_storage = new T[size];
		//std::cout << "Dynarray allocating " << (sizeof(T) * size) << " bytes, address=" << _storage << std::endl;
	}

	//Dynarray(const Dynarray& source) = delete;

//	Dynarray(Dynarray&& source) {
//		//std::cout << "Dynarray move-construct source address=" << source._storage << std::endl;
////		std::swap(_storage, source._storage);
////		std::swap(_size, source._size);
//		_size = std::move(source._size);
//		_storage = std::move(source._storage);
//	}

	//Dynarray& operator=(const Dynarray&) = delete;

	//~Dynarray() {
	//	//std::cout << "Dynarray destroying delete[] address=" << _storage << std::endl;
	//	delete[] _storage;
	//}

	const T& operator[](const int index) const noexcept { return _storage[index]; }
	T& operator[](const int index) noexcept { return _storage[index]; }
	T* GetData() const noexcept { return _storage.get(); }
	int GetSize() const noexcept { return _size; }

private:
	int _size = 0;
	//T* _storage = nullptr;
	std::unique_ptr<T[]> _storage;
};

typedef Dynarray<uint8_t> TBytes;
