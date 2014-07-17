#ifndef ALGORITHM_HPP
#define ALGORITHM_HPP

#include <algorithm>

namespace ex
{
    using namespace std;
    
    template <class T, class Container >
    bool contains(const Container & container, const T & value)
    {
        return find(container.begin(), container.end(), value) != container.end();
    }
}

#endif//ALGORITHM_HPP
