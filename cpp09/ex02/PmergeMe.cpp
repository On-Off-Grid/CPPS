#include "PmergeMe.hpp"
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <climits>
#include <algorithm>
#include <sys/time.h>
#include <iomanip>

Element::Element() : id(0), value(0) {}

Element::Element(size_t idVal, int v) : id(idVal), value(v) {}

Element::Element(const Element &other) : id(other.id), value(other.value), history(other.history) {}

Element &Element::operator=(const Element &other) {
    if (this != &other) {
        id = other.id;
        value = other.value;
        history = other.history;
    }
    return *this;
}

Element::~Element() {}

PmergeMe::PmergeMe() {}

PmergeMe::PmergeMe(const PmergeMe &other) : _vec(other._vec), _deq(other._deq) {}

PmergeMe &PmergeMe::operator=(const PmergeMe &other) {
    if (this != &other) {
        _vec = other._vec;
        _deq = other._deq;
    }
    return *this;
}

PmergeMe::~PmergeMe() {}

bool PmergeMe::compareElement(const Element &a, const Element &b) {
    return a.value < b.value;
}

std::vector<size_t> PmergeMe::generateJacobsthalSequence(size_t n) {
    std::vector<size_t> jacob;
    if (n == 0)
        return jacob;

    size_t j0 = 1;
    size_t j1 = 3;

    jacob.push_back(j0);
    if (n == 1)
        return jacob;

    jacob.push_back(j1);
    while (true) {
        size_t nextJ = j1 + 2 * j0;
        jacob.push_back(nextJ);
        if (nextJ >= n)
            break;
        j0 = j1;
        j1 = nextJ;
    }
    return jacob;
}

void PmergeMe::fordJohnsonVector(std::vector<Element> &elems) {
    if (elems.size() <= 1)
        return;

    bool hasStraggler = (elems.size() % 2 != 0);
    Element straggler;
    if (hasStraggler) {
        straggler = elems.back();
        elems.pop_back();
    }

    std::vector<Element> winners;
    for (size_t i = 0; i < elems.size(); i += 2) {
        if (elems[i].value > elems[i + 1].value) {
            elems[i].history.push_back(elems[i + 1]);
            winners.push_back(elems[i]);
        } else {
            elems[i + 1].history.push_back(elems[i]);
            winners.push_back(elems[i + 1]);
        }
    }

    fordJohnsonVector(winners);

    std::vector<Element> mainChain;
    std::vector<Element> pending;

    for (size_t i = 0; i < winners.size(); ++i) {
        Element w = winners[i];
        Element l = w.history.back();
        w.history.pop_back();

        if (i == 0) {
            mainChain.push_back(l);
            mainChain.push_back(w);
        } else {
            mainChain.push_back(w);
            pending.push_back(l);
        }
    }

    if (!pending.empty()) {
        size_t k = pending.size();
        std::vector<size_t> jacob = generateJacobsthalSequence(k);
        std::vector<size_t> insertOrder;
        size_t last = 0;
        for (size_t i = 0; i < jacob.size(); ++i) {
            size_t val = jacob[i];
            size_t current = std::min(val, k);
            for (size_t idx = current; idx > last; --idx) {
                insertOrder.push_back(idx);
            }
            last = current;
            if (last >= k)
                break;
        }

        for (size_t i = 0; i < insertOrder.size(); ++i) {
            size_t pIdx = insertOrder[i] - 1;
            Element toInsert = pending[pIdx];
            Element pairedWinner = winners[pIdx + 1];

            std::vector<Element>::iterator itW = mainChain.begin();
            while (itW != mainChain.end() && itW->id != pairedWinner.id) {
                ++itW;
            }

            std::vector<Element>::iterator pos = std::lower_bound(mainChain.begin(), itW, toInsert, compareElement);
            mainChain.insert(pos, toInsert);
        }
    }

    if (hasStraggler) {
        std::vector<Element>::iterator pos = std::lower_bound(mainChain.begin(), mainChain.end(), straggler, compareElement);
        mainChain.insert(pos, straggler);
    }

    elems = mainChain;
}

