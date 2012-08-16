#ifndef ECHELON_STD_VECTOR_HPP
#define ECHELON_STD_VECTOR_HPP

#include <vector>

namespace echelon
{

template<typename T>
inline void require_dimensions(std::vector<T>& container,
                               const std::vector<std::size_t>& dims)
{
    container.resize(dims[0]);
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
