#ifndef ECHELON_ATTRIBUTE_REPOSITORY_HPP
#define ECHELON_ATTRIBUTE_REPOSITORY_HPP

#include <echelon/type.hpp>
#include <echelon/type_factory.hpp>
#include <echelon/attribute.hpp>

#include <memory>
#include <map>
#include <string>
#include <exception>

namespace echelon
{

class non_existing_attribute_exception: public std::exception
{
public:
    non_existing_attribute_exception(const std::string& what_)
    : what_(what_)
    {
    }

    const char* what() const noexcept
    {
        return what_.c_str();
    }
    private:
    std::string what_;
};

template<typename Parent>
class attribute_repository
{
public:
    explicit attribute_repository(const Parent& parent_)
    : parent_(&parent_)
    {}

    attribute add(const std::string& name, const type& datatype)
    {
        return attribute(object(*parent_), name, datatype);
    }

    template<typename T>
    attribute add(const std::string& name)
    {
        return add(name,get_hdf5_type<T>());
    }

    template<typename T>
    attribute add(const std::string& name,const T& value)
    {
        attribute attr = add<T>(name);

        attr <<= value;

        return attr;
    }

    attribute operator[](const std::string& name)const
    {
        return attribute(object(*parent_),name);
    }
private:
    const Parent* parent_;
};

}

#endif
