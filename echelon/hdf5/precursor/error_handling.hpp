//  Copyright (c) 2012-2015 Christopher Hinz
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef ECHELON_HDF5_PRECURSOR_ERROR_HANDLING_HPP
#define ECHELON_HDF5_PRECURSOR_ERROR_HANDLING_HPP

#include <hdf5.h>
#include <stdexcept>
#include <system_error>
#include <utility>

#include <boost/assert.hpp>

#define ECHELON_ASSERT_MSG(expr, msg) BOOST_ASSERT_MSG(expr, msg)

#if defined(NDEBUG)

#define ECHELON_VERIFY_MSG(expr, msg) ((void)(expr))

#else

#define ECHELON_VERIFY_MSG(expr, msg) ECHELON_ASSERT_MSG(expr, msg)

#endif

namespace echelon
{
namespace hdf5
{
namespace precursor
{

class hdf5_category : public std::error_category
{
public:
    virtual ~hdf5_category() = default;

    const char* name() const noexcept override;

    std::string message(int condition) const override;
};

const std::error_category& get_hdf5_category();

class exception : public std::system_error
{
public:
    explicit exception(int ev_);

    explicit exception(int ev_, std::string what_);

    virtual ~exception() noexcept = default;
};

class hdf5_error : public exception
{
public:
    hdf5_error(hid_t major_num_, hid_t minor_num_);

    hdf5_error(hid_t major_num_, hid_t minor_num_, std::string what_);

    virtual ~hdf5_error() noexcept = default;

    hid_t minor_num() const;

private:
    hid_t minor_num_;
};

class error_class
{
public:
    explicit error_class(hid_t id_);

    ~error_class();

    error_class(const error_class&) = delete;
    error_class(error_class&&) = default;

    error_class& operator=(const error_class&) = delete;
    error_class& operator=(error_class&&) = default;

    hid_t id() const;

private:
    hid_t id_;
};

class error_message
{
public:
    explicit error_message(hid_t id_);

    error_message(const error_message&) = delete;
    error_message(error_message&&) = default;

    error_message& operator=(const error_message&) = delete;
    error_message& operator=(error_message&&) = default;

    ~error_message();

    hid_t id() const;

private:
    hid_t id_;
};

const error_class& get_echelon_error_class();

const error_message& get_unable_to_obtain_ref_msg();
const error_message& get_no_associated_name_msg();

void push_error_onto_stack(const char* file, const char* func, unsigned line,
                           const error_class& err_class, const error_message& major_msg,
                           const error_message& minor_msg, const std::string& desc);

void throw_on_hdf5_error();

void enable_error_handling();
}
}
}

#endif
