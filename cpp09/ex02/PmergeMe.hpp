#ifndef PMERGEME_HPP
#define PMERGEME_HPP

#include <vector>
#include <deque>
#include <string>
#include <cstddef>

struct Element {
    size_t id;
    int value;
    std::vector<Element> history;

    Element();
    Element(size_t idVal, int v);
    Element(const Element &other);
    Element &operator=(const Element &other);
    ~Element();
};

class PmergeMe {
private:
    std::vector<int> _vec;
    std::deque<int> _deq;

    static bool compareElement(const Element &a, const Element &b);
    static std::vector<size_t> generateJacobsthalSequence(size_t n);

    static void fordJohnsonVector(std::vector<Element> &elems);
    static void fordJohnsonDeque(std::deque<Element> &elems);

public:
    PmergeMe();
    PmergeMe(const PmergeMe &other);
    PmergeMe &operator=(const PmergeMe &other);
    ~PmergeMe();

    bool parseInput(int argc, char **argv);
    void run();
};

#endif
