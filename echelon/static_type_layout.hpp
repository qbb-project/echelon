#ifndef ECHELON_STATIC_TYPE_LAYOUT_HPP
#define ECHELON_STATIC_TYPE_LAYOUT_HPP

namespace echelon
{

enum class static_type_layout_category
{
    generic,
    packed
};

template<typename T>
struct static_type_layout;

struct id_offset_pair
{
    constexpr id_offset_pair(const char* id,std::size_t offset)
    :id(id),offset(offset)
    {}

    const char* id;
    std::size_t offset;
};

struct id_size_pair
{
    constexpr id_size_pair(const char* id,std::size_t size)
    :id(id),size(size)
    {}

    const char* id;
    std::size_t size;
};

}

#endif
