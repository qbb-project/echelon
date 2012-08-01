 
#ifndef ECHELON_TYPE_HPP
#define ECHELON_TYPE_HPP

#include <echelon/hdf5/type.hpp>

#include <string>
#include <vector>

namespace echelon
{
class type_layout;

class type
{
public:
    static type char_();
    static type short_();
    static type int_();
    static type long_();
    static type long_long();
    static type uchar();
    static type ushort();
    static type uint();
    static type ulong();
    static type ulong_long();
    static type float_();
    static type double_();

    static type string();
    static type object_reference();

    static type compound_type(const type_layout& layout);

    type clone()const;

    const hdf5::type& get_native_type()const;
private:
    explicit type(hdf5::type type_wrapper_);

    hdf5::type type_wrapper_;
};

class type_layout
{
public:
    struct element
    {
        element(const std::string& name, const type& type, std::size_t offset)
        : name(name), type_(type.clone()), offset(offset)
        {}

        std::string name;
        type type_;
        std::size_t offset;
    };

    explicit type_layout(std::size_t size_)
    : size_(size_)
    {}

    void add_element(std::string name, const type& element_type,
                     std::size_t offset)
    {
        elements_.emplace_back(name, element_type, offset);
    }

    typedef std::vector<element>::const_iterator iterator;

    iterator begin() const
    {
        using std::begin;

        return begin(elements_);
    }

    iterator end() const
    {
        using std::end;

        return end(elements_);
    }

    std::size_t size() const
    {
        return size_;
    }
private:
    std::vector<element> elements_;
    std::size_t size_;
};

}

#endif
