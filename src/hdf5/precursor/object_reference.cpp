//  Copyright (c) 2012-2014 Christopher Hinz
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <echelon/hdf5/precursor/object_reference.hpp>

#include <echelon/hdf5/precursor/dataset.hpp>
#include <echelon/hdf5/precursor/handle.hpp>

#include <echelon/hdf5/precursor/error_handling.hpp>

#include <vector>
#include <type_traits>
#include <cstring>

namespace echelon
{
namespace hdf5
{
namespace precursor
{
#if defined(__clang__) || defined(__ICC) || defined(__INTEL_COMPILER)

static_assert(std::is_trivial<object_reference>::value &&
                  std::is_standard_layout<object_reference>::value,
              "hdf5::object_reference should be a POD type");

#else

static_assert(std::is_pod<object_reference>::value, "hdf5::object_reference should be a POD type");

#endif

static_assert(sizeof(object_reference) == sizeof(hobj_ref_t),
              "hdf5::object_reference should have the same size as hobj_ref_t");

object_reference::object_reference(hid_t obj_id_)
{
    ssize_t expected_len = H5Iget_name(obj_id_, nullptr, 0);

    if (expected_len == 0)
    {
        throw no_associated_name_exception("unable to obtain a reference;"
                                           "no associated name is found",
                                           obj_id_);
    }
    else if (expected_len < 0)
    {
        throw_on_hdf5_error();
    }

    std::vector<char> name(expected_len + 1);
    auto received_len = H5Iget_name(obj_id_, name.data(), expected_len + 1);

    if (received_len < 0)
        throw_on_hdf5_error();

    ECHELON_ASSERT_MSG(received_len == expected_len, "received lenght must match expected lenght");

    hid_t file_id = H5Iget_file_id(obj_id_);

    if (file_id < 0)
        throw_on_hdf5_error();

    hdf5::precursor::handle file_handle(file_id);

    herr_t error_code = H5Rcreate(&obj_ref_, file_handle.id(), name.data(), H5R_OBJECT, -1);

    if (error_code < 0)
        throw_on_hdf5_error();
}

object_reference::object_reference(hid_t loc_id_, const std::string& name_)
{
    herr_t error_code = H5Rcreate(&obj_ref_, loc_id_, name_.c_str(), H5R_OBJECT, -1);

    if (error_code < 0)
        throw_on_hdf5_error();
}

object_reference::object_reference(hobj_ref_t obj_ref_) : obj_ref_(obj_ref_)
{
}

const hobj_ref_t& object_reference::native_ref() const
{
    return obj_ref_;
}

H5O_type_t object_reference::get_object_type(hid_t valid_obj_id) const
{
    H5O_type_t obj_type;

    herr_t error_code = H5Rget_obj_type2(valid_obj_id, H5R_OBJECT, &obj_ref_, &obj_type);

    if (error_code < 0)
        throw_on_hdf5_error();

    return obj_type;
}

hid_t object_reference::dereference(hid_t valid_obj_id) const
{
    hid_t ref_obj_id = H5Rdereference(valid_obj_id, H5R_OBJECT, &obj_ref_);

    if (ref_obj_id < 0)
        throw_on_hdf5_error();

    return ref_obj_id;
}

/*
 * warning: We use the same assumption about the representation of null
 * references here, as in the
 *          default constructor. You should look there for a full discussion of
 * this matter.
 */
object_reference::operator bool() const
{
    hobj_ref_t null_ref{};

    return memcmp(&obj_ref_, &null_ref, sizeof(hobj_ref_t)) != 0;
}
}
}
}
