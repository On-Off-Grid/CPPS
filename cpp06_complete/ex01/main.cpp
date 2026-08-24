#include "Serializer.hpp"
#include "Data.hpp"
#include <iostream>


int main()
{
    Data d;
    d.value = 42;
    uintptr_t raw = Serializer::serialize(&d);
    Data* pd = Serializer::deserialize(raw);
    std::cout << "original addr: " << &d << std::endl;
    std::cout << "deserialized:  " << pd << std::endl;
    std::cout << "value:         " << pd->value << std::endl;
    std::cout << "pointer match: " << (pd == &d ? "yes" : "no") << std::endl;
    return 0;
}
