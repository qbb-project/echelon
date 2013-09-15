//  Copyright (c) 2012 Christopher Hinz
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef ECHELON_BROKEN_CONTRACT_EXCEPTION_HPP
#define ECHELON_BROKEN_CONTRACT_EXCEPTION_HPP

#include <string>
#include <exception>
#include <utility>

namespace echelon
{

/** \brief Exception, which is thrown, if a require_*-type method can't fulfill
 * the contract.
 */
class broken_contract_exception : public std::exception
{
public:
    /** \brief Creates a new exception with a given error description.
     *
     * \param what_ error description
     */
    broken_contract_exception(std::string what_) : what_(std::move(what_))
    {
    }

    /** \brief The destructor
     */
    ~broken_contract_exception() noexcept
    {
    }

    /** \brief An associated error description.
     */
    const char* what() const noexcept override
    {
        return what_.c_str();
    }

private:
    std::string what_;
};
}

#endif
