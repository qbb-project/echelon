#ifndef ECHELON_BROKEN_CONTRACT_EXCEPTION_HPP
#define ECHELON_BROKEN_CONTRACT_EXCEPTION_HPP

#include <string>
#include <exception>

namespace echelon
{

class broken_contract_exception: public std::exception
{
public:
    broken_contract_exception(const std::string& what_)
    : what_(what_)
    {
    }

    ~broken_contract_exception() noexcept {}

    const char* what() const noexcept
    {
        return what_.c_str();
    }
private:
    std::string what_;
};

}

#endif
