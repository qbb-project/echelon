#ifndef ECHELON_HDF5_NOT_IMPLEMENTED_EXCEPTION_HPP
#define ECHELON_HDF5_NOT_IMPLEMENTED_EXCEPTION_HPP

#include <exception>
#include <string>
#include <utility>

namespace echelon
{
namespace hdf5
{
class not_implemented_exception : public std::exception
{
public:
    explicit not_implemented_exception(std::string what_) : what_{std::move(what_)}
    {
    }

    const char* what() const noexcept override
    {
        return what_.c_str();
    }

private:
    std::string what_;
};
}
}

#endif