#ifndef ARRAY_HPP
# define ARRAY_HPP

#include <cstddef>
#include <exception>

template <typename T>
class Array
{
public:
	class OutOfBoundsException : public std::exception
	{
	public:
		virtual const char* what() const throw();
	};

	Array();
	explicit Array(unsigned int n);
	Array(const Array& other);
	Array& operator=(const Array& other);
	~Array();

	T& operator[](unsigned int index);
	const T& operator[](unsigned int index) const;
	unsigned int size() const;

private:
	T* _data;
	unsigned int _size;
};

#include "Array.tpp"

#endif
