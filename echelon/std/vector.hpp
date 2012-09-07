#ifndef ECHELON_STD_VECTOR_HPP
#define ECHELON_STD_VECTOR_HPP

#include <vector>
#include <cassert>

namespace echelon
{

template<typename T>
inline void reshape(std::vector<T>& container,
                    const std::vector<std::size_t>& new_shape)
{
    assert(new_shape.size() == 1);

    container.resize(new_shape[0]);
}

template<typename T,typename Iterator>
inline void fill(std::vector<T>& container,Iterator first,Iterator last)
{
    container.clear();

    for(Iterator iter = first; iter != last;++iter)
    {
        container.push_back(*iter);
    }
}

}

#endif
