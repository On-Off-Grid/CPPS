template <typename T>
const char* Array<T>::OutOfBoundsException::what() const throw()
{
	return "Array index out of bounds";
}

template <typename T>
Array<T>::Array() : _data(0), _size(0)
{
}

template <typename T>
Array<T>::Array(unsigned int n) : _data(0), _size(n)
{
	if (_size != 0)
		_data = new T[_size]();
}

template <typename T>
Array<T>::Array(const Array& other) : _data(0), _size(other._size)
{
	if (_size != 0)
	{
		_data = new T[_size]();
		for (unsigned int index = 0; index < _size; ++index)
			_data[index] = other._data[index];
	}
}

template <typename T>
Array<T>& Array<T>::operator=(const Array& other)
{
	if (this != &other)
	{
		T* newData = 0;

		if (other._size != 0)
		{
			newData = new T[other._size]();
			for (unsigned int index = 0; index < other._size; ++index)
				newData[index] = other._data[index];
		}
		delete[] _data;
		_data = newData;
		_size = other._size;
	}
	return *this;
}

template <typename T>
Array<T>::~Array()
{
	delete[] _data;
}

template <typename T>
T& Array<T>::operator[](unsigned int index)
{
	if (index >= _size)
		throw OutOfBoundsException();
	return _data[index];
}

template <typename T>
const T& Array<T>::operator[](unsigned int index) const
{
	if (index >= _size)
		throw OutOfBoundsException();
	return _data[index];
}

template <typename T>
unsigned int Array<T>::size() const
{
	return _size;
}
