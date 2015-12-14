//  Copyright (c) 2012-2015 Christopher Hinz
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <echelon/hdf5/precursor/error_handling.hpp>

#include <exception>
#include <map>
#include <string>
#include <utility>
#include <functional>
#include <cassert>
#include <vector>

namespace echelon
{
namespace hdf5
{
namespace precursor
{

const char* hdf5_category::name() const noexcept
{
    return "echelon.hdf5";
}

std::string hdf5_category::message(int condition) const
{
    H5E_type_t msg_type;
    auto msg_size = H5Eget_msg(condition, &msg_type, nullptr, 0);

    std::vector<char> buffer(msg_size + 1);

    H5Eget_msg(condition, &msg_type, buffer.data(), buffer.size());

    return std::string(buffer.data(), buffer.size());
}

const std::error_category& get_hdf5_category()
{
    static hdf5_category instance;

    return instance;
}

exception::exception(int ev_) : std::system_error(ev_, get_hdf5_category())
{
}

exception::exception(int ev_, std::string what_)
: std::system_error(ev_, get_hdf5_category(), std::move(what_))
{
}

hdf5_error::hdf5_error(hid_t major_num_, hid_t minor_num_)
: exception(major_num_), minor_num_(minor_num_)
{
}

hdf5_error::hdf5_error(hid_t major_num_, hid_t minor_num_, std::string what_)
: exception(major_num_, std::move(what_)), minor_num_(minor_num_)
{
}

hid_t hdf5_error::minor_num() const
{
    return minor_num_;
}

error_class::error_class(hid_t id_) : id_(id_)
{
}

error_class::~error_class()
{
    if (id_ > 0)
    {
        H5Eunregister_class(id_);
    }
}

hid_t error_class::id() const
{
    return id_;
}

error_message::error_message(hid_t id_) : id_(id_)
{
}

error_message::~error_message()
{
    H5Eclose_msg(id_);
}

hid_t error_message::id() const
{
    return id_;
}

namespace
{

extern "C" {

static herr_t kubus_hdf5_translate_error_stack(unsigned, const H5E_error2_t* err_desc,
                                               void* client_data)
{
    std::exception_ptr& current_exception = *static_cast<std::exception_ptr*>(client_data);

    if (current_exception)
    {
        try
        {
            try
            {
                std::rethrow_exception(current_exception);
            }
            catch (...)
            {
                std::throw_with_nested(
                    hdf5_error(err_desc->maj_num, err_desc->min_num, err_desc->desc));
            }
        }
        catch (...)
        {
            current_exception = std::current_exception();
        }
    }
    else
    {
        current_exception = std::make_exception_ptr(
            hdf5_error(err_desc->maj_num, err_desc->min_num, err_desc->desc));
    }

    return 0;
}
}

struct error_handling_enabler
{
    error_handling_enabler()
    {
        enable_error_handling();
    }
};

error_handling_enabler eh_enabler = error_handling_enabler();
}

void throw_on_hdf5_error()
{
    std::exception_ptr nested_exception;

    herr_t result = H5Ewalk2(H5E_DEFAULT, H5E_WALK_UPWARD, &kubus_hdf5_translate_error_stack,
                             &nested_exception);

    if (result < 0)
        throw_on_hdf5_error();

    if (nested_exception)
        std::rethrow_exception(nested_exception);
}

const error_class& get_echelon_error_class()
{
    static error_class echelon_error_class(H5Eregister_class("generic", "echelon", nullptr));

    if (echelon_error_class.id() < 0)
        throw_on_hdf5_error();

    return echelon_error_class;
}

const error_message& get_unable_to_obtain_ref_msg()
{
    static error_message unable_to_obtain_ref__msg(
        H5Ecreate_msg(get_echelon_error_class().id(), H5E_MAJOR, "Unable to obtain a reference."));

    return unable_to_obtain_ref__msg;
}

const error_message& get_no_associated_name_msg()
{
    static error_message no_associated_name_msg(
        H5Ecreate_msg(get_echelon_error_class().id(), H5E_MINOR, "No associated name is found."));

    return no_associated_name_msg;
}

void push_error_onto_stack(const char* file, const char* func, unsigned line,
                           const error_class& err_class, const error_message& major_msg,
                           const error_message& minor_msg, const std::string& desc)
{
    if (H5Epush2(H5E_DEFAULT, file, func, line, err_class.id(), major_msg.id(), minor_msg.id(),
                 desc.c_str()) < 0)
        throw_on_hdf5_error();
}

void enable_error_handling()
{
    // TODO: Make this function thread-safe.

    H5Eset_auto2(H5E_DEFAULT, nullptr, nullptr);
}
}
}
}