void PmergeMe::fordJohnsonDeque(std::deque<Element> &elems) {
    if (elems.size() <= 1)
        return;

    bool hasStraggler = (elems.size() % 2 != 0);
    Element straggler;
    if (hasStraggler) {
        straggler = elems.back();
        elems.pop_back();
    }

    std::deque<Element> winners;
    for (size_t i = 0; i < elems.size(); i += 2) {
        if (elems[i].value > elems[i + 1].value) {
            elems[i].history.push_back(elems[i + 1]);
            winners.push_back(elems[i]);
        } else {
            elems[i + 1].history.push_back(elems[i]);
            winners.push_back(elems[i + 1]);
        }
    }

    fordJohnsonDeque(winners);

    std::deque<Element> mainChain;
    std::deque<Element> pending;

    for (size_t i = 0; i < winners.size(); ++i) {
        Element w = winners[i];
        Element l = w.history.back();
        w.history.pop_back();

        if (i == 0) {
            mainChain.push_back(l);
            mainChain.push_back(w);
        } else {
            mainChain.push_back(w);
            pending.push_back(l);
        }
    }

    if (!pending.empty()) {
        size_t k = pending.size();
        std::vector<size_t> jacob = generateJacobsthalSequence(k);
        std::vector<size_t> insertOrder;
        size_t last = 0;
        for (size_t i = 0; i < jacob.size(); ++i) {
            size_t val = jacob[i];
            size_t current = std::min(val, k);
            for (size_t idx = current; idx > last; --idx) {
                insertOrder.push_back(idx);
            }
            last = current;
            if (last >= k)
                break;
        }

        for (size_t i = 0; i < insertOrder.size(); ++i) {
            size_t pIdx = insertOrder[i] - 1;
            Element toInsert = pending[pIdx];
            Element pairedWinner = winners[pIdx + 1];

            std::deque<Element>::iterator itW = mainChain.begin();
            while (itW != mainChain.end() && itW->id != pairedWinner.id) {
                ++itW;
            }

            std::deque<Element>::iterator pos = std::lower_bound(mainChain.begin(), itW, toInsert, compareElement);
            mainChain.insert(pos, toInsert);
        }
    }

    if (hasStraggler) {
        std::deque<Element>::iterator pos = std::lower_bound(mainChain.begin(), mainChain.end(), straggler, compareElement);
        mainChain.insert(pos, straggler);
    }

    elems = mainChain;
}

bool PmergeMe::parseInput(int argc, char **argv) {
    for (int i = 1; i < argc; ++i) {
        std::stringstream ss(argv[i]);
        std::string token;
        while (ss >> token) {
            size_t start = 0;
            if (token[0] == '+')
                start = 1;
            if (token.length() <= start)
                return false;

            for (size_t c = start; c < token.length(); ++c) {
                if (!std::isdigit(token[c]))
                    return false;
            }

            char *endPtr;
            long val = std::strtol(token.c_str(), &endPtr, 10);
            if (*endPtr != '\0' || val <= 0 || val > INT_MAX)
                return false;

            _vec.push_back(static_cast<int>(val));
            _deq.push_back(static_cast<int>(val));
        }
    }
    return !_vec.empty();
}

void PmergeMe::run() {
    std::cout << "Before: ";
    for (size_t i = 0; i < _vec.size(); ++i) {
        std::cout << _vec[i] << (i + 1 == _vec.size() ? "" : " ");
    }
    std::cout << std::endl;

    // Vector timing
    std::vector<Element> vecElems;
    for (size_t i = 0; i < _vec.size(); ++i) {
        vecElems.push_back(Element(i, _vec[i]));
    }

    struct timeval startVec, endVec;
    gettimeofday(&startVec, NULL);
    fordJohnsonVector(vecElems);
    gettimeofday(&endVec, NULL);

    double vecTime = (endVec.tv_sec - startVec.tv_sec) * 1e6 + (endVec.tv_usec - startVec.tv_usec);

    // Deque timing
    std::deque<Element> deqElems;
    for (size_t i = 0; i < _deq.size(); ++i) {
        deqElems.push_back(Element(i, _deq[i]));
    }

    struct timeval startDeq, endDeq;
    gettimeofday(&startDeq, NULL);
    fordJohnsonDeque(deqElems);
    gettimeofday(&endDeq, NULL);

    double deqTime = (endDeq.tv_sec - startDeq.tv_sec) * 1e6 + (endDeq.tv_usec - startDeq.tv_usec);

    std::cout << "After:  ";
    for (size_t i = 0; i < vecElems.size(); ++i) {
        std::cout << vecElems[i].value << (i + 1 == vecElems.size() ? "" : " ");
    }
    std::cout << std::endl;

    std::cout << "Time to process a range of " << _vec.size() << " elements with std::vector : "
              << std::fixed << std::setprecision(5) << vecTime << " us" << std::endl;
    std::cout << "Time to process a range of " << _deq.size() << " elements with std::deque  : "
              << std::fixed << std::setprecision(5) << deqTime << " us" << std::endl;
}
