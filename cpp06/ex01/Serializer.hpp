#ifndef SERIALIZER_HPP
#define SERIALIZER_HPP

#include <stdint.h>
struct Data;

class Serializer
{
public:
    static uintptr_t serialize(Data* ptr);
    static Data* deserialize(uintptr_t raw);

private:
    Serializer();
    ~Serializer();
    Serializer(const Serializer&);
    Serializer& operator=(const Serializer&);
};

#endif
