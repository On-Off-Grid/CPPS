#ifndef ITER_HPP
# define ITER_HPP

#include <cstddef>

template <typename T, typename F>
void iter(T* array, const std::size_t length, F function)
{
	for (std::size_t index = 0; index < length; ++index)
		function(array[index]);
}

template <typename T, typename F>
void iter(const T* array, const std::size_t length, F function)
{
	for (std::size_t index = 0; index < length; ++index)
		function(array[index]);
}

#endif
