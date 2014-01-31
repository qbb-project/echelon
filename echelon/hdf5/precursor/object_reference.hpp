//  Copyright (c) 2012-2014 Christopher Hinz
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef ECHELON_HDF5_PRECURSOR_OBJECT_REFERENCE_HPP
#define ECHELON_HDF5_PRECURSOR_OBJECT_REFERENCE_HPP

#include <hdf5.h>
#include <string>

namespace echelon
{
namespace hdf5
{
namespace precursor
{
class dataset;

class object_reference
{
public:
    /*
     *  \warning  I am not sure, if HDF5 really denotes a null reference with a
     * zero-initialized
     *            hobj_ref_t object, since the HDF5 documentation is (at best)
     * very vague about such matters.
     *            I got the idea while studying the h5py source code. Since h5py
     * seems to be well tested, this
     *            assumption should be valid. To be forearmed against future
     * changes, which could break this
     *            assumption, we should verify this behavior via unit tests.
     */
    object_reference() = default;

    explicit object_reference(hid_t obj_id_);
    explicit object_reference(hobj_ref_t obj_ref_);
    object_reference(hid_t loc_id_, const std::string& name_);

    const hobj_ref_t& native_ref() const;

    H5O_type_t get_object_type(hid_t valid_obj_id) const;

    hid_t dereference(hid_t valid_obj_id) const;

    explicit operator bool() const;

private:
    hobj_ref_t obj_ref_;
};
}
}
}

#endif
