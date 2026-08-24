#include "identify.hpp"
#include "classes.hpp"
#include <ctime>
#include <cstdlib>

static void testKnownTypes(void)
{
    A a;
    B b;
    C c;

    identify(&a);
    identify(a);
    identify(&b);
    identify(b);
    identify(&c);
    identify(c);
}

static void testGeneratedTypes(void)
{
    for (int i = 0; i < 3; ++i)
    {
        Base* p = generate();
        identify(p);
        identify(*p);
        delete p;
    }
}

int main()
{
    ::srand(static_cast<unsigned int>(std::time(NULL)));
    testKnownTypes();
    testGeneratedTypes();
    return 0;
}
